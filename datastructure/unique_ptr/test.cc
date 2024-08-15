#include <iostream>
#include "unique_ptr.hh"

// 测试函数
void testUniquePtr() {
    // 测试单对象指针
    {
        UniquePtr<int> ptr1(new int(5)); // 创建UniquePtr，管理int对象
        std::cout << "ptr1 value: " << *ptr1 << std::endl; // 测试解引用

        UniquePtr<int> ptr2(std::move(ptr1)); // 测试移动构造
        std::cout << "ptr1 is null: " << (ptr1 ? "false" : "true") << std::endl;
        std::cout << "ptr2 value: " << *ptr2 << std::endl;

        ptr2.reset(new int(10)); // 测试reset
        std::cout << "ptr2 value after reset: " << *ptr2 << std::endl;

        int* rawPtr = ptr2.release(); // 测试release
        std::cout << "ptr2 is null after release: " << (ptr2 ? "false" : "true") << std::endl;
        std::cout << "rawPtr value: " << *rawPtr << std::endl;
        delete rawPtr; // 手动释放rawPtr
    }

    // 测试数组指针
    {
        UniquePtrArray<int> arrPtr(new int[3]{1, 2, 3}); // 创建UniquePtr，管理int数组
        std::cout << "Array values: " << arrPtr[0] << ", " << arrPtr[1] << ", " << arrPtr[2] << std::endl; // 测试数组下标访问

        UniquePtrArray<int> arrPtr2 = std::move(arrPtr); // 测试数组的移动构造
        std::cout << "arrPtr is null: " << (arrPtr ? "false" : "true") << std::endl;
        std::cout << "Array values in arrPtr2: " << arrPtr2[0] << ", " << arrPtr2[1] << ", " << arrPtr2[2] << std::endl;

        arrPtr2.reset(new int[3]{4, 5, 6}); // 测试数组的reset
        std::cout << "Array values in arrPtr2 after reset: " << arrPtr2[0] << ", " << arrPtr2[1] << ", " << arrPtr2[2] << std::endl;

        int* rawArr = arrPtr2.release(); // 测试数组的release
        std::cout << "arrPtr2 is null after release: " << (arrPtr2 ? "false" : "true") << std::endl;
        std::cout << "rawArr values: " << rawArr[0] << ", " << rawArr[1] << ", " << rawArr[2] << std::endl;
        delete[] rawArr; // 手动释放rawArr 
    }
}

int main() {
    testUniquePtr();
    return 0;
}
