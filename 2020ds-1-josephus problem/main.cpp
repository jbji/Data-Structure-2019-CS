#include <iostream>
#include <vector>

using namespace std;

vector<int> ans;

class Node{
public:
    int data;
    Node * next;
    Node * before;
    explicit Node(int _data = 0):data(_data),next(nullptr),before(nullptr){}
};

class List{
public:
    Node * head;
    Node * tail;
    bool isConnected;
    List():isConnected(false),head(nullptr),tail(nullptr){}
    void append(int ele){
        if(head){
            Node * p  = new Node(ele);
            tail -> next = p;
            p -> before = tail;
            tail = tail->next;
        }else{
            head = new Node(ele);
            tail = head;
        }
    }
    void josephus(int k,int m){
        //connect
        head->before = tail;
        tail->next = head;
        Node * p = head;
        for (int i = 1; i < k; i++) {
            p = p->next;
        }

        //start
        while(p){
            for (int i = 1; i < m; i++) {
                p = p->next;
            }
            Node * before = p->before;
            Node * next = p->next;
            ans.push_back(p->data);
            //to delete
            if(before == next){
                if(before == p){
                    delete p;
                    p = nullptr;
                }else{
                    next->before = next;
                    next->next = next;
                    delete p;
                    p = next;
                }
            }else{
                before -> next = next;
                next -> before = before;
                delete p;
                p = next;
            }
        }
    }
};

bool inputValid(int n, int k, int m);

void printAns();



int main() {
    int n,k,m;
//    cin >> n >> k >> m;
    scanf("%d,%d,%d",&n,&k,&m);
    if(!inputValid(n, k, m)) return 0;
    List l;
    for(int i=1;i<=n;i++){
        l.append(i);
    }
    l.josephus(k,m);
    printAns();
    return 0;
}

void printAns() {
    int i = 0;
    for(auto it=ans.begin();it<ans.end();it++){
        cout << *it;
        if (i != 9 && it != ans.end()-1) {
            cout << " ";
            i++;
        } else {
            i = 0;
            cout << endl;
        }
    }
}

bool inputValid(int n, int k, int m) {
    if(n<1 || k<1 || m<1){
        cout << "n,m,k must bigger than 0." << endl;
        return false;
    }
    if(k>n){
        cout << "k should not bigger than n." << endl;
        return false;
    }
    return true;
}
