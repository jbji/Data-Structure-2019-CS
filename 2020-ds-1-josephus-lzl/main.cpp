#include <iostream>

void josephus(long n, long k, long m);

void print(long n, int num, int ans);

int getNextT(long n, int t);

int getPriorT(long n, int t);

using namespace std;

int main() {
    long int n, k, m;
    scanf("%ld,%ld,%ld",&n,&k,&m);
    if (n < 1 || k < 1 || m < 1) printf("n,m,k must bigger than 0.\n");
    else if (k > n) printf("k should not bigger than n.\n");
    else {
        josephus(n, k, m);
    }
}

void josephus(long n, long k, long m) {
    int *vis = new int[n+1];
//    bool vis[10];
    for (int i = 0; i <= n; i++) vis[i] = false;
    int remaining_num = n;
    int t = k;
    while (remaining_num > 0) {
        for(int non_vis_cnt = 0; non_vis_cnt < m; t = getNextT(n, t)){
            if(!vis[t]){
                non_vis_cnt++;
            }
        }
        vis[getPriorT(n, t)] = true;
        print(n, --remaining_num, getPriorT(n,t));
    }
}

int getPriorT(long n, int t) {
    if (t == 1) {
        return n;
    } else {
        return --t;
    }
}

int getNextT(long n, int t) { return t % n + 1; }

void print(long n, int num, int ans) {
    if (num == 0 || (n - num) % 10 == 0) printf("%ld\n", ans);
    else printf("%ld ", ans);
}
