#include <iostream>
#include "shared_ptr.hh"

// 自定义删除器
void customDeleter(int* ptr) {
    std::cout << "Custom deleter called for pointer: " << ptr << std::endl;
    delete ptr;
}

// 自定义数组删除器
void customArrayDeleter(int* arr) {
    std::cout << "Custom array deleter called" << std::endl;
    delete[] arr;
}

void testSharedPtrWithDeleter() {
    {
        Shared_ptr<int> sp1(new int(5), customDeleter);
        std::cout << "sp1 value: " << *sp1 << ", use_count: " << sp1.use_count() << std::endl;

        Shared_ptr<int> sp2 = sp1; // 拷贝构造
        std::cout << "sp2 value: " << *sp2 << ", use_count: " << sp2.use_count() << std::endl;
    }

    {
        Shared_ptr<int[]> arrPtr(new int[3]{1, 2, 3}, customArrayDeleter);
        std::cout << "Array values: " << arrPtr[0] << ", " << arrPtr[1] << ", " << arrPtr[2] << std::endl;
        std::cout << "arrPtr use_count: " << arrPtr.use_count() << std::endl;
    }
}

int main() {
    testSharedPtrWithDeleter();
    return 0;
}
