#include <iostream>
#include <vector>

using std::cout; using std::endl;
using std::vector;

int main() {
    vector<int> vec = {2, 3, 5, 7, 1};
    auto ptr = vec.data();
    cout << vec.size() << endl;
    cout << vec.capacity() << endl;
    for (int* p = ptr; p != ptr + vec.size(); ++p) {
        cout << *p << endl;
    }
    vec.push_back(4);
    cout << endl << vec.size() << endl;
    cout << vec.capacity() << endl;
    return 0;
}