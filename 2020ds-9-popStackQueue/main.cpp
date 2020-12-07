#include <iostream>
#include <queue>
#include <stack>

using namespace std;

int main() {
    int n,m;
    for (cin >> n >> m; n != 0 || m != 0;) {
        for(;m>0;m--){
            queue<int> q;
            stack<int> s;
            int tmp;
            for(int i=0;i<n;i++){
                cin >> tmp;
                q.push(tmp);
            }
            for(int i=1;i<=n;i++){
                s.push(i);
                while(!s.empty() &&!q.empty() && s.top() == q.front()){
                    s.pop();
                    q.pop();
                }
            }
            if(s.empty() && q.empty()){
                cout << "Yes" <<endl;
            }else{
                cout << "No" << endl;
            }
        }
        cin >> n >> m;
        if( n != 0 || m != 0){
            cout << endl;
        }
    }

    return 0;
}