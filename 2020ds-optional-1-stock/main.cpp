#include <iostream>
#include <string>
#include <deque>
#include <utility>
#include <iomanip>
#include <sstream>

using namespace std;

int g_order_id = 1;

bool debugMode = false;

class Quest{
    int _order_id;
    string _stock_id;
    double price;
    int amount;
    char type;
public:
    const string &getStockId() const {
        return _stock_id;
    }
    void setStockId(const string &stockId) {
        _stock_id = stockId;
    }
    double getPrice() const {
        return price;
    }
    void setPrice(double price) {
        Quest::price = price;
    }
    int getAmount() const {
        return amount;
    }
    void setAmount(int amount) {
        Quest::amount = amount;
    }
    char getType() const {
        return type;
    }
    void setType(char type) {
        Quest::type = type;
    }
    int getOrderId() const {
        return _order_id;
    }

    Quest(string stockId, double price, int amount, char type)
        : _stock_id(std::move(stockId)), price(price),amount(amount), type(type) {
        _order_id = g_order_id++;
    }
    static bool cmp(const Quest & i,const Quest & j){
        return i.price > j.price;
    }
    friend ostream &operator<<(ostream &os, const Quest &quest) {
        os << "orderid: " << setw(4)<<setfill('0')<< quest._order_id << ", stockid:" << quest._stock_id << ", price: " << setw(6) << fixed << setprecision(1) << setfill(' ')<< quest.price
           << ", quantity: " << setw(4) << quest.amount << ", b/s: " << quest.type;
        return os;
    }
};

class Node{
public:
    Quest data;
    Node * next;
    explicit Node(Quest q):data(std::move(q)),next(nullptr){}
};

enum QueueType{
    BUY,SELL
};

class QuestQueue{
    Node * head;
    QueueType type;
public:
    explicit QuestQueue(QueueType type = BUY):head(nullptr),type(type){}
    void insert(Quest q){
        if(head){
            double new_price = q.getPrice();
            Node * before = nullptr;
            Node *p = head;
            while (p) {
                double p_price = p->data.getPrice();
                if ((type == BUY && p_price < new_price) || (type == SELL && p_price > new_price)) {
                    //found and insert
                    if(before){
                        //not on the head
                        Node * tmp = before->next;
                        before->next = new Node(q);
                        before->next->next = tmp;
                    }else{
                        //on the head
                        head = new Node(q);
                        head->next = p;
                    }
                    break;
                }
                before = p;
                p = p->next;
            }
            if(p == nullptr && before){
                //not found, add at tail
                before->next = new Node(q);
            }
        }else{
            head = new Node(std::move(q));
        }

    }
    bool cancel(int order_id,string & cancel_info){
        Node * before = nullptr;
        Node * p = head;
        while(p){
            if(debugMode) cout <<"neq-p:data: "<< p->data << endl;
            if(p->data.getOrderId() == order_id){
                if(debugMode) cout <<"p:data: "<< p->data << endl;
                stringstream s;
                s << p->data;
                cancel_info.append(s.str());
                if(before){
                    before->next = p->next;
                }else{
                    head = p->next;
                }
                delete p;
                break;
            }
            before=p;
            p = p->next;
        }
        return p!=nullptr;
    }
    void searchById(const string& id){
        cout << (type == BUY?"buy":"sell")<<" orders:" << endl;
        for(Node * p = head ; p ; p = p ->next){
            if(p->data.getStockId() == id){
                cout << p->data << endl;
            }
        }
    }
    void tradeBy(Quest &q){
        Node * before = nullptr;
        for(Node * p = head; p;){
            Node * tmp = nullptr;
            if (((type == BUY && p->data.getPrice() >= q.getPrice()) ||
                 (type == SELL && p->data.getPrice() <= q.getPrice()))
                && p->data.getStockId() == q.getStockId()) {
                //here we trade.
                double dealPrice = (p->data.getPrice() + q.getPrice())/2.0;
                int tradeAmount = min(q.getAmount(),p->data.getAmount());
                q.setAmount(q.getAmount() - tradeAmount);
                p->data.setAmount(p->data.getAmount() - tradeAmount);
                int sellorder,buyorder;
                cout << "deal--price:" << setw(6) << fixed << setprecision(1) << setfill(' ') << dealPrice
                     << "  quantity:"<< setw(4) << tradeAmount;
                if(type == SELL){
                    sellorder = p->data.getOrderId();
                    buyorder = q.getOrderId();
                    cout <<"  buyorder:"<< setw(4) << setfill('0') << buyorder
                         <<"  sellorder:" <<setw(4) << setfill('0') << sellorder;
                }else{
                    buyorder = p->data.getOrderId();
                    sellorder = q.getOrderId();
                    cout <<"  sellorder:" <<setw(4) << setfill('0') << sellorder
                         <<"  buyorder:"<< setw(4) << setfill('0') << buyorder;
                }
                cout << endl;

                if(p->data.getAmount() == 0){
                    if(before){
                        before->next = p->next;
                        tmp = p;
                    }else{
                        head = p->next;
                        tmp = p;
                    }
                }else{
                    before = p;
                }
                if(q.getAmount() == 0){
                    break;
                }
            }else{
                before = p;
            }
            p = p->next;
            delete tmp;
        }
    }
    friend ostream &operator<<(ostream &os, const QuestQueue &queue) {
        os << (queue.type == BUY?"buy":"sell")<<" orders:" << endl;
        for(Node * p = queue.head ; p ; p = p ->next){
            os << p->data << endl;
        }
        return os;
    }
};

class StockSystem{
    QuestQueue buy;
    QuestQueue sell;
public:
    StockSystem():buy(BUY),sell(SELL){}
    void addQuest(){
        string _stock_id;
        double price;
        int amount;
        char type;
        cin >> _stock_id >> price >> amount >> type;
        Quest q(_stock_id,price,amount,type);
        cout << "orderid: " << setw(4)<<setfill('0')<< q.getOrderId() << endl;
        switch(type){
            case 'b': case 'B':
                sell.tradeBy(q);
                if(q.getAmount()){
                    buy.insert(q);
                }
                break;
            case 's': case'S':
                buy.tradeBy(q);
                if(q.getAmount()){
                    sell.insert(q);
                }
                break;
            default:
                break;
        }
    }
    void showQuestQueue(){
        cout << buy;
        cout << sell;
    }
    void searchQuestQueueById(){
        string id;
        cin >> id;
        buy.searchById(id);
        sell.searchById(id);
    }
    void cancelQuestById(){
        int order_id;
        cin >> order_id;
        string result;
        if (buy.cancel(order_id, result) || sell.cancel(order_id, result)){
            cout << "deleted order:" << result <<endl;
        }else{
            cout << "not found" << endl;
        }
    }
};

int main() {
//    debugMode = true;
//    freopen("input4.txt","r",stdin);
//    freopen("output.txt","w",stdout);
    ios::sync_with_stdio(false);
    StockSystem ss;
    int mode;
    while(cin >> mode){
        switch(mode){
            case 1:
                ss.addQuest();
                break;
            case 2:
                ss.searchQuestQueueById();
                break;
            case 3:
                ss.cancelQuestById();
                break;
            default:
                return 0;
        }
    }
    return 0;
}