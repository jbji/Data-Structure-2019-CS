#include <iostream>
#include <string>
#include <queue>
#define INDENT "    "

using namespace std;

typedef char TElemType;

class BiTNode{
    TElemType data;
    BiTNode * lchild, * rchild; //pointer for l & r child.
    BiTNode * parent;
public:
    explicit BiTNode(TElemType _data = '#',BiTNode * _parent = nullptr, BiTNode * _lchild = nullptr, BiTNode * _rchild = nullptr)
            : parent(_parent),lchild(_lchild),rchild(_rchild),data(_data){
    }
    static void TreeCreation(BiTNode * root,string BiTStr);
    static void IndentPrint(BiTNode * root);
    static void PrintSequence(BiTNode * node);
    static void Swap(BiTNode * root);
    static void PrintLeaves(BiTNode * root);
private:
    static string GetPreSequence(BiTNode * node);
    static string GetInSequence(BiTNode * node);
    static string GetPostSequence(BiTNode * node);
    static void RecursionSwap(BiTNode * root);
    static void RecursionPrint(BiTNode * root,int & depth);
    static void RecursionLeaves(BiTNode * node,int & ans);
};


void BiTNode::TreeCreation(BiTNode * root, string BiTStr){
    queue<BiTNode *> q;
    q.push(root);
    auto s = BiTStr.begin();
    root->data = *s;
    for(++s;s<BiTStr.end() && !q.empty();++s){
        BiTNode * tmp = q.front();
        q.pop();
        tmp->lchild = new BiTNode(*s,tmp);
        if(++s<BiTStr.end()){
            tmp->rchild = new BiTNode(*s,tmp);
        }
        q.push(tmp->lchild);
        q.push(tmp->rchild);
    }
    cout << "BiTree" << endl;
}
void BiTNode::IndentPrint(BiTNode * root){
    int depth = 0;
    RecursionPrint(root,depth);
}
void BiTNode::RecursionPrint(BiTNode *node,int & depth){
    if(node == nullptr) return;
    if(node->data != '#'){
        for(int i=0;i<depth;i++){
            cout << INDENT;
        }
        cout << node->data << endl;
    }
    RecursionPrint(node->lchild,++depth);
    depth--;
    RecursionPrint(node->rchild,++depth);
    depth--;
}

string BiTNode::GetPreSequence(BiTNode * node){
    string ans;
    if(node){
        if(node->data != '#'){
            ans += node->data;
        }
        ans += GetPreSequence(node->lchild);
        ans += GetPreSequence(node->rchild);
    }
    return ans;
}
string BiTNode::GetInSequence(BiTNode * node){
    string ans;
    if(node){
        ans += GetInSequence(node->lchild);
        if(node->data != '#'){
            ans += node->data;
        }
        ans += GetInSequence(node->rchild);
    }
    return ans;
}
string BiTNode::GetPostSequence(BiTNode * node){
    string ans;
    if(node){
        ans += GetPostSequence(node->lchild);
        ans += GetPostSequence(node->rchild);
        if(node->data != '#'){
            ans += node->data;
        }
    }
    return ans;
}

void BiTNode::PrintSequence(BiTNode *node) {
    cout << "pre_sequence  : " << BiTNode::GetPreSequence(node) << endl;
    cout << "in_sequence   : " << BiTNode::GetInSequence(node) << endl;
    cout << "post_sequence : " << BiTNode::GetPostSequence(node) << endl;
}
void BiTNode::Swap(BiTNode * root){
    RecursionSwap(root);
    cout << "BiTree swapped" << endl;
}

void BiTNode::RecursionSwap(BiTNode * root) {
    if(root == nullptr) return;
    RecursionSwap(root->lchild);
    RecursionSwap(root->rchild);
    BiTNode * tmp = root->lchild;
    root->lchild = root->rchild;
    root->rchild = tmp;
}

void BiTNode::PrintLeaves(BiTNode *root) {
    int ans = 0;
    RecursionLeaves(root,ans);
    cout << "Number of leaf: " << ans << endl;
}

void BiTNode::RecursionLeaves(BiTNode *node,int & ans) {
    if(!node) return;
    if(node->data != '#' && node->lchild == nullptr && node->rchild == nullptr){
        ans++;
    }
    if(node->data != '#'){
        if(node->lchild && node->rchild){
            if(node->lchild->data == '#' && node->rchild->data == '#'){
                ans++;
            }
        }
    }
    RecursionLeaves(node->lchild,ans);
    RecursionLeaves(node->rchild,ans);
}

int main(){
    BiTNode *root = new BiTNode();
    string tree_str;
    cin >> tree_str;
    BiTNode::TreeCreation(root,tree_str);
    BiTNode::IndentPrint(root);
    BiTNode::PrintSequence(root);
    BiTNode::PrintLeaves(root);
    BiTNode::Swap(root);
    BiTNode::IndentPrint(root);
    BiTNode::PrintSequence(root);
}