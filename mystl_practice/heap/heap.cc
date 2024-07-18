#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

template <typename T>
void display(vector<T>& vec) {
    for (auto& elem : vec) {
        cout << elem << " ";
    }
    cout << '\n';
}

int main() {
    int ia[9] = {0, 1, 2, 3, 4, 8, 9, 3, 5};
    vector<int> ivec(ia, ia + 9);

    make_heap(ivec.begin(), ivec.end());
    display(ivec);

    ivec.push_back(7);
    display(ivec);
    push_heap(ivec.begin(), ivec.end());
    display(ivec);

    pop_heap(ivec.begin(), ivec.end());
    display(ivec);
    ivec.pop_back();
    display(ivec);

    sort_heap(ivec.begin(), ivec.end());
    display(ivec);

}