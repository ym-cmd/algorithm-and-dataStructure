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

int main() {
    vector<int> vec = {1,2,3,4,5,6,7};
    //vec.assign(10, 3);
    vec.assign({1,3,47,89});
    display(vec);

    return 0;
}