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
    Triple transpose(){
        return Triple(j,i,e);
    }
    int getI() const {
        return i;
    }
    int getJ() const {
        return j;
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
public:
    TripleMatrix(int mu,int nu, int tu):mu(mu),nu(nu),tu(tu){
        data = new Triple[tu];
        memset(data,0,sizeof(Triple)*tu);
        row_pos = new int[nu];
        memset(row_pos,0,sizeof(int)*nu);
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
    TripleMatrix fastTranspose(){
        TripleMatrix result = TripleMatrix(nu,mu,tu);
        if(tu){
            int num[nu],cpot[nu];
            memset(num,0,sizeof(int)*nu);
            memset(cpot,0,sizeof(int)*nu);
            for(int t=0; t < tu; t++){
                num[data[t].getJ()]++; //求M中每一列非零值的个数
            }
            cpot[0] = 0;
            for(int col = 1;col < nu ; col++){ //指某一列的元素下一次应当放置到哪一个位置，即col列中第一个非零元在data中的序号
                cpot[col] = cpot[col-1] + num[col-1];
            }
            //输出
            cout << "num:";
            for(int i=0;i<nu;i++){
                cout << num[i] << ",";
            }
            cout << endl;
            cout << "cpot:";
            for(int i=0;i<nu;i++){
                cout << cpot[i] + 1 << ",";
            }
            cout << endl;
            for(int t = 0;t<tu ;t++){
                int col = data[t].getJ();
                result.data[cpot[col]] = data[t].transpose();
                cpot[col]++;
            }
        }

        return result;
    }
    friend ostream &operator<<(ostream &os, const TripleMatrix &matrix) {
        for(int i=0;i<matrix.tu;i++){
            os << matrix.data[i] << endl;
        }
        return os;
    }
};

int main() {
    debugMode = false;
//    freopen("1.in","r",stdin);
//    freopen("my1.out","w",stdout);
    int mu,nu,tu;
    cin >> mu >> nu >> tu;
    TripleMatrix m(mu,nu,tu);
    int tmpi,tmpj,tmpe;
    for(int i=0;i<tu;i++){
        cin >> tmpi >> tmpj >> tmpe;
        m.setElement(new Triple(tmpi-1,tmpj-1,tmpe),i);
    }
    if(debugMode) cout << m;
    TripleMatrix m_t = m.fastTranspose();
    cout << m_t;
    return 0;
}