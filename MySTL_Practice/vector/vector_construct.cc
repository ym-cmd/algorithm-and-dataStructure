#include <iostream>
#include <vector>

using std::cout; using std::endl;
using std::vector;

int main() {
    vector<int> vec;
    vector<int> vec1 = {1,2,3};
    vector<int> vec2 = vec1;
    vector<int> vec3(vec1.begin(), vec1.end());

    int arr[10] = {1,3,4,5,6};
    vector<int> vec4(arr, arr + 8);

    return 0;
}