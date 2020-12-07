#include <iostream>
#include <stack>
#include <string>
#include <cmath>
using namespace std;


//operator set: + - * / % ^ ( )


class Opnd_Num{
    int data;
public:
    explicit Opnd_Num(int data = 0) : data(data) {}
    int getData() const {
        return data;
    }
    void setData(int data) {
        Opnd_Num::data = data;
    }
    void append(char digit){
        data = data*10 + digit -'0';
    }
    void inverse(){
        data = -data;
    }

    friend ostream &operator<<(ostream &os, const Opnd_Num &opnd) {
        os << opnd.data;
        return os;
    }
};
enum StatusType{
    ERROR,DIVIDE_ZERO,SUCCESSFUL
};

enum LastPushType{
    NUMBER,OPERATOR
};

bool debugMode = false;
int opOrder[9][9]={//行、列、行是before，列是current/now
        { 1, 1,-1,-1,-1,-1,-1, 1, 1},
        { 1, 1,-1,-1,-1,-1,-1, 1, 1},
        { 1, 1, 1, 1, 1,-1,-1, 1, 1},
        { 1, 1, 1, 1, 1,-1,-1, 1, 1},
        { 1, 1, 1, 1, 1,-1,-1, 1, 1},
        { 1, 1, 1, 1, 1,-1,-1, 1, 1},
        {-1,-1,-1,-1,-1,-1,-1, 0, 2},
        { 1, 1, 1, 1, 1, 1,-2, 1, 1},
        {-1,-1,-1,-1,-1,-1,-1,-2, 0}
};

StatusType expCalc(string exp, string &s);

inline bool isOperator(char c);
int cmpOperator(char before,char now);
StatusType processOp(stack<char> &op, stack<Opnd_Num> &opnd, StatusType &status);
inline int getOpId(char op);
Opnd_Num opnd_calc(Opnd_Num a, char op, Opnd_Num b, StatusType &status);

StatusType expCalc(string exp, string &s) {
    stack<char> op;
    stack<Opnd_Num> opnd;
    LastPushType flag = OPERATOR;
    int multiplier = 1;
    op.push('#');
    exp.append("#");
    //exp got;
    if(debugMode) cout <<"exp: "<< exp <<endl;
    for(auto p = exp.begin();p<exp.end();){
        if(isOperator(*p)){
            //operator got;
            if(debugMode) cout << *p <<endl;

            if(flag != NUMBER){
                if(*p == '-'){
                    multiplier = -1;
                    p++;
                    continue;
                }else if(*p!='('){
                    return ERROR;
                }
            }
            int switchFlag = cmpOperator(op.top(),*p);
            switch(switchFlag){
                case 1:{ //栈顶元素优先级更高，那么把栈顶的算完就好啦
                    StatusType status;
                    status = processOp(op, opnd, status);
                    switch (status){
                        case ERROR:
                            return ERROR;
                        case DIVIDE_ZERO:
                            return DIVIDE_ZERO;
                        default:
                            break;
                    }
                    continue;
                }
                case -1: //栈顶元素优先级低，新运算符的优先级更高
                    flag = OPERATOR;
                    op.push(*p);
                    break;
                case 0: //脱括号
                    op.pop();
                    break;
                default:
                    return ERROR;
            }
        }else{
            flag = NUMBER;
            Opnd_Num tmp(0);
            tmp.append(*p);
            while(p+1<exp.end() && !isOperator(*(p+1))){
                p++;
                tmp.append(*p);
            }
            if(multiplier == -1){ //to match numbers under the ZERO;
                tmp.inverse();
                multiplier = 1;
            }
            //number got;
            if(debugMode) cout << tmp << endl;
            opnd.push(tmp);
        }
        p++;
    }
    s.append(to_string(opnd.top().getData()));
    if(op.empty() && opnd.size() == 1){
        return SUCCESSFUL;
    }else{
        return ERROR;
    }
}

inline bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^' || c == '(' || c == ')' || c=='#';
}
inline int getOpId(char op){
    switch(op){
        case '+': return 0;
        case '-': return 1;
        case '*': return 2;
        case '/': return 3;
        case '%': return 4;
        case '^': return 5;
        case '(': return 6;
        case ')': return 7;
        case '#': return 8;
        default: return -1;
    }
}
int cmpOperator(char before,char now) {
    return opOrder[getOpId(before)][getOpId(now)];
}
StatusType processOp(stack<char> &op, stack<Opnd_Num> &opnd, StatusType &status) {
    Opnd_Num b = opnd.top();
    opnd.pop();
    Opnd_Num a = opnd.top();
    opnd.pop();
    opnd.push(opnd_calc(a, op.top(), b, status));
    op.pop();
    return status;
}
Opnd_Num opnd_calc(Opnd_Num a, char op, Opnd_Num b, StatusType &status) {
    status = SUCCESSFUL;
    switch (op) {
        case '+':
            return Opnd_Num(a.getData() + b.getData());
        case '-':
            return Opnd_Num(a.getData() - b.getData());
        case '*':
            return Opnd_Num(a.getData() * b.getData());
        case '/':{
            if(b.getData() != 0){
                return Opnd_Num(a.getData() / b.getData());
            }else{
                status = DIVIDE_ZERO;
                return Opnd_Num();
            }
        }
        case '%':
            return Opnd_Num(a.getData() % b.getData());
        case '^':{
            if(b.getData() >= 0){
                return Opnd_Num(pow(a.getData(), b.getData()));
            }else{
                status = ERROR;
                return Opnd_Num();
            }
        }
        default:
            status = ERROR;
            return Opnd_Num();
    }
}


int main() {
    int t;
//    debugMode = true;
    for(cin >> t;t>0;t--){
        string str;
        cin >> str;
        string result;
        switch(expCalc(str, result)){
            case ERROR:
                cout << "error." << endl;
                break;
            case DIVIDE_ZERO:
                cout << "Divide 0." << endl;
                break;
            case SUCCESSFUL:
                cout << result << endl;
                break;
        }
    }
    return 0;
}
