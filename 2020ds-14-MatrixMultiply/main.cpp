#include <iostream>
#include <cstring>

using namespace std;

bool debugMode = false;

class Triple{
    int i; //index i
    int j; //index j
    int e; // element
public:
    explicit Triple(int i = -1, int j = -1, int e = 0) : i(i), j(j), e(e) {}
    Triple transpose(){return Triple(j,i,e);}
    int getI() const {return i;}
    int getJ() const {return j;}
    int getE() const { return e;}
    void setE(int _e){e=_e;}
    void replace(int _i,int _j,int _e){
        i = _i; j = _j; e=_e;
    }
    friend ostream &operator<<(ostream &os, const Triple &triple) {
        os  << triple.i + 1 << "," << triple.j + 1 << "," << triple.e;
        return os;
    }
};

class TripleMatrix{
    Triple * data;
    int * row_pos;
    int mu;
    int nu;
    int tu;
    int data_size;
public:
    TripleMatrix(int mu,int nu, int tu):mu(mu),nu(nu),tu(tu){
        data_size = tu;
        if(data_size == 0) data_size = 200;
        data = new Triple[data_size];
        memset(data,0,sizeof(Triple)*data_size);
        row_pos = new int[mu];
        for(int i=0;i<mu;i++){
            row_pos[i] = -1;
        }
    }
    ~ TripleMatrix(){
        delete[] data;
        delete[] row_pos;
    }
    void setElement(Triple * element, int index){
        if(index >=0 && index < tu){
            if(debugMode) cout << *element << endl;
            data[index] = *element;
            if(debugMode) cout << data[index] << endl;
        }
    }
    void generate_row_pos(){ //这里有巨大bug
        for(int i=0;i<tu;i++){
            if(row_pos[data[i].getI()] == -1){
                row_pos[data[i].getI()] = i;
            }
        }
    }
    TripleMatrix * multiply(const TripleMatrix * N){
        //take the current matrix as the matrix m;
        //What we're going to calc is M(i,k) X N(k,j);
        TripleMatrix * M = this;
        if(M->nu != N->mu) return nullptr;
        auto * Q = new TripleMatrix(M->mu,N->nu,0);
        int ctemp[N->nu];//每一行元素的累加器
        if(M->tu * N->tu == 0){ //如果Q是零矩阵，直接返回
            return Q;
        }
        for(int arow = 0; arow < M->mu; ++ arow){
            //处理M的每一行，亦即对应Q的每一行
            memset(ctemp,0, sizeof(int)*N->nu);//累加器清零
            Q->row_pos[arow] = Q->tu;
            int tp = arow < M->mu-1 ? M->row_pos[arow+1] : M->tu;//寻找当前行结尾
            for(int p = M->row_pos[arow] ; p < tp && p != -1;p++){//对当前行中的每一个非零元
                int brow = M->data[p].getJ(); //对应元在N中的行号
                int t = brow < N->mu-1 ? N->row_pos[brow+1] : N->tu; //寻找当前行结尾
                for(int q = N->row_pos[brow]; q < t && q!=-1; q++){
                    int ccol = N->data[q].getJ();
                    ctemp[ccol] += M->data[p].getE() * N->data[q].getE();
                }
            }
            for(int ccol = 0; ccol < Q->nu ; ccol++){
                if(ctemp[ccol]){
                    if(Q->tu + 1 > data_size - 1){
                        realloc(Q->data,data_size*2);
                    }
                    Q->data[Q->tu++].replace(arow,ccol,ctemp[ccol]);
                }
            }
        }
        return Q;
    }
    void printBasics(){
        cout << mu << endl;
        cout << nu << endl;
        cout << tu << endl;
    }
    friend ostream &operator<<(ostream &os, const TripleMatrix &matrix) {
        for(int i=0;i<matrix.tu;i++){
            os << matrix.data[i] << endl;
        }
        return os;
    }
};

TripleMatrix *getMatrix();

int main() {
    debugMode = false;
//    freopen("1.in","r",stdin);
//    freopen("my1.out","w",stdout);
    TripleMatrix * m = getMatrix();
    TripleMatrix * n = getMatrix();
    TripleMatrix * ans = m->multiply(n);
    if(ans){
        ans->printBasics();
        cout << *ans;
    }
    delete m;
    delete n;
    delete ans;
    return 0;
}

TripleMatrix *getMatrix() {
    int mu,nu,tu;
    cin >> mu >> nu >> tu;
    auto * m = new TripleMatrix(mu,nu,tu);
    int tmpi,tmpj,tmpe;
    for(int i=0;i<tu;i++){
        cin >> tmpi >> tmpj >> tmpe;
        m->setElement(new Triple(tmpi-1,tmpj-1,tmpe),i);
    }
    m->generate_row_pos();
    return m;
}
