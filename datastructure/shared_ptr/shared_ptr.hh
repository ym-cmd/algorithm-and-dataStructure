#pragma once

#include <iostream>
#include <utility>
#include <atomic>
#include <functional>

template <typename T>
class Shared_ptr 
{
public:
    // 构造函数支持自定义删除器
    explicit Shared_ptr(T* ptr = nullptr, std::function<void(T*)> deleter = default_deleter)
    : _ptr(ptr) , _deleter(deleter)
    {
        if (_ptr) 
        {
            _ref_count = new std::atomic<int>(1);
        }
        else 
        {
            _ref_count = nullptr;
        }
    }

    // 拷贝构造函数
    Shared_ptr(const Shared_ptr& other)
    {
        copy(other);
    }

    // 移动构造函数
    Shared_ptr(Shared_ptr&& other) noexcept
    {
        move(std::move(other));
    }

    // 拷贝赋值操作符
    Shared_ptr& operator=(const Shared_ptr& rhs) 
    {
        if (this != rhs) 
        {
            release(); // 释放当前资源
            copy(rhs);
        }
        return *this;
    }

    // 移动赋值操作符
    Shared_ptr& operator=(Shared_ptr&& rhs) noexcept
    {
        if (this != rhs) 
        {
            release();
            move(std::move(rhs));
        }
        return *this;
    }

    // 获取引用计数
    int use_count() const
    {
        return _ref_count ? (*_ref_count).load() : 0;
    }

    // 获取原始指针
    T* get() const
    {
        return _ptr;
    }

    // 重置指针
    void reset(T* ptr = nullptr, std::function<void(T*)> deleter = default_deleter)
    {
        release();
        if (ptr) 
        {
            _ptr = ptr;
            _deleter = deleter;
            _ref_count = new std::atomic<int>(1);
        }
    }

    // 重载*操作符
    T& operator*() const 
    {
        return *_ptr;
    }

    // 重载->操作符
    T* operator->() const
    {
        return _ptr;
    }

    // 判断指针是否为空
    explicit operator bool() const
    {
        return _ptr != nullptr;
    }

    // 析构函数
    ~Shared_ptr() 
    {
        release();
    }

private:
    T* _ptr = nullptr;
    std::atomic<int>* _ref_count = nullptr;
    std::function<void(T*)> _deleter;

    static void default_deleter(T* ptr) 
    {
        delete ptr;
    }

    void release() 
    { 
        if (_ref_count && --(*_ref_count) == 0) 
        {
            _deleter(_ptr);
            delete _ref_count;
            _ptr = nullptr;
            _ref_count = nullptr;
        }
    }

    void copy(const Shared_ptr& other)
    {
        _ptr = other._ptr;
        _deleter = other._deleter;
        _ref_count = other._ref_count;
        if (_ref_count) {
            ++(*_ref_count);
        }
    }

    void move(Shared_ptr&& other) 
    {
        _ptr = other._ptr;
        _deleter = std::move(other._deleter);
        _ref_count = other._ref_count;
        other._ptr = nullptr;
        other._ref_count = nullptr;
    }
};

// 针对数组的偏特化
template <typename T>
class Shared_ptr<T[]> 
{
public:
        // 构造函数支持自定义删除器
    explicit Shared_ptr(T* ptr = nullptr, std::function<void(T*)> deleter = default_deleter)
    : _ptr(ptr) , _deleter(deleter)
    {
        if (_ptr) 
        {
            _ref_count = new std::atomic<int>(1);
        }
        else 
        {
            _ref_count = nullptr;
        }
    }

    // 拷贝构造函数
    Shared_ptr(const Shared_ptr& other)
    {
        copy(other);
    }

    // 移动构造函数
    Shared_ptr(Shared_ptr&& other) noexcept
    {
        move(std::move(other));
    }

    // 拷贝赋值操作符
    Shared_ptr& operator=(const Shared_ptr& rhs) 
    {
        if (this != rhs) 
        {
            release(); // 释放当前资源
            copy(rhs);
        }
        return *this;
    }

    // 移动赋值操作符
    Shared_ptr& operator=(Shared_ptr&& rhs) noexcept
    {
        if (this != rhs) 
        {
            release();
            move(std::move(rhs));
        }
        return *this;
    }

    // 获取引用计数
    int use_count() const
    {
        return _ref_count ? (*_ref_count).load() : 0;
    }

    // 获取原始指针
    T* get() const
    {
        return _ptr;
    }

    // 重置指针
    void reset(T* ptr = nullptr, std::function<void(T*)> deleter = default_deleter)
    {
        release();
        if (ptr) 
        {
            _ptr = ptr;
            _deleter = deleter;
            _ref_count = new std::atomic<int>(1);
        }
    }

    T& operator[](std::size_t index) const 
    {
        return _ptr[index];
    }


    explicit operator bool() const 
    {
        return _ptr != nullptr;
    }

    ~Shared_ptr() 
    {
        release();
    }

private:
    T* _ptr = nullptr;
    std::atomic<int>* _ref_count = nullptr;
    std::function<void(T*)> _deleter;

    static void default_deleter(T* ptr) 
    {
        delete[] ptr;
    }

    void release() 
    {
        if (_ref_count && --(*_ref_count) == 0) 
        {
            _deleter(_ptr); // 使用删除器释放数组
            delete _ref_count;
            _ptr = nullptr;
            _ref_count = nullptr;
        }
    }

    void copy(const Shared_ptr& other) 
    {
        _ptr = other._ptr;
        _deleter = other._deleter;
        _ref_count = other._ref_count;
        if (_ref_count) 
        {
            ++(*_ref_count);
        }
    }

    void move(Shared_ptr&& other) 
    {
        _ptr = other._ptr;
        _deleter = std::move(other._deleter);
        _ref_count = other._ref_count;
        other._ptr = nullptr;
        other._ref_count = nullptr;
    }
    
};