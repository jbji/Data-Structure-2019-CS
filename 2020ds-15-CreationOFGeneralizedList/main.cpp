#include <iostream>
#include <string>

using namespace std;

bool debugMode = true;

enum ElemTag{
    ATOM,LIST
};

//template <class AtomType> //AtomType模板类, string suggested

typedef string AtomType;

class GLNode{ //Generalized List Node
    friend class GeneralizedList;
    ElemTag tag;
    union{//匿名union,直接访问union内部成员名称作为成员即可
        AtomType * atom = nullptr; // for atom
        struct{//匿名类,all members are public.
            GLNode * hp = nullptr; // head pointer & (non-static member in-class initialization: C++11 Feature)
            GLNode * tp = nullptr; // tail pointer
        } ptr;// pointers for lists
    };

public:
    explicit GLNode(ElemTag _tag = ATOM):tag(_tag){
        if(_tag == ATOM){
            atom = nullptr;
        }else if(_tag == LIST){
            ptr.hp = nullptr;
            ptr.tp = nullptr;
        }
    }
    explicit GLNode(const string& s){ // To Create a list
        if(s.empty()){
            new(this) GLNode(LIST);
        }else{
            if(s.length() == 1){
                tag = ATOM;
                atom = new AtomType(s);
            }else {
                tag = LIST;
                ptr.hp = nullptr;
                ptr.tp = nullptr;
                GLNode *p = this;
                string sub = s.substr(1, s.length() - 2);//脱括号
                string hsub;
                do {
                    severStr(sub, hsub);
                    if(!hsub.empty()) p->ptr.hp = new GLNode(hsub); //表头hsub如果有括号，构造函数会为其完成脱括号操作。
                    if (!sub.empty()) {
                        p->ptr.tp = new GLNode(LIST);
                        p = p->ptr.tp;
                    }
                } while (!sub.empty());
            }
        }
    }
    ~GLNode(){
        if(tag == ATOM){
            delete atom;
        }else if(tag == LIST){
            delete ptr.hp;
            delete ptr.tp;
        }
    }
    ElemTag getTag() const{
        return tag;
    };
    static void severStr(string & str, string & hstr){
        int leftBrackets = 0;
        string::iterator itr;
        for(itr = str.begin(); itr < str.end() && (*itr != ',' || leftBrackets!=0); ++itr){
            if(*itr == '('){
                ++leftBrackets;
            }else if(*itr == ')'){
                --leftBrackets;
            }
        }
        if(itr < str.end()){ //found to split
            int index = distance(str.begin(),itr);
            hstr = str.substr(0,index);
            str = str.substr(index + 1,str.length()-index-1);
        }else{ // only head found
            hstr = str;
            str.clear();
        }
    };
    int getDepth(){
        switch(tag){
            case ATOM:
                return 0;
            case LIST:{
                if(ptr.hp == nullptr){
                    return 1;
                }else{
                    int max = 0;
                    for(GLNode * p = this; p != nullptr ;p = p->ptr.tp){
                        int dep = p->ptr.hp->getDepth();
                        if(dep > max) max = dep;
                    }
                    return max+1;
                }
            }
        }
    }
    bool empty(){
        return ptr.hp == nullptr && ptr.tp == nullptr;
    }
    friend ostream &operator<<(ostream &os, const GLNode &node) {
        if(node.tag == ATOM){
            os << *node.atom;
        }else if(node.tag == LIST && node.ptr.hp){
            if(node.ptr.hp->tag == LIST){
                os << "(";
                os << *node.ptr.hp;
                os << ")";
                if(node.ptr.tp){
                    os << ",";
                    os << *node.ptr.tp;
                }
            }else{
                os << *node.ptr.hp;
                if(node.ptr.tp){
                    os << ",";
                    os << *node.ptr.tp;
                }
            }
        }
        return os;
    }
};

class GeneralizedList{
    GLNode * head;
public:
    explicit GeneralizedList(const string& s){
        head = new GLNode(s);
        print();
    }
    ~GeneralizedList(){
        delete head;
    }
    bool empty(){
        return head == nullptr || head->getTag() == ATOM || (head->getTag() == LIST && head->ptr.hp == nullptr);
    }
    void manipulate(int mode){
        //1 for take head, when you need to destroy the tail.
        //2 for take the tail, where you need to free head node.
        GLNode * tmp = head;
        if(mode == 1){
            head = tmp->ptr.hp;
            tmp->ptr.hp = nullptr;
            delete tmp;
            cout << "destroy tail" << endl <<"free list node" <<endl;
        }else if(mode == 2){
            GLNode * tmp = head;
            head = tmp->ptr.tp;
            tmp->ptr.tp = nullptr;
            delete tmp;
            cout << "free head node" << endl <<"free list node" <<endl;
        }
        print();
    }
    void print(){
        cout << "generic list: "<< *this << endl;
    }
    friend ostream &operator<<(ostream &os, const GeneralizedList &list) {
        if(list.head){
            if(list.head->getTag() == ATOM){
                os << *list.head;
            }else{
                os <<"(";
                os << *list.head;
                os << ")";
            }
        }else{
            os <<"(";
            os << ")";
        }
        return os;
    }
};


int main() {
    string glstr;
    cin >> glstr;
    GeneralizedList gg(glstr);
    //1 for take the head, when you need to destroy the tail.
    //2 for take the tail, where you need to free head node.
    for(int i;!gg.empty();){
        cin >> i;
        gg.manipulate(i);
    }
    return 0;
}
