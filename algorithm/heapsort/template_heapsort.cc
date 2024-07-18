/**
 * @file template_heapsort.cc
 * @author Ym_cmd (yim45ad@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-07-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <ostream>


using std::cout;
using std::endl;
using std::cin;
using std::vector;

template <typename T> 
void swap(T& lhs, T& rhs) {
    auto temp = lhs;
    lhs = rhs;
    rhs = temp;
}

class Point
{
public:
    Point(int ix = 0, int iy = 0)
    : _ix(ix)
    , _iy(iy)
    {
    }

    ~Point()
    {
    }

    void print() const
    {
        cout << "(" << _ix 
            << ", " << _iy 
            << ")" << endl;
    }

    double getDistance() const
    {
        return hypot(_ix, _iy);
    } 

    int getX() const
    {
        return _ix;
    }

    int getY() const
    {
        return _iy;

    }

    friend std::ostream & operator<<(std::ostream &os, const Point &rhs);
private:
    int _ix;
    int _iy;
};

std::ostream & operator<<(std::ostream &os,const Point &rhs)
{
    os << "(" << rhs._ix
       << ", " <<rhs._iy
       << ")";
    return os;
}



// 对于自定义类型有三种常见方式
// 1. 重载大于号小于号
// 2. 模板特化
// 3. 函数对象 

// 1.重载大于小于号
bool operator<(const Point& lhs, const Point& rhs) {
    if (lhs.getDistance() < rhs.getDistance()) return true;
    else if (lhs.getDistance() == rhs.getDistance()) {
        if (lhs.getX() < rhs.getX() || (lhs.getX() == rhs.getX() && lhs.getY() < rhs.getY())) return true;
    }
    return false;
}

bool operator>(const Point& lhs, const Point& rhs) {
    if (lhs.getDistance() > rhs.getDistance()) return true;
    else if (lhs.getDistance() == rhs.getDistance()) {
        if (lhs.getX() > rhs.getX() || (lhs.getX() == rhs.getX() && lhs.getY() > rhs.getY())) return true;
    }
    return false;
}


// 2.模板特化
namespace std
{
    template <>
    struct less<Point> {
        bool operator()(const Point& lhs, const Point& rhs) const {
            if (lhs.getDistance() < rhs.getDistance()) return true;
            else if (lhs.getDistance() == rhs.getDistance()) {
                if (lhs.getX() < rhs.getX() || (lhs.getX() == rhs.getX() && lhs.getY() < rhs.getY())) return true;
            }
            return false;
        }
    };

    template <>
    struct greater<Point> {
        bool operator()(const Point& lhs, const Point& rhs) const {
            if (lhs.getDistance() > rhs.getDistance()) return true;
            else if (lhs.getDistance() == rhs.getDistance()) {
                if (lhs.getX() > rhs.getX() || (lhs.getX() == rhs.getX() && lhs.getY() > rhs.getY())) return true;
            }
            return false;
        }
    };
}

// 3. 函数对象
struct ComparePoint {
    bool operator()(const Point& lhs, const Point& rhs) const {
        if (lhs.getDistance() < rhs.getDistance()) return true;
        else if (lhs.getDistance() == rhs.getDistance()) {
            if (lhs.getX() < rhs.getX() || (lhs.getX() == rhs.getX() && lhs.getY() < rhs.getY())) return true;
        }
        return false;
    }
};

template <typename T, typename Compare = std::less<T>> 
class HeapSort {
public:  
    HeapSort(T *arr, size_t size, const Compare&);  
    void heapAdjust(size_t ,size_t, const Compare&);  
    void sort(const Compare&);
    void print() const;

private:  
    void buildHeap(const Compare&);
    vector<T> _vec;
};

template <typename T, typename Compare>
void HeapSort<T, Compare>::buildHeap(const Compare& com) {
    for (int i = _vec.size() / 2 - 1; i >= 0; --i) {
        heapAdjust(i, _vec.size(), com);
    }
}

template <typename T, typename Compare>
void HeapSort<T, Compare>::sort(const Compare& com) {
    buildHeap(com);
    for (size_t i = _vec.size() - 1; i > 0; --i) {
        swap(_vec[0], _vec[i]);
        heapAdjust(0, i, com);
    }
}

template <typename T, typename Compare>
HeapSort<T, Compare>::HeapSort(T* arr, size_t size, const Compare& com) {
    _vec.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        _vec.push_back(arr[i]);
    }
    sort(com);
}


template <typename T, typename Compare>
void HeapSort<T, Compare>::heapAdjust(size_t adjust_pos, size_t arrlen, const Compare& com) {
    size_t dad = adjust_pos;
    while (true) {
        
        size_t left_son = 2 * dad + 1;
        size_t right_son = 2 * dad + 2;
            
        // 分别检查左右孩子是否存在且比当前的最大值（父节点）大
        if (left_son < arrlen && com(_vec[dad], _vec[left_son])) {
            dad = left_son;
        }

        if (right_son < arrlen && com(_vec[dad], _vec[right_son])) {
            dad = right_son;
        }

        if (dad != adjust_pos) {
            swap(_vec[adjust_pos], _vec[dad]);
            adjust_pos = dad; // 更新根节点索引为交换后的位置
        } else {
            break;
        }
    }
}


template <typename T, typename Compare>
void HeapSort<T, Compare>::print() const{
    for (const T& elem : _vec) {
        cout << elem << " ";
    }
    cout << endl;
}


int main() {  
    int arr[10] = {1, 2, 6, 3, 4, 8, 5, 7, 9, 10};
    HeapSort<int> hs(arr, 10, std::less<int>());
    hs.print();

    cout << endl;
    Point parr[5] = {{1,2}, {3,4}, {-1,2}, {4,5}, {2,5}};

    HeapSort<Point> hsPt(parr, 5, std::less<Point>());
    hsPt.print();
    

    return 0;
}