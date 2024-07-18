#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>

using std::cout;
using std::endl;
using std::ostream_iterator;
using std::copy;
using std::vector;

void print(vector<int>& vec) {
    for (auto x : vec) {
        cout << x << " ";
    }
}

void test()
{
    //遍历vector
    vector<int> vec = {1, 5, 7, 9, 3, 4};
    print(vec);
    // ostream_iterator<int> osi(cout, " ");
    // copy(vec.begin(), vec.end(), osi);

    //cout << endl;
    //operator<<
}

int main(int argc, char *argv[])
{
    test();
    return 0;
}