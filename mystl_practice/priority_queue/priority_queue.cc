/**
 * @file priority_queue.cc
 * @author Ym_cmd (yim45ad@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-07-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <queue>
#include <iostream>

using std::cout;
using std::endl;
using std::vector;
using std::priority_queue;

template <typename T>
void print_queue(T& q) {
    while(!q.empty()) {
        cout << q.top() << " ";
        q.pop();
    }
    cout << '\n';
}

void create() {
    priority_queue<int> number;

    vector<int> vec{1, 3, 4, 6, 2};
    priority_queue<int> number2(vec.begin(), vec.end());
    priority_queue<int> number3 = number2;
    priority_queue<int> number4(std::move(number2));

    print_queue(number);
    print_queue(number2);
    print_queue(number3);
    print_queue(number4);

}

int main() {
    create();


    return 0;
}
