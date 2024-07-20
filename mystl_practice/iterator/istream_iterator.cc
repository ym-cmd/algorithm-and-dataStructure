#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>


using namespace std;

int main() {
    istream_iterator<int> isi(cin);
    vector<int> vec;
    copy(isi, istream_iterator<int>(), vec.begin());

    copy(vec.begin(), vec.end(), ostream_iterator<int>(cout, " "));
    cout << endl;


    return 0;
}