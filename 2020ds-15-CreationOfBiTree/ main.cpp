#include <iostream>
#include <string>
#include <queue>

using namespace std;

typedef char TElemType;

class BiTNode{
	TElemType data;
	BiTNode * lchild, * rchild; //pointer for l & r child.
	BiTNode * parent;
public:
	explicit BiTNode(TElemType _data = '#',BiTNode * _parent = nullptr, BiTNode * _lchild = nullptr, BiTNode * _rchild = nullptr)
		: parent(_parent),lchild(_lchild),rchild(_rchild),data(_data){}
	static void TreeCreation(BiTNode * root,string BiTStr);
	static void IndentPrint(BiTNode * root);
};


void BiTNode::TreeCreation(BiTNode * root, string BiTStr){
	queue<BiTNode *> q;
	q.push(root);
	for(auto s = BiTStr.begin();s<BiTStr.end();++s){
		BiTNode * tmp = q.front();
		q.pop();
		tmp->lchild = new BiTNode(*s,tmp);
		if(++s<BiTStr.end()){
			tmp->rchild = new BiTNode(*s,tmp);
		}
		q.push(tmp->lchild);
		q.push(tmp->rchild);
	}
}

int main(){
	BiTNode *root = new BiTNode();
	string tree_str;
	cin >> tree_str;
	BiTNode::TreeCreation(root,tree_str);
	cout << "Hello World" << endl;
}