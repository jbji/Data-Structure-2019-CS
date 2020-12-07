#include <iostream>
#include <cstring>

using namespace std;

class BiTNode{
public:
    char data;
    int lchild;
    int rchild;
    BiTNode(char _data = '-',int _lchild = -3,int _rchild = -3)
            : data(_data),lchild(_lchild),rchild(_rchild){}
};

bool recursiveMark(BiTNode ** tree_a,BiTNode ** tree_b, bool * vis_a,bool * vis_b,
                   int ia,int ib){
    vis_a[ia] = true;
    int al = tree_a[ia]->lchild;
    int ar = tree_a[ia]->rchild;
    vis_b[ib] = true;
    int bl = tree_b[ib]->lchild;
    int br = tree_b[ib]->rchild;
    if(al== -3 && ar == -3 && bl == -3 && br == -3) return tree_a[ia]->data == tree_b[ib]->data;
    bool fa = false,fb = false,fc = false,fd = false;
    if(al!=-3 && bl!= -3 && tree_a[al]->data == tree_b[bl]->data) fa = recursiveMark(tree_a,tree_b,vis_a,vis_b,al,bl);
    if(ar!=-3 && br!= -3 && tree_a[ar]->data == tree_b[br]->data) fb = recursiveMark(tree_a,tree_b,vis_a,vis_b,ar,br);
    if(al!=-3 && br!= -3 && tree_a[al]->data == tree_b[br]->data) fc = recursiveMark(tree_a,tree_b,vis_a,vis_b,al,br);
    if(ar!=-3 && bl!= -3 && tree_a[ar]->data == tree_b[bl]->data) fd = recursiveMark(tree_a,tree_b,vis_a,vis_b,ar,bl);
    return fa || fb || fc || fd;
}

int main(){
    //getting input.
    char tmpdata,tmplc,tmprc;
    int dim_a;
    cin >> dim_a;
    //tree a
    BiTNode ** tree_a = new BiTNode *[dim_a];
    for(int i=0;i<dim_a;i++){
        cin >> tmpdata >> tmplc >> tmprc;
        tree_a[i] = new BiTNode(tmpdata,tmplc-'0',tmprc-'0'); // '-' - '0' == -3
    }
    int dim_b;
    cin >> dim_b;
    //tree b
    BiTNode ** tree_b = new BiTNode*[dim_b];
    for(int i=0;i<dim_b;i++){
        cin >> tmpdata >> tmplc >> tmprc;
        tree_b[i] = new BiTNode(tmpdata,tmplc-'0',tmprc-'0'); // '-' - '0' == -3
    }
    if(dim_a!=dim_b){
        cout << "No." <<endl;
        return 0;
    }
    bool vis_a[dim_a];
    bool vis_b[dim_b];
    memset(vis_a,0,sizeof(bool)*dim_a);
    memset(vis_b,0,sizeof(bool)*dim_b);
    for(int ia=0;ia<dim_a;ia++){
        if(vis_a[ia]) continue;
        vis_a[ia] = true;
        int ib = -1;
        char tmp = tree_a[ia]->data;
        for(int j=0;j<dim_b;j++){
            if(tree_b[j]->data == tree_a[ia]->data){
                ib = j;
                vis_b[j] = true;
                break;
            }
        }
        if(ib == -1){ //没找到与之匹配结点
            cout << "No." <<endl;
            return 0;
        }
        if(!recursiveMark(tree_a,tree_b,vis_a,vis_b,ia,ib)){;//开始匹配节点下的树结构
            cout << "No." <<endl;
            return 0;
        }
    }
    for(int i=0;i<dim_b;i++){
        if(!vis_b[i]){
            cout << "No." <<endl;
            return 0;
        }
    }
    cout << "Yes." <<endl;
    return 0;
}