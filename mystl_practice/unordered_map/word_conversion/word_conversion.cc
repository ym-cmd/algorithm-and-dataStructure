#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

const string& transform(const string& s, const unordered_map<string, string>& mp) {
    auto it = mp.find(s);
    if (it != mp.end()) {
        return it->second;
    }
    return s;
}

unordered_map<string, string> init_map(ifstream& map_file) {
    unordered_map<string, string> ret;
    string text;
    string key;
    string val;
    while (map_file >> key && getline(map_file, text)) {
        ret[key] = text.substr(1);
    }
    return ret;
}

void word_transform(ifstream& map_file, ifstream& input) {
    auto trans_map = init_map(map_file);
    string text;
    while (getline(input, text)) {
        istringstream stream(text);
        string word;
        bool firstword = true;
        while (stream >> word) {
            if (firstword) firstword = false;
            else cout << " ";
            
            cout << transform(word, trans_map);
        }
        cout << endl;
    }
}

int main(int argc, char** argv) {
    ifstream map_file(argv[1]), input(argv[2]);
    if (map_file && input) word_transform(map_file, input);

    return 0;
}