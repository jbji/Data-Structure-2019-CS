#include <iostream>

using namespace std;

int main(){
	int n;
	cin >> n;
	int * tree = new int[n+1];
	for(int i=1;i<=n;i++){
		cin >> tree[i];
	}
	int i,j;
	cin >> i >> j;
	if(tree[i] == 0){
		cout << "ERROR: T[" << i << "] is NULL" << endl;
		return 0;
	}else if(tree[j] ==0){
		cout << "ERROR: T[" << j << "] is NULL" << endl;
		return 0;
	}
	while(i!=j){
		if(i>j){
			i/=2;
		}else{
			j/=2;
		}
	}
	cout << i << " " << tree[i] << endl;
}