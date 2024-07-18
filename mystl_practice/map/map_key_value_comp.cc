#include <iostream>
#include <map>
#include <utility>
#include <string>

using std::cout; using std::endl;
using std::map;
using std::pair;
using std::string;

void key_comp_test(const map<int, string>& mp) {
    auto comp = mp.value_comp();
    pair<int, string> pair1 = std::make_pair(2, "two");
    pair<int, string> pair2 = std::make_pair(3, "three");

    if (comp(pair1, pair2)) {
        cout << pair1.first << " < " << pair2.first << endl;
    } else {
        cout << pair1.first << " > " << pair2.first << endl;
    }
}

void value_comp_test() {

}


int main() {
    map<int, string> mp;
    mp[1] = "one";
    mp[2] = "two";
    mp[3] = "three";

    key_comp_test(mp);
}