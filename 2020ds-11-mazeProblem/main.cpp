#include <iostream>
#include <stack>

using namespace std;

class Pos{
    int x;
    int y;
public:
    Pos(int x, int y) : x(x), y(y) {}

    int getX() const {
        return x;
    }

    int getY() const {
        return y;
    }

    friend ostream &operator<<(ostream &os, const Pos &pos) {
        os << "<" << pos.x+1 << "," << pos.y+1 << ">";
        return os;
    }
    Pos operator +(const Pos &pos){
        return Pos(x+pos.x,y+pos.y);
    }
    bool isValid(int dim,Pos itr,bool ** vis,bool ** maze){
        int new_x = x+itr.x;
        int new_y = y+itr.y;
        return new_x >= 0 && new_x < dim && new_y >= 0 && new_y < dim && !vis[new_x][new_y] && !maze[new_x][new_y];
    }
};

int main() {
    int n;
    cin >> n >> n;
    bool ** maze = new bool*[n];
    bool ** vis = new bool*[n];
    for(int i=0;i<n;i++){
        maze[i] = new bool[n];
        vis[i] = new bool[n];
        for(int j=0;j<n;j++){
            cin >> maze[i][j];
            vis[i][j] = false;
        }
    }
    Pos itr[4] = {{-1,0},{0,1},{1,0},{0,-1}};//逆序的下右上左
    stack<Pos> s;
    stack<Pos> result;
    s.push(Pos(0,0));
    vis[0][0] = true;
    while(!s.empty()){
        Pos p = s.top();
        vis[p.getX()][p.getY()] = true;
        if(p.getX() == n-1 && p.getY() == n-1){
            while(!s.empty()){
                Pos r = s.top();
                s.pop();
                if(vis[r.getX()][r.getY()]){
                    result.push(r);
                }
            }
            break;
        }
        int flag = true;
        for(auto i : itr){
            if(p.isValid(n,i,vis,maze)){
                s.push(p+i);
                flag = false;
            }
        }
        if(flag){
            s.pop();
//            vis[p.getX()][p.getY()] = false;
        }
    }
    if(result.empty()){
        cout << "There is no solution!" << endl;
    }else{
        while(!result.empty()){
            Pos r = result.top();
            result.pop();
            cout << r <<" ";
        }
        cout << endl;
    }

    return 0;
}