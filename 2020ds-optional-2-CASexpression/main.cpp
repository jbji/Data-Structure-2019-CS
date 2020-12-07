#include <iostream>
#include <stack>
#include <string>
#include <cmath>
#include <map>
#include <sstream>

using namespace std;


//operator set: + - * / % ^ ( )
bool isNum(const string & str);

class Opnd_Num{
    string data;
public:
    explicit Opnd_Num(string data = "") : data(data) {}
    const string &getData() const {
        return data;
    }
    void setData(const string &data) {
        Opnd_Num::data = data;
    }

    void append(char digit){
        data.append(1,digit);
//        data = data*10 + digit -'0';
    }
    void inverse(){
        if(isNum(data)){
            data = to_string(-stoi(data));
        }else{
            data.insert(0,1,'-');
        }
    }

    friend ostream &operator<<(ostream &os, const Opnd_Num &opnd) {
        os << opnd.data;
        return os;
    }
};
enum StatusType{
    ERROR,DIVIDE_ZERO,SLIENT_SUCCESSFUL,SUCCESSFUL
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
map<string,int> notation_mapper;

StatusType expCalc(string exp, string &s);

inline bool isOperator(char c);
int cmpOperator(char before,char now);
StatusType processOp(stack<char> &op, stack<Opnd_Num> &opnd, StatusType &status);
inline int getOpId(char op);
Opnd_Num opnd_calc(const Opnd_Num& a, char op, const Opnd_Num& b, StatusType &status);

StatusType parseAndClac(const string& exp, string &result);

StatusType expCalc(string exp, string &s) {
    stack<char> op;
    stack<Opnd_Num> opnd;
    LastPushType flag = OPERATOR;
    int multiplier = 1;
    op.push('#');
    exp.append("#");
    //exp got;
    if(debugMode) cout <<"expCalc::exp: "<< exp <<endl;
    for(auto p = exp.begin();p<exp.end();){
        if(isOperator(*p)){
            //operator got;
            if(debugMode) cout <<"expCalc::"<< *p <<endl;

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
            Opnd_Num tmp;
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
            if(debugMode) cout <<"expCalc::"<< tmp << endl;
            opnd.push(tmp);
        }
        p++;
    }
    s.append(opnd.top().getData());
    if(debugMode) cout << "expCalc::AfterCalc s: " << s << endl;
    if(op.empty() && opnd.size() == 1){
        return SLIENT_SUCCESSFUL;
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
Opnd_Num opnd_calc(const Opnd_Num& a, char op, const Opnd_Num& b, StatusType &status) {
    status = SLIENT_SUCCESSFUL;
    int ia,ib; //integer a, integer b
    if(debugMode) cout << "opnd_calc::a data: " << a.getData() << ",b data: "<< b.getData() <<endl;
    if(!isNum(a.getData())){
        if(notation_mapper.count(a.getData())){
            ia = notation_mapper[a.getData()];
        }else{
            status = ERROR;
            return Opnd_Num();
        }
    }else{
        ia = stoi(a.getData());
    }
    if(!isNum(b.getData())){
        if(notation_mapper.count(b.getData())){
            ib = notation_mapper[b.getData()];
        }else{
            status = ERROR;
            return Opnd_Num();
        }
    }else{
        ib = stoi(b.getData());
    }
    if(debugMode) cout << "opnd_calc::ia: " << ia << ",ib: "<<ib<<endl;
    switch (op) {
        case '+':
            return Opnd_Num(to_string(ia+ib));
        case '-':
            return Opnd_Num(to_string(ia-ib));
        case '*':
            return Opnd_Num(to_string(ia*ib));
        case '/':{
            if(ib != 0){
                return Opnd_Num(to_string(ia/ib));
            }else{
                status = DIVIDE_ZERO;
                return Opnd_Num();
            }
        }
        case '%':
            return Opnd_Num(to_string(ia%ib));
        case '^':{
            if(ib >= 0){
                return Opnd_Num(to_string((int)pow(ia,ib)));
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

bool isNum(const string & str){
    stringstream sin(str);
    double d;
    char c;
    if(!(sin >> d)) return false;
    if(sin >> c) return false;
    return true;
}


int main() {
//    debugMode = true;
//    freopen("6.in","r",stdin);
//    freopen("my6.out","w",stdout);
    while(true){
        string str;
//        cin >> str;
        getline(cin,str);
        if(str == "end") return 0;
        string result;
        switch(parseAndClac(str, result)){
            case ERROR:
                cout << "error." << endl;
                break;
            case DIVIDE_ZERO:
                cout << "Divide 0." << endl;
                break;
            case SLIENT_SUCCESSFUL:
                break;
            case SUCCESSFUL:
                cout << result << endl;
                break;
        }
    }
}

StatusType parseAndClac(const string& exp, string &result) {
    StatusType status = ERROR;
    string::size_type found = exp.find('=');
    if(found!=string::npos){
        string var = exp.substr(0,found);
        string real_exp = exp.substr(found+1);
        if(debugMode) cout <<"parseAndClac::"<< var << endl;
        if(debugMode) cout << "parseAndClac::"<< real_exp << endl;
        status = expCalc(real_exp,result);
        if(debugMode) cout <<"parseAndClac::"<< "status: " << status << ",result: " << result << endl;
        if(status == SLIENT_SUCCESSFUL){
            notation_mapper[var] = stoi(result);
        }
    }
    found = exp.find("? ");
    if(debugMode) cout <<"parseAndClac::found?"<< found << endl;
    if(found!=string::npos){
        string var = exp.substr(found+2);
        if(debugMode) cout <<"parseAndClac::varname?"<< var << endl;
        if(notation_mapper.count(var)){
            status = SUCCESSFUL;
            result.append(var).append("=").append(to_string(notation_mapper[var]));
        }
    }
    if(debugMode) cout <<"parseAndClac::"<< "afterParse: status:" << status << endl;
    return status;
}
