/* PRESET CODE BEGIN - NEVER TOUCH CODE BELOW */

#include <iostream>
#include <cstdlib>

typedef struct node {
    int data;
    struct node *next;
} NODE;

NODE *find(NODE *, int *);

void outputring(NODE *);

void change(int, int, NODE *);

void outputring(NODE *pring) {
    NODE *p;
    p = pring;
    if (p == NULL)
        printf("NULL");
    else
        do {
            printf("%d", p->data);
            p = p->next;
        } while (p != pring);
    printf("\n");
    return;
}

int main() {
    int n, m;
    NODE *head, *pring;

    scanf("%d%d", &n, &m);
    head = (NODE *) malloc(sizeof(NODE));
    head->next = NULL;
    head->data = -1;

    change(n, m, head);
    pring = find(head, &n);
    printf("ring=%d\n", n);
    outputring(pring);

    return 0;
}

NODE * resultNode = nullptr;

bool process(int n,int m, NODE * head,NODE * tmp){
    NODE * p1 = tmp;
    NODE * p2 = head;
    bool found = false;
    while(p1->next){
        p1 = p1->next;
        p2 = p2->next;
        if(p1->data == n){
            found = true;
            break;
        }
    }
    NODE * tmpNode = p2;
    while(p2->next){
        p2 = p2->next;
    }
    if(found){
        p2->next = tmpNode;
        resultNode = tmpNode;
    }else{
        p1->next = new NODE;
        p1->next->data = n;
        p1->next->next = nullptr;
        p2->next = new NODE;
        p2->next->data = n / m;
        p2->next->next = nullptr;
    }
    return found;
}
void change(int n, int m, NODE *head) {
    bool isFound = false;
    NODE * tmp = new NODE;
    tmp -> next = nullptr;
    tmp -> data = -1;
    //建议delete掉，这里就不删了
    while(n!=0 && !isFound){
        n *= 10;
        isFound = process(n,m,head,tmp);
        n %= m;
    }
}

NODE *find(NODE *head, int *n) {
    int ans_n = 0;
    if(resultNode){
        for(NODE * p = resultNode->next; true ;p = p->next){
            ans_n++;
            if(p== resultNode){
                break;
            }
        }
    }
    *n = ans_n;
    return resultNode;
} 


/* PRESET CODE END - NEVER TOUCH CODE ABOVE */  