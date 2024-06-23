// epoll_chatroom.c 
// 为了教学方便，省略了错误处理
// 用了bitmap，冗余
// 超时时间没有设成相对时间，可能存在几十ms到几百ms的误差，在高并发状态下，可能永远都超不了时
#include <func.h>
#include "BitMap.h"
#include "timer.h"
#include "hashmap.h"

#define TIMEOUT 1000
#define PORT "12123"
#define BACKLOG 10
#define MAXLINE 1024
#define MAXEVENTS 1024

int tcp_listen(void) {
    struct addrinfo hints, *res, *p;

    bzero(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;    // wildcard address

    int rv;
    if ((rv = getaddrinfo(NULL, PORT, &hints, &res)) != 0) {
        error(1, rv, "getaddrinfo: %s", gai_strerror(rv));
    }

    // loop through all the results and bind to the first we can
    int sockfd;
    for (p = res; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            error(0, errno, "socket failed");
            continue;
        }

        int yes = 1;
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
            error(1, errno, "setsockopt");
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            error(0, errno, "bind");
            continue;
        }
  
        break;
    }

    freeaddrinfo(res); // all done with this structure

    if (p == NULL) {
        error(1, 0, "All address failed to bind");
    }

    if (listen(sockfd, BACKLOG) == -1) {
        error(1, errno, "listen");
    }
    
    return sockfd;
}


// get ip address, IPv4 or IPv6
void* get_ip_addr(struct sockaddr* sa)
{
    if (sa->sa_family == AF_INET) {
        return &((struct sockaddr_in*)sa)->sin_addr;
    }

    return &((struct sockaddr_in6*)sa)->sin6_addr;
}

int main(void) {

    // init timerwheel
    HashMap* hashmap = hashmapCreate();
    HashedWheelTimer* timer = hwtCreate(WHEEL_SIZE + 1);


    int listenfd = tcp_listen();
    
    /* track all file descriptors that epoll instance monitors */

    // 额外自己跟踪红黑树里面有哪些fd
    BitMap* bm = bitmap_create(16);

    // create epoll instance
    int epfd;
    if ((epfd = epoll_create(1)) == -1) {
        error(1, errno, "epoll_create");
    }

    // Add the listener to `epfd`
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = listenfd;
    
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &event) == -1) {
        error(1, errno, "epoll_ctl");
    }
    bitmap_set(bm, listenfd);

    /* Buffer where events are returned */
    struct epoll_event events[MAXEVENTS];

    struct timespec start, end;
    int timeout = TIMEOUT;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    // Main loop
    for (;;) {
        int n = epoll_wait(epfd, events, MAXEVENTS, timeout);
        if (n == -1 && errno == EINTR) {
            error(0, errno, "epoll_wait");
            continue;
        } else if (n == 0) {
            // 超时
            hwtClear(timer);
            timer->curr_idx = (timer->curr_idx + 1) % timer->size;
            timeout = TIMEOUT;
            clock_gettime(CLOCK_MONOTONIC, &start);
            continue;
        } else if (n == -1) {
            break;
        } else {
            // run through `events` for new connection and data to read 
            for (int i = 0; i < n; i++) {
                int sockfd = events[i].data.fd;

                if (!(events[i].events & EPOLLIN)) { // 没有读事件
                    // something went wrong 
                    error(0, 0, "epoll error on socket %d", sockfd);
                    epoll_ctl(epfd, EPOLL_CTL_DEL, sockfd, NULL);
                    bitmap_unset(bm, sockfd);
                    close(sockfd);
                    
                    continue;
                } else if (sockfd == listenfd) {
                    // listener is ready to read, handle new connection
                    struct sockaddr_storage cliaddr;
                    socklen_t addrlen = sizeof(cliaddr);
                    
                    int connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &addrlen);
                    if (connfd == -1) {
                        error(0, errno, "accept");
                    } else {
                        event.data.fd = connfd;
                        event.events = EPOLLIN;
                        epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &event);
                        bitmap_set(bm, connfd);

                        char ipstr[50];
                        inet_ntop(cliaddr.ss_family, get_ip_addr((struct sockaddr*)&cliaddr), ipstr, 50);
                        
                        printf("INFO: new connection from %s on socket %d\n", ipstr, connfd);
                    }


                    // connfd update
                    int slot_idx = hashmapSearch(hashmap, connfd); // 若存在返回slot，不存在返回-1
                    slot_idx = hwtUpdate(timer, connfd, slot_idx); // 插入的新的slot，也就时curr_idx的上一个槽
                    hashmapInsert(hashmap, connfd, slot_idx); //更新hashmap

                } else {
                    // handle data from a client
                    char recvline[MAXLINE];
                    int n;
                    if ((n = recv(sockfd, recvline, MAXLINE, 0)) <= 0) {
                        if (n == 0) {
                            // connection closed by client
                            printf("INFO: socket %d hung up\n", sockfd);
                        } else {
                            error(0, errno, "recv");    // something wrong
                        }
                        // remove from `epfd`
                        close(sockfd);
                        epoll_ctl(epfd, EPOLL_CTL_DEL, sockfd, NULL);
                        bitmap_unset(bm, sockfd);
                    } else {
                        // got some data from a client
                        for (size_t j = 0; j < bm->bits; j++) {
                            if (bitmap_isset(bm, j) && j != listenfd && j != sockfd) {
                                if (send(j, recvline, n, 0) == -1) {
                                    error(0, errno, "send");
                                }
                            }
                        } 
                    }

                    // connfd update
                    int slot_idx = hashmapSearch(hashmap, sockfd); // 若存在返回slot，不存在返回-1
                    slot_idx = hwtUpdate(timer, sockfd, slot_idx); // 插入的新的slot，也就是上一个curr_idx的上一个
                    hashmapInsert(hashmap, sockfd, slot_idx); // 更新hashmap

                    

                } // handle data from a client
            } // for 

            // 超时时间的相对计算
            clock_gettime(CLOCK_MONOTONIC, &end);
            timeout -= (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;
            if (timeout <= 0) {
                timeout = 0;
            }
            clock_gettime(CLOCK_MONOTONIC, &start);

        } // (n = epoll_wait) > 0
    } // while (1)
}