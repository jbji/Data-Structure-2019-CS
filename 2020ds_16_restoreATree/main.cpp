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
    static BiTNode * RestoreTree(string inseq,string postseq);
    static void PrintLayer(BiTNode * root);
private:
    static BiTNode * RecursionRestoreTree(const string& inseq,const string& postseq,BiTNode * parent);
};

BiTNode * BiTNode::RestoreTree(string inseq,string postseq){
	BiTNode * root = new BiTNode(*(postseq.end()-1));
	int sp = inseq.find(*(postseq.end()-1));
	string lin = inseq.substr(0,sp);
	string lpost = postseq.substr(0,sp);
	string rin = inseq.substr(sp+1);
	string rpost = postseq.substr(sp,postseq.length()-sp-1);
	root->lchild = RecursionRestoreTree(lin,lpost,root);
	root->rchild = RecursionRestoreTree(rin,rpost,root);
	return root;
}
BiTNode * BiTNode::RecursionRestoreTree(const string& inseq,const string& postseq,BiTNode * parent){
	if(postseq.end()-1 >= postseq.begin()){
		BiTNode * result = new BiTNode(*(postseq.end()-1),parent);
		int sp = inseq.find(*(postseq.end()-1));
		string lin = inseq.substr(0,sp);
		string lpost = postseq.substr(0,sp);
		string rin = inseq.substr(sp+1);
		string rpost = postseq.substr(sp,postseq.length()-sp-1);
		switch (lin.length()){
			case 0:
				result->lchild = nullptr;
				break;
			case 1:
				result->lchild = new BiTNode(*lin.begin(),result);
				break;
			default:
				result->lchild = RecursionRestoreTree(lin,lpost,result);
				break;
		}
		switch (rin.length()){
			case 0:
				result->rchild = nullptr;
				break;
			case 1:
				result->rchild = new BiTNode(*rin.begin(),result);
				break;
			default:
				result->rchild = RecursionRestoreTree(rin,rpost,result);
				break;
		}
		return result;
	}
	return nullptr;
}

void BiTNode::PrintLayer(BiTNode * root){
	queue<BiTNode *> q;
	q.push(root);
	while(!q.empty()){
		BiTNode * tmp = q.front();
		q.pop();
		if(tmp->data != '#'){
			cout << tmp->data;
		}
		if(tmp->lchild) q.push(tmp->lchild);
		if(tmp->rchild) q.push(tmp->rchild);
	}
	cout << endl;
}

int main(){
    string in_str,post_str;
    cin >> in_str >> post_str;
    BiTNode *root = BiTNode::RestoreTree(in_str,post_str);
    BiTNode::PrintLayer(root);
}