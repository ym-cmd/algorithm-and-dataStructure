// 未完成...
#include <atomic>
#include <memory>

template<typename T>
class LockFreeQueue {
public:
    LockFreeQueue() {
        Node* dummy = new node(T());
        head.store(dummy);
        tail.store(dummy);
    }

    ~LockFreeQueue() {
        while (Node* node = head.load()) {
            head.store(node->next);
            delete node;
        }
    }

    void enqueue(T val) {
        Node* newNode = new Node(val);
        Node* oldTail = nullptr;

        while (true) {
            oldTail = tail.load();
            Node* next = oldTail->next.load();
            if (oldTail == tail.load()) {
                if (next == nullptr) {
                    if (oldTail->next.compare_exchange_weak(next, newNode)) {
                        break;
                    }
                } else {
                    tail.compare_exchange_weak(oldTail, next);
                }
            }
        }
        tail.compare_exchange_weak(oldTail, newNode);
    }

    bool dequeue(T& result) {
        Node* oldHead;
        while (true) {
            oldHead = head.load();
            Node* oldTail = tail.load();
            Node* next = oldHead->next.load();
            if (oldHead == head.load) { // 头没给改
                if (oldHead == oldTail) {
                    if (next == nullptr) {
                        return false; // 队列为空
                    }
                    // 如果队列不为空但尾指针落后，更新尾指针
                    tail.compare_exchange_weak(oldTail, next); 
                } else {
                    result = next->value;
                    if (head.compare_exchange_weak(oldHead, next)) {
                        break;
                    }
                }
            }
        }
        delete oldHead;
        return true;
    }

private:
    struct Node {
        T value;
        std::atomic<Node*> next;
        Node(const T& val) : value(val), next(nullptr) {}
    };

    std::atomic<Node*> head;
    std::atomic<Node*> tail;
};