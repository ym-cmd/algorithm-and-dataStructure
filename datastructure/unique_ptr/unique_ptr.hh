#pragma once

#include <utility>
#include <memory>

// 针对数组的删除器
template <typename T>
struct unique_ptr_deleter 
{
    void operator()(T* resource) const
    {
        delete[] resource;
    }
};

// 默认Deleter为std::default_delete
template <typename T, typename Deleter = std::default_delete<T>>
class UniquePtr {
public:
    explicit UniquePtr(T* ptr = nullptr, Deleter deleter = Deleter()) : _deleter(deleter), _ptr(ptr) {}

    // 禁止拷贝构造和拷贝赋值
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // 允许移动构造和移动赋值
    UniquePtr(UniquePtr&& other) noexcept;
    UniquePtr& operator=(UniquePtr&& other) noexcept;

    // 重置指针
    void reset(T* ptr = nullptr);
    
    // 获取原始指针
    T* get() const { return _ptr; }

    // 释放指针
    T* release();

    // 单对象解引用操作符
    T& operator*() const;
    T* operator->() const;

    // 检查指针是否为空
    explicit operator bool() const { return _ptr != nullptr; }

    // 析构
    ~UniquePtr();

private:
    Deleter _deleter;
    T* _ptr;
};

// 针对数组类型的偏特化
template <typename T, typename Deleter>
class UniquePtr<T[], Deleter> {
public:
    explicit UniquePtr(T* ptr = nullptr, Deleter deleter = Deleter()) : _deleter(deleter), _ptr(ptr) {}

    // 禁止拷贝构造和拷贝赋值
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // 允许移动构造和移动赋值
    UniquePtr(UniquePtr&& other) noexcept;
    UniquePtr& operator=(UniquePtr&& other) noexcept;

    // 重置指针
    void reset(T* ptr = nullptr);

    // 获取原始指针
    T* get() const { return _ptr; }

    // 释放指针
    T* release();

    // 数组的下标访问操作符
    T& operator[](std::size_t index) const;

    // 检查指针是否为空
    explicit operator bool() const { return _ptr != nullptr; }

    // 析构
    ~UniquePtr();

private:
    Deleter _deleter;
    T* _ptr;
};

// 单对象的移动构造
template <typename T, typename Deleter>
UniquePtr<T, Deleter>::UniquePtr(UniquePtr&& other) noexcept : _ptr(other._ptr)
{
    other._ptr = nullptr;
}

// 数组的移动构造
template <typename T, typename Deleter>
UniquePtr<T[], Deleter>::UniquePtr(UniquePtr&& other) noexcept : _ptr(other._ptr)
{
    other._ptr = nullptr;
}

// 单对象的移动赋值
template <typename T, typename Deleter>
UniquePtr<T, Deleter>& UniquePtr<T, Deleter>::operator=(UniquePtr&& other) noexcept 
{
    if (this != &other) 
    {
        delete _ptr;
        _ptr = other._ptr;
        other._ptr = nullptr;
    }

    return *this;
}

// 数组的移动赋值
template <typename T, typename Deleter>
UniquePtr<T[], Deleter>& UniquePtr<T[], Deleter>::operator=(UniquePtr&& other) noexcept 
{
    if (this != &other) 
    {
        delete[] _ptr;
        _ptr = other._ptr;
        other._ptr = nullptr;
    }

    return *this;
}

// 单对象的 release
template <typename T, typename Deleter>
T* UniquePtr<T, Deleter>::release()
{
    T* temp = _ptr;
    _ptr = nullptr;
    return temp;
}

// 数组的 release
template <typename T, typename Deleter>
T* UniquePtr<T[], Deleter>::release()
{
    T* temp = _ptr;
    _ptr = nullptr;
    return temp;
}

// 单对象的 reset
template <typename T, typename Deleter>
void UniquePtr<T, Deleter>::reset(T* ptr)
{
    delete _ptr;
    _ptr = ptr;
}

// 数组的 reset
template <typename T, typename Deleter>
void UniquePtr<T[], Deleter>::reset(T* ptr)
{
    delete[] _ptr;
    _ptr = ptr;
}

// 单对象的解引用操作符
template <typename T, typename Deleter>
T& UniquePtr<T, Deleter>::operator*() const
{
    return *_ptr;
}

// 数组的下标访问操作符
template <typename T, typename Deleter>
T& UniquePtr<T[], Deleter>::operator[](std::size_t index) const 
{
    return _ptr[index];
}

// 单对象的析构函数
template<typename T, typename Deleter>
UniquePtr<T, Deleter>::~UniquePtr() {
    if (_ptr) {
        _deleter(_ptr); // 删除所指向的对象
    }
}

// 数组的析构函数
template<typename T, typename Deleter>
UniquePtr<T[], Deleter>::~UniquePtr() {
    if (_ptr) {
        _deleter(_ptr); // 删除所指向的数组
    }
}

// 别名
template<typename T, typename Deleter = std::default_delete<T>>
using UniquePtrPtr = UniquePtr<T, Deleter>;

template<typename T>
using UniquePtrArray = UniquePtr<T[], unique_ptr_deleter<T>>;
