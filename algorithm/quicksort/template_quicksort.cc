/**
 * @file template_quicksort.cc
 * @author Ym_cmd (yim45ad@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-07-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <functional>
#include <vector>

using std::vector;
using std::cout;
using std::endl;


template<typename T,typename Compare=std::less<T>> 
class MyQsort 
{ 
public:     
    MyQsort(T *arr, size_t size, Compare com);     
    void quick(int left, int right, Compare &com);     
    T medianOfThree(const T& a, const T& b, const T& c, Compare& com);     
    void print(); 
private:    
    vector<T> _vec; 
};

template<class T, class Compare>
MyQsort<T, Compare>::MyQsort(T* arr, size_t size, Compare com) {
    _vec.reserve(size);
    for (int i = 0; i < size; ++i) {
        _vec.push_back(arr[i]);
    }
    quick(0, _vec.size() - 1, com);
}

// 三数取中
template<class T, class Compare>
T MyQsort<T, Compare>::medianOfThree(const T& a, const T& b, const T& c, Compare& com) {
    if ((com(a, b) && com(b, c)) || (com(c, b) && com(b, a))) return b;
    else if ((com(b, a) && com(a, c)) || (com(c, a) && com(a, b))) return a;
    else return c;
}

template<class T>
void swap(T& lhs, T& rhs) {
    auto temp = lhs;
    lhs = rhs;
    rhs = temp;
}

template<class T, class Compare>
void MyQsort<T, Compare>::quick(int left, int right, Compare& com) {
    if (left < right) {
        T pivot = medianOfThree(_vec[left], _vec[(left + ((right - left) >> 1))], _vec[right], com);

        int i = left, j = right;
        while (i <= j) {
            while (com(_vec[i], pivot)) i++;
            while (com(pivot, _vec[j])) j--;
            if (i <= j) {
                swap(_vec[i], _vec[j]);
                i++;
                j--;
            }
        }

        if (left < j) quick(left, j, com);
        if (i < right) quick(i, right, com);
    }
}

template<class T, class Compare>
void MyQsort<T, Compare>::print() {
    for (const T& elem : _vec) {
        cout << elem << " ";
    }
    cout << endl;
}


int main() {

    int arr[10] = {1, 2, 4, 5, 8, 6, 3, 7, 10, 9} ;
    MyQsort<int, std::greater<int>> mqInt(arr, 10, std::greater<int>());
    mqInt.print();

}