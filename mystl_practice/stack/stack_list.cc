/**
 * @file stack_list.cc
 * @author Ym_cmd (yim45ad@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-07-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <stack>
#include <iostream>
#include <list>
#include <algorithm>

using namespace std;

int main() {
    stack<int, list<int>> istack;

    istack.push(1);
    istack.push(3);
    istack.push(2);

    cout << istack.size() << endl;
    cout << istack.top() << endl;

    istack.pop();
    cout << istack.top() << endl;
    istack.pop();
    cout << istack.top() << endl;
    istack.pop();
    cout << istack.top() << endl;
    cout << istack.size() << endl;

    return 0;
}