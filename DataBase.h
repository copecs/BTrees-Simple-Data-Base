#pragma once
#include "DataCustomerAccount.h"
class DataBase{
public:
    struct NodeBase{
    public:
        NodeBase* next = nullptr;
        NodeBase* back = nullptr;

    public:
        NodeBase* listnext = nullptr;
        DataCustomerAccount val;
        NodeBase(DataCustomerAccount &val): val(val){}
        friend class DataBase;
    };
    
    NodeBase* head = nullptr;
    NodeBase* last = nullptr;
    void insert(NodeBase* next){
        if(not head){
            head = last = next;
        }
        else{
            NodeBase* prev = last;
            last = last->next = next;
            last->back =prev;
        }
    }
    void remove(NodeBase *node){
        NodeBase* prev = node->back;
        if(not prev){
            head = node->next;
            delete node;
            return;
        }
        prev->next = node->next;
        if(node->next){
            node->next->back = prev;
        }
        else{
            last = prev;
        }
        delete node;

    }
    void input(ifstream &input){
        string str;
        while(getline(input,str,'|')){
            DataCustomerAccount data;
            data.CAID =stoll(str);
            getline(input,str,'|');
            data.CABID = stoll(str);
            getline(input,str,'|');
            data.CID = stoll(str);
            getline(input,data.NAME,'|');
            getline(input,str,'|');
            data.CATAXID = stoi(str);
            getline(input,str,'\n');
            data.BAL = stod(str);
            insert(new NodeBase(data));
        }
    }

    void print(){
        NodeBase* curr = head;
        while(curr){
            cout<<curr->val<<endl;
            curr = curr->next;
        }
    }


};


