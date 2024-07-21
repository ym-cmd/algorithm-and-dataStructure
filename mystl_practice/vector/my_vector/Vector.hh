/**
 * @file Vector.hh
 * @author Ym_cmd (yim45ad@gmail.com)
 * @brief 还有很多功能没实现，拷贝构造等
 * @version 0.1
 * @date 2024-07-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once

#include <cstddef>
#include <iterator>
#include <memory>
#include <limits>
#include <stdexcept>
#include <utility>
#include <compare>
#include <initializer_list>
#include <iostream>

template <class T, class Alloc = std::allocator<T>>
class Vector {
public:
    using value_type = T;
    using allocator_type = Alloc;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using iterator = T*;
    using const_iterator = const T*;
    using reverse_iterator = std::reverse_iterator<T*>;
    using const_reverse_iterator = std::reverse_iterator<const T*>;

protected:
    iterator start;
    iterator finish;
    iterator end_of_storage;
    Alloc m_alloc;

    // 配置然后填充
    iterator allocate_and_fill(size_type n, const T& x) {
        iterator result = m_alloc.allocate(n);
        std::uninitialized_fill_n(result, n, x);
        return result;
    }
    void insert_aux(iterator position, const T& x) {
        if (finish != end_of_storage) { // 无备用空间

        } else {
            const size_type old_size = size();
            const size_type len = old_size != 0 ? 2 * old_size : 1;

            iterator new_start = m_alloc.allocate(len);
            iterator new_finish = new_start;
            try {
                new_finish = std::uninitialized_copy(start, position, new_start);
                m_alloc.construct(new_finish, x);
                ++new_finish;
            } catch (...) {
                destroy(new_start, new_finish);
                m_alloc.deallocate(new_start, len);
                throw;
            }

            destroy(begin(), end());
            deallocate();

            start = new_start;
            finish = new_finish;
            end_of_storage = new_start + len;
        }
    }

    void destroy(iterator first, iterator last) {
        for (size_type i = 0; i != (last - first); ++i) {
            m_alloc.destroy(first + i);
        }
    }
    void deallocate() {
        if (start) {
            m_alloc.deallocate(start, end_of_storage - start);
        }
    }
    // 填充并初始化
    void fill_initialize(size_type n, const T& value) {
        start = allocate_and_fill(n, value);
        finish = start + n;
        end_of_storage = finish;
    }

    iterator allocate_and_copy(size_type n, iterator first, iterator last) {
        iterator result = m_alloc.allocate(n);
        try {
            std::uninitialized_copy(first, last, result);
        } catch (...) {
            m_alloc.deallocate(result, n);
            throw;
        }
        return result;
    }

public:

    Vector(const Alloc& alloc = Alloc()) noexcept {
        start = nullptr;
        finish = nullptr;
        end_of_storage = nullptr;
        m_alloc = alloc;
    }
    Vector(std::initializer_list<T> list, const Alloc& alloc = Alloc()) 
    : m_alloc(alloc) {
        size_type n = list.size();
        start = m_alloc.allocate(n);
        end_of_storage = finish = start + n;
        auto it = list.begin();
        for (size_type i = 0; i != n; ++i) {
            m_alloc.construct(start + i, *it);
            ++it;
        }
    }
    Vector(size_type n, const T& value, const Alloc& alloc = Alloc()) 
    : m_alloc(alloc) { fill_initialize(n, value); }
    Vector(int n, const T& value, const Alloc& alloc = Alloc())
    : m_alloc(alloc) { fill_initialize(n, value); }
    Vector(long n, const T& value, const Alloc& alloc = Alloc())
    : m_alloc(alloc) { fill_initialize(n, value); }
    explicit Vector(size_type n, const Alloc& alloc = Alloc()) : m_alloc(alloc) {
        fill_initialize(n, T());
    }

    Vector(const iterator first, const iterator last, const Alloc& alloc = Alloc()) : m_alloc(alloc) {
        size_type n = last - first;
        start = m_alloc.allocate(n);
        end_of_storage = finish = start + n;
        for (size_type i = 0; i != n; ++i) {
            m_alloc.construct(start + i, *(first + i));
        }
    }


    ~Vector() noexcept {
        destroy(start, finish);
        deallocate();
    }

    reference front() {
        return *begin();
    }

    const_reference front() const {
        return *begin();
    }
    reference back() {
        return *(end() - 1);
    }
    iterator begin() {
        return start;
    }
    const_iterator begin() const {
        return start;
    }
    iterator end() {
        return finish;
    }
    size_type size() const {
        return finish - start;
    }
    size_type capacity() const {
        return end_of_storage - start;
    }
    bool empty() const {
        return finish == start;
    }
    reference operator[](size_type n) {
        return *(begin() + n);
    }
    const_reference operator[](size_type n) const {
        return *(begin() + n);
    }

    void push_back(const T& x) {
        if (finish != end_of_storage) {
            m_alloc.construct(finish, x);
            ++finish;
        } else {
            insert_aux(end(), x);
        }
    }
    void pop_back() {
        --finish;
        m_alloc.destroy(finish);
    }

    iterator erase(iterator position) {
        if (position + 1 != end()) {
            copy(position + 1, finish, position);
        }
        --finish;
        m_alloc.destroy(finish);
        return position;
    }

    iterator erase(iterator first, iterator last) {
        iterator i = std::copy(last, finish, first);
        destroy(i, finish);
        finish = finish - (last - first);
        return first;
    }
    void resize(size_type new_size, const T& x) {
        if (new_size < size()) {
            erase(begin() + new_size, end());
        } else {
            insert(end(), new_size - size(), x);
        }
    }
    void resize(size_type new_size) {
        resize(new_size, T());
    }

    void shrink_of_fit() noexcept {
        if (end_of_storage == finish) return;
        m_alloc.deallocate(finish, end_of_storage - finish);
        end_of_storage = finish;
    }
    void reserve(size_type new_capacity) {
        if (new_capacity <= capacity()) return;
        iterator new_start = allocate_and_copy(new_capacity, start, finish);
        destroy(start, finish);
        deallocate();
        
        finish = new_start + (finish - start);
        start = new_start;
        end_of_storage = start + new_capacity;
    }

    void clear() {
        erase(begin(), end());
    }

    // size = m, capacity = n, 插入元素个数为t
    // case1: t < n - m; 不扩容
    // case2: n - m < t < m; 按 2 * m 进行扩容
    // case3: n - m < t && m < t; 按 t + m 进行扩容
    void insert(iterator position, size_type n, const T& x = T()) {
        if (n != 0) {
            if (size_type(end_of_storage - finish) >= n) { // t <= n - m 插入元素小于等于剩余容量，不用扩容 
                const size_type elems_after = finish - position; 
                iterator old_finish = finish;
                if (elems_after > n) { //
                    std::uninitialized_copy(finish - n, finish, finish); // insert position front
                    finish += n;
                    std::copy_backward(position, old_finish - n, old_finish);
                    std::fill(position, position + n, x);
                } else { // 插入点之后的元素小于等于新增元素个数
                    std::uninitialized_fill_n(finish, n - elems_after, x);
                    finish += n - elems_after;
                    std::uninitialized_copy(old_finish - elems_after, old_finish, finish);
                    finish += elems_after;
                    std::fill(position, old_finish, x);
                }


            }  else { // 要扩容
                const size_type old_size = size();
                // t <= m; t = 2 * m;
                // m < t; end_of_storage - start = t + m
                const size_type len = old_size + std::max(old_size, n);
                iterator new_start = allocate_and_copy(len, start, position);
                iterator new_position = std::uninitialized_fill_n(new_start + (position - start), n, x);
                iterator new_finish = std::uninitialized_copy(position, finish, new_position);

                destroy(start, finish);
                deallocate();

                start = new_start;
                finish = new_finish;
                end_of_storage = new_start + len;
            }
        }
    }

    template <class U, class AllocU>
    friend bool operator==(const Vector<U, AllocU>& lhs, const Vector<U, AllocU>& rhs);

};

// 运算符重载
template <class T, class Alloc>
bool operator==(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs) {
    if (lhs.size() == rhs.size()) {
        for (size_t i = 0; i < lhs.size(); ++i) { 
            if (lhs[i] != rhs[i]) {
                return false;
            }
        }
        return true;
    }

    return false;   
}
