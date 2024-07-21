
#include <iostream>
#include <cassert>
#include "Vector.hh"
using namespace std;
void test_basic_operations() {
    Vector<int> vec;
    assert(vec.empty());
    cout << vec.size() << endl;
    cout << vec.capacity() << endl;
    vec.push_back(1);
    cout << vec.size() << endl;
    cout << vec.capacity() << endl;
    vec.push_back(2);
    cout << vec.size() << endl;
    cout << vec.capacity() << endl;
    vec.push_back(3);
    cout << vec.size() << endl;
    cout << vec.capacity() << endl;
    
    assert(vec.size() == 3);
    assert(vec[0] == 1);
    assert(vec[1] == 2);
    assert(vec[2] == 3);

    vec.pop_back();
    assert(vec.size() == 2);
    assert(vec[0] == 1);
    assert(vec[1] == 2);

    vec.clear();
    assert(vec.empty());
}

void test_boundary_conditions() {
    Vector<int> vec(10, 5);
    assert(vec.size() == 10);
    for (size_t i = 0; i < vec.size(); ++i) {
        assert(vec[i] == 5);
    }

    vec.resize(20, 10);
    assert(vec.size() == 20);
    for (size_t i = 10; i < vec.size(); ++i) {
        assert(vec[i] == 10);
    }

    
}

void test_performance() {
    Vector<int> vec;
    const int large_size = 1000000;
    for (int i = 0; i < large_size; ++i) {
        vec.push_back(i);
    }
    assert(vec.size() == large_size);

    for (int i = 0; i < large_size; ++i) {
        assert(vec[i] == i);
    }

    for (int i = 0; i < large_size / 2; ++i) {
        vec.pop_back();
    }
    assert(vec.size() == large_size / 2);
}

void test_error_handling() {
    Vector<int> vec(10, 5);


    // 模拟内存分配失败
    try {
        Vector<int> large_vec(std::numeric_limits<size_t>::max());
        assert(false); // 如果没有抛出异常，测试失败
    } catch (...) {
        // 预期的异常
    } 
}


void test0() {
    Vector<int> vec = {2,3,4,5};
    Vector<int> vec2 = {2,3,4,5};
    cout << boolalpha << (vec == vec2) << endl;
}

int main() {
    test0();

    return 0;
}