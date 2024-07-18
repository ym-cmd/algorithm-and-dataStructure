#include <iostream>
#include <queue>
#include <list>
using namespace std;

void queue_list() {
    queue<int, list<int>> q;
    q.push(1);
    q.push(3);
    q.push(5);
    q.push(4);

    cout << q.size() << endl;
    cout << q.back() << endl;
    cout << q.front() << endl;

    q.pop();
    q.pop();
    q.pop();
    q.pop();
    //q.pop();
    cout << q.front() << endl;
}

int main() {
    queue_list();
    return 0;
}