#pragma once
#include <vector>
#include <iostream>
#include <cmath>
#include <queue>
#include "DataBase.h"
using namespace std;
class Btree {
private:
    struct Node{
        vector<long long> keys;
        vector<Node*> next;
        vector<DataBase::NodeBase*> values;
        Node * parr = nullptr;

        void insertKey(long long key,DataBase::NodeBase* val);
        void removeKey(long long pos,DataBase::NodeBase* val);
        bool isLeaf(){return next.empty();}
        int isHalfFull(){
            return (keys.size()>=(ceil(double(m)/2)-1))?keys.size():0;
        }

        Node(){
            values.resize(m+1);
        };

    };
    Node *root;
    static int m;
    static void setM(int mm);

public:
    Btree(int m){
        root = new Node();
        setM(m);
    }
    bool insert(long long key,DataBase::NodeBase* val);
    friend int findinsertplace(vector<long long> keys, long long key);
    void remove_all_accounts(long long key, DataBase &base);
    void remove_account(long long CID, long long CAID,DataBase &base );
    int search_customer(long long CID,string customerinfo);
    ~Btree(){}
    void print();

};