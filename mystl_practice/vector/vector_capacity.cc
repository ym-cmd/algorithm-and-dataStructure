#include <iostream>
#include <vector>

using std::cout; using std::endl;
using std::vector;

template <typename Container>
void display(const Container& con) {
    for (const auto& elem : con) {
        cout << elem << " ";
    }
    cout << endl;
}

template <typename Container>
void printCapacity(const Container& con) {
    cout << endl << "size = " << con.size() << endl;
    cout << "capacity = " << con.capacity() << endl;
}

// 扩容
// gcc 2倍 
// windows 1.5倍 

// size = m, capacity = n, 插入元素个数为t
// case1: t < n - m; 不扩容
// case2: n - m < t < m; 按 2 * m 进行扩容
// case3: n - m < t && m < t; 按 t + m 进行扩容


int main() {
    vector<int> vec = {1,2,3,4,5,6,7};
    printCapacity(vec);

    vec.push_back(5);
    printCapacity(vec);

    return 0;
}