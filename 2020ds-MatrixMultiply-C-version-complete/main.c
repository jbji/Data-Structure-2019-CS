#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct _Triple{
    int i;
    int j;
    int e;
}Triple;

void TripleSetter(Triple * triple, int i, int j, int e){
    triple->i = i;
    triple->j = j;
    triple->e = e;
}

void TripleOutput(const Triple * triple){
    printf("%d,%d,%d\n",triple->i+1,triple->j+1,triple->e);
}

typedef struct _TripleMatrix{
    Triple * data;
    int data_size;
    int * row_pos;
    int mu;
    int nu;
    int tu;
}TripleMatrix;

void TripleMatrixConstructor(TripleMatrix * t,int mu,int nu,int tu){
    t->mu = mu; t->nu = nu; t->tu = tu;
    t ->data_size = tu;
    if(t->data_size == 0) t->data_size = 200;
    t->data = (Triple * )malloc(sizeof(Triple) * t->data_size);
    memset(t->data,0,sizeof(Triple)*t->data_size);
    t->row_pos = (int *)malloc(sizeof(int)*mu);
    for(int i=0;i<t->mu;i++){
        t->row_pos[i] = -1;
    }
}
void TripleMatrixDestructor(TripleMatrix * t){
    free(t->data);
    free(t->row_pos);
}
void TripleMatrixSetElement(TripleMatrix * t,int index,int i,int j,int e){
    if(index >=0 && index < t->tu){
        t->data[index].i = i;
        t->data[index].j = j;
        t->data[index].e = e;
    }
}
void TripleMatrixGenerate_Row_pos(TripleMatrix * t){
    for(int i=0;i<t->tu;i++){
        if(t->row_pos[t->data[i].i] == -1){
            t->row_pos[t->data[i].i] = i;
        }
    }
    for(int i=t->mu-1;i>=0;i--){
        if(t->row_pos[i] == -1){
            if(i == t->mu-1){
                t->row_pos[i] = t->tu;
            }else{
                t->row_pos[i] = t->row_pos[i+1];
            }
        }
    }
//    for(int i=0;i<t->mu-1;i++){
//        printf("%d ",t->row_pos[i]);
//    }
//    printf("\n");
}
TripleMatrix * TripleMatrixMultiply(const TripleMatrix * M, const TripleMatrix * N){
    //take the current matrix as the matrix m;
    //What we're going to calc is M(i,k) X N(k,j);
    if(M->nu != N->mu) return NULL;
    TripleMatrix * Q = (TripleMatrix *)malloc(sizeof(TripleMatrix));
    TripleMatrixConstructor(Q,M->mu,N->nu,0);
    int ctemp[N->nu];//每一行元素的累加器
    if(M->tu * N->tu == 0){ //如果Q是零矩阵，直接返回
        return Q;
    }
    for(int arow = 0; arow < M->mu; ++ arow){
        //处理M的每一行，亦即对应Q的每一行
        memset(ctemp,0, sizeof(int)*N->nu);//累加器清零
        Q->row_pos[arow] = Q->tu;
        int tp = arow < M->mu-1 ? M->row_pos[arow+1] : M->tu;//寻找当前行结尾
        for(int p = M->row_pos[arow] ; p < tp;p++){//对当前行中的每一个非零元
            int brow = M->data[p].j; //对应元在N中的行号
            int t = brow < N->mu-1 ? N->row_pos[brow+1] : N->tu; //寻找当前行结尾
            for(int q = N->row_pos[brow]; q < t; q++){
                int ccol = N->data[q].j;
                ctemp[ccol] += M->data[p].e * N->data[q].e;
            }
        }
        for(int ccol = 0; ccol < Q->nu ; ccol++){
            if(ctemp[ccol]){
                if(Q->tu + 1 > Q->data_size - 1){
                    realloc(Q->data,Q->data_size*2);
                }
                TripleMatrixSetElement(Q,Q->tu++,arow,ccol,ctemp[ccol]);
            }
        }
    }
    return Q;
}

void TripleMatrixOutput(TripleMatrix * t){
    printf("%d\n%d\n%d\n",t->mu,t->nu,t->tu);
    for(int i=0;i<t->tu;i++){
        TripleOutput(t->data+i);
    }
}

TripleMatrix *getMatrix();

int main() {
    TripleMatrix * m = getMatrix();
    TripleMatrix * n = getMatrix();
    TripleMatrix * ans = TripleMatrixMultiply(m,n);
    if(ans){
        TripleMatrixOutput(ans);
    }
    TripleMatrixDestructor(m);
    free(m);
    TripleMatrixDestructor(n);
    free(n);
    TripleMatrixDestructor(ans);
    free(ans);
    return 0;
}

TripleMatrix * getMatrix() {
    int mu,nu,tu;
    scanf("%d %d %d",&mu,&nu,&tu);
    TripleMatrix * m = (TripleMatrix *)malloc(sizeof(TripleMatrix));
    TripleMatrixConstructor(m,mu,nu,tu);
    int tmpi,tmpj,tmpe;
    for(int i=0;i<tu;i++){
        scanf("%d %d %d",&tmpi,&tmpj,&tmpe);
        TripleMatrixSetElement(m,i,tmpi-1,tmpj-1,tmpe);
    }
    TripleMatrixGenerate_Row_pos(m);
    return m;
}
