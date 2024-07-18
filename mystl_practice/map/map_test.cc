/**
 * @file map_test.cc
 * @author Ym_cmd (yim45ad@gmail.com)
 * @brief STL源码剖析 p242
 * @version 0.1
 * @date 2024-07-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <map>
#include <iostream>
#include <string>
using namespace std;

int main() {
    map<string, int> simap;
    simap[string("jjjou")] = 1;
    simap[string("jerry")] = 2;
    simap[string("jason")] = 3;
    simap[string("jimmy")] = 4;

    return 0;
}