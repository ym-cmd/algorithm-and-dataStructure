#include <stack>
#include <iostream>
#include <algorithm>

using namespace std;

int main() {
    stack<int> st;

    st.push(1);
    st.push(3);
    st.push(2);

    cout << st.size() << endl;
    cout << st.top() << endl;

    st.pop();
    cout << st.top() << endl;
    st.pop();
    cout << st.top() << endl;
    st.pop();

    // 断错误，说明list为底层，最后节点为空节点，而deque为空，则不能用top()
    cout << st.top() << endl;
    cout << st.size() << endl;

    return 0;
}