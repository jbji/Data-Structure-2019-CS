#include <iostream>
#include <string>
#include <sstream>
using namespace std;

bool isNum(const string & str){
    stringstream sin(str);
    double d;
    char c;
    if(!(sin >> d)) return false;
    if(sin >> c) return false;
    return true;
}

int main() {
    string s = "test";
    cout << isNum(s) << endl;
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
