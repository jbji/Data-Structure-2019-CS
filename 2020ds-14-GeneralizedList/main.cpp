#include <iostream>
#include <vector>

using namespace std;

//class Atom{
//
//};
//class GeneralizedList{
//    vector<void *> data;
//};

enum ElemTag{
    ATOM,LIST
};

class GLNode{
    ElemTag tag;
    union{

    };
};


int main() {
//    vector<void *> data;
//    data.push_back(new GeneralizedList());
//    data.push_back(new Atom());

    std::cout << "Hello, World!" << std::endl;
    return 0;
}