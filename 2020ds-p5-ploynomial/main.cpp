#include <iostream>
#include <string>

using namespace std;


class PolyElem {
    int coefficient;
    int index;
public:
    explicit PolyElem(int coefficient = 0, int index = 0) : coefficient(coefficient), index(index) {}

    friend ostream &operator<<(ostream &os, const PolyElem &elem) {
        os << "<" << elem.coefficient << "," << elem.index << ">";
        return os;
    }

    int getCoefficient() const {
        return coefficient;
    }

    void setCoefficient(int coefficient) {
        PolyElem::coefficient = coefficient;
    }

    int getIndex() const {
        return index;
    }

    void setIndex(int index) {
        PolyElem::index = index;
    }
};

class Node {
    PolyElem data;
    Node *next;
public:
    //generators;
    //to generate a Node, you must provide your coefficient and the index.
    Node(int coefficient, int index) : data(PolyElem(coefficient, index)), next(nullptr) {}

    explicit Node(const PolyElem &data, Node *next = nullptr) : data(data), next(next) {}

    friend ostream &operator<<(ostream &os, const Node &node) {
        os << node.data;
        return os;
    }

    //setter and getter;
    const PolyElem &getData() const {
        return data;
    }

    void setData(const PolyElem &data) {
        Node::data = data;
    }

    Node *getNext() const {
        return next;
    }

    void setNext(Node *next) {
        Node::next = next;
    }
};

class OrderedList {
    Node *head;
public:
    OrderedList() : head(nullptr) {}

    friend ostream &operator<<(ostream &os, const OrderedList &list) {
        if(list.getHead() == nullptr){
            os << "<0,0>";
        }
        for (Node *p = list.getHead(); p; p = p->getNext()) {
            if (p->getNext()) {
                os << (*p) << ",";
            } else {
                os << (*p);
            }
        }
        return os;
    }

    Node *getHead() const {
        return head;
    }

    void insert(PolyElem e) {
        if (e.getCoefficient() == 0) {
            return;
        }
        if (head) {//如果有头节点
            Node *p = head;
            Node *pb = nullptr; //记录前驱
            while (p->getNext() && p->getData().getIndex() < e.getIndex()) {
                pb = p;
                p = p->getNext();
            }
            //p has no next,then p could be bigger than e
            //p has an equivalent or bigger index;
            if (p->getData().getIndex() < e.getIndex()) {//新的比旧的更大，直接在后面插入
                p->setNext(new Node(e));
            } else if (p->getData().getIndex() > e.getIndex()) {//新的比旧的指数更小，在前面插入
                if (pb) {//如果有前驱
                    pb->setNext(new Node(e));//前驱的后继是新节点
                    pb->getNext()->setNext(p);//新节点的后继是当前节点p
                } else {//如果没有前驱，说明就是头节点
                    head = new Node(e);//在头节点处插入新的节点
                    head->setNext(p);//新节点的后继是当前节点
                }
            } else {//指数相等
                //新的系数
                int newCoefficient = p->getData().getCoefficient() + e.getCoefficient();
                if (newCoefficient != 0) {//如果这个系数不是零
                    //直接改变当前节点内容
                    p->setData(PolyElem(newCoefficient, p->getData().getIndex()));
                } else {//系数是零
                    if (pb) {//如果有前驱，那么直接删掉当前节点
                        pb->setNext(p->getNext());
                        delete p;
                    } else {//没有前驱，即当前就是头节点，删掉就行了.
                        head = p->getNext();
                        delete p;
                    }
                }

            }
        } else {//没有头节点，就是空表
            head = new Node(e);
        }
    }

    void insideMerge(const OrderedList &list) {
        for (Node *p = list.getHead(); p; p = p->getNext()) {
            this->insert(p->getData());
        }
    }

    virtual ~OrderedList() {
        Node *p = head;
        Node *pb = nullptr; //记录前驱
        if(p){
            while (p->getNext()) {
                pb = p;
                p = p->getNext();
                delete pb;
            }
            delete p;
        }
    }
};

class Polynomial {
private:
    OrderedList *list;
public:
    Polynomial() {
        list = new OrderedList();
    }
    explicit Polynomial(int dim) : Polynomial(){
        int tmpCoefficient,tmpIndex;
        while(dim > 0){
            cin >> tmpCoefficient >> tmpIndex;
            insert(tmpCoefficient,tmpIndex);
            dim--;
        }
    }

    friend ostream &operator<<(ostream &os, const Polynomial &polynomial) {
        os << *(polynomial.list);
        return os;
    }

    OrderedList *getList() const {
        return list;
    }

    void insert(int coefficient, int index) {
        list->insert(PolyElem(coefficient, index));
    }

    void insert(PolyElem e) {
        list->insert(e);
    }

    void insideMerge(const Polynomial &p) {
        list->insideMerge(*(p.getList()));
    }

    virtual ~Polynomial() {
        delete list;
    }
};

int main() {
    ios::sync_with_stdio(false);
    int status = 0;
    int tmp_dim;
    while(true){
        cin >> status;
        if(status==0) break;

        cin >> tmp_dim;
        Polynomial a(tmp_dim);
        cout << a << endl;

        cin >> tmp_dim;
        Polynomial b(tmp_dim);
        cout << b << endl;

        cin >> tmp_dim;
        Polynomial c(tmp_dim);
        cout << c << endl;

        a.insideMerge(b);
        cout << a << endl;
        a.insideMerge(c);
        cout << a << endl;
        break;
    }
    return 0;
}