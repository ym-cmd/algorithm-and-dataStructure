#include <iostream>
#include <map>
#include <utility>
#include <string>

using std::cout; using std::endl;
using std::map;
using std::pair;
using std::string;

int main() {
    map<int, string> mp;
    cout << mp.max_size() << endl;
}