#include <iostream>

template <class TElemType>
class BiTNode{
    TElemType data;
    BiTNode * lChild,rChild;
    BiTNode * parent;
};
int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}