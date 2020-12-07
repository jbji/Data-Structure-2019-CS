#include <iostream>
#include <cstring>

bool debugMode = false;


struct Triple{
    int i;
    int j;
    int e;
};

void TripleConstructor(Triple * t, int i = ,int j, int e){

}



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

TripleMatrix * getMatrix() {
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
