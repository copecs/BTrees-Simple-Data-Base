

#include "Btree.h"
#include <cmath>
int Btree::m=3;
void Btree::setM(int mm){
    Btree::m=mm;
}
void Btree::Node::insertKey(long long key,DataBase::NodeBase* val) {
    if(keys.empty()){
        keys.push_back(key);
        values[0] = val;
        return;
    }
    values[keys.size()] = val;
    keys.push_back(0);


    int i = keys.size()-1;
    while(i-1>=0 and keys[i-1]>key){
        keys[i]=keys[i-1];
        values[i] = values[i-1];
        if(not next.empty()) {
            next[i+1] = next[i];
        }
        i--;
    }
    keys[i]=key;

}

void Btree::Node::removeKey(long long pos,DataBase::NodeBase* val) {
    keys.erase(keys.begin()+pos);
    pos+=1;
    while(next.size()>pos+1){
        next[pos]=next[pos+1];
        pos++;
    }
    next.erase(next.begin()+next.size()-1);
}


int findinsertplace(vector<long long> keys, long long key) {
    int i=0;
    while(!keys.empty() and keys.size()>i and keys[i]<key){
        i++;
    }
    return i;
}

bool Btree::insert(long long key,DataBase::NodeBase* val) {
    Node* curr = root;

    Node* t = curr;
    bool test = true;
    while(t->keys.size() and t and test){
        int i = findinsertplace(t->keys,key);
        if(t->keys.size() and t->keys[i] == key){
            //samo dodajemo racun na kraj liste i proveravamo da slucajno nije dupliciran
            DataBase::NodeBase* curr_racun = t->values[i];
            while(curr_racun->listnext){
                if(curr_racun->val.CAID == val->val.CAID){
                    return false;
                }
                curr_racun=curr_racun->listnext;
            }
            curr_racun->listnext = val;
            return true;
        }
        else if(t->isLeaf()){
            test = false;
        }
        if(not t->isLeaf()){
            t = t->next[i];
        }
    }

    while(!curr->next.empty()){
        int i = findinsertplace(curr->keys,key);

        curr = curr->next[i];
    }
    curr->insertKey(key,val);
    while (curr->keys.size() == m) //treba cepanje
    {
        int sr = (m % 2 == 0) ? (m / 2 - 1) : (m / 2);
        long long zaslanje = curr->keys[sr];
        DataBase::NodeBase* zaslanjeval = curr->values[ceil(double(m-1)/2)];
        Node *newnode = new Node();
        newnode->parr = curr->parr;
        if(!curr->next.empty()){
            newnode->next.push_back(curr->next[sr+1]);
            curr->next[sr+1]->parr = newnode;

        }
        for(int i=sr+1;i<m;i++){
            newnode->keys.push_back(curr->keys[i]);
            newnode->values[i-(sr+1)]=curr->values[i];
            if(!curr->next.empty()){
                newnode->next.push_back(curr->next[i+1]);
                curr->next[i+1]->parr = newnode;
            }
        }
        for(int i=sr;i<m;i++){
            curr->keys.erase(curr->keys.begin()+sr);
            curr->values[i]= nullptr;
            if(!curr->next.empty()){
                curr->next.erase(curr->next.begin()+sr+1);
            }
        }
        Node *cale = curr->parr;
        if(not cale){
            root = curr->parr =newnode->parr= cale = new Node();
            root->next.push_back(curr);
            cale->next.push_back(newnode);
            cale->insertKey(zaslanje,zaslanjeval);

        }
        else {
            cale->next.push_back(nullptr);
            cale->insertKey(zaslanje,zaslanjeval);
            for (int i = 0; i < cale->keys.size(); i++) {
                if (cale->keys[i] == zaslanje) {
                    cale->next[i+1]= newnode;
                    newnode->parr = cale;
                }
            }
        }
        curr=cale;
    }
    return true;

}

void Btree::remove_all_accounts(long long key,DataBase &base) {
    //prvo lociramo
    Node* curr = root;
    bool trazi = true;
    int i;
    int br = 0;
    while(curr and trazi){
        i = findinsertplace(curr->keys,key);
        br++;
        if(curr->keys[i] == key){
            trazi = false;
            continue;
        }
        if(curr->next.size()>i) {
            curr = curr->next[i];
        }
        else{
            return;
        }
    }
    if(trazi){
        return;
    }
    if(!curr->isLeaf()){
        Node * curr2 = curr;
        curr2 = curr2->next[i+1];
        while(not curr2->isLeaf()){
            curr2=curr2->next[0];
        }
        swap(curr->keys[i],curr2->keys[0]);
        swap(curr->values[i],curr2->values[0]);
        i=0;
        curr=curr2;
    }

    curr->keys.erase(curr->keys.begin()+i);
    curr->values[i] = nullptr;
    while(curr->parr and !curr->isHalfFull()){
        //pitamo levog i desnog brata za pozajmicu
        Node* calac = curr->parr;


        int j=0;
        while(calac->next[j] != curr){
            j++;
        }

        if(j+1<calac->next.size() and calac->next[j+1]->isHalfFull()-1 >= ceil(double(m)/2)-1){
            //zajmimo od desnog
            curr->keys.push_back(calac->keys[j]);
            curr->values[curr->keys.size()-1] = calac->values[j];
            if(not curr->isLeaf()) {
                curr->next.push_back(calac->next[j + 1]->next[0]);
                calac->next[j + 1]->next.erase(calac->next[j + 1]->next.begin());
            }
            calac->keys[j] = calac->next[j+1]->keys[0];
            calac->values[j] = calac->next[j+1]->values[0];
            calac->next[j+1]->keys.erase(calac->next[j+1]->keys.begin()+0);
            calac->next[j+1]->values.erase(calac->next[j+1]->values.begin()+0);
            calac->next[j+1]->values.push_back(nullptr);
        }
        else if(j-1>=0 and calac->next[j-1]->isHalfFull()-1 >= ceil(double(m)/2)-1){
            //zajmimo od levog
            curr->keys.insert(curr->keys.begin(),calac->keys[j-1]);
            curr->values[0]=calac->values[j-1];
            if(not curr->isLeaf()) {
                curr->next.insert(curr->next.begin(), calac->next[j - 1]->next[calac->next[j - 1]->next.size() - 1]);
                calac->next[j - 1]->next.pop_back();
            }
            calac->keys[j-1] = calac->next[j-1]->keys[calac->next[j-1]->keys.size()-1];
            calac->values[j-1] = calac->next[j-1]->values[calac->next[j-1]->keys.size()-1];
            calac->next[j-1]->keys.pop_back();
            calac->next[j-1]->values[calac->next[j-1]->keys.size()] = nullptr;
        }
        else{
            //spajanje
            //prvo sa desnim pa ako ne postoji sa levim
            j=0;
            while(calac->next[j] != curr){
                j++;
            }
            if (j + 1 < calac->next.size()) {
                //spajanje sa desnim
            } else {
                //spajanje sa levim
                curr = calac->next[j - 1];
                j -= 1;
            }
            int iv = curr->keys.size();
            curr->keys.push_back(calac->keys[j]);
            curr->values[iv++] = calac->values[j];
            for (int k=0;k<calac->next[j + 1]->keys.size();k++) {
                curr->keys.push_back(calac->next[j+1]->keys[k]);
                curr->values[iv++] = calac->next[j+1]->values[k];
            }
            for(int k =0;k<calac->next[j+1]->next.size();k++){
                curr->next.push_back(calac->next[j+1]->next[k]);
            }
            for(int k=0;k<calac->next[j+1]->next.size();k++){
                calac->next[j+1]->next[k]->parr=curr;
            }
            delete calac->next[j + 1];
            calac->removeKey(j, nullptr);

            curr= calac;

            if(this->root->keys.empty()){
                this->root = this->root->next[0];
                return;
            }
        }
    }




}

void Btree::print() {
    queue<Node*> queue1;
    queue1.push(root);
    while(not queue1.empty()){
        int curr_queue_size = queue1.size();
        int pos = 0;
        for(int i=0;i<curr_queue_size;i++){
            Node *next = queue1.front();
            queue1.pop();
            cout<< pos++<<":( ";
            for (long long key : next->keys) {
                std::cout<< key << "; ";
            }
            cout<<") ";
            for (Node* child : next->next) {
                if (child != nullptr) {
                    queue1.push(child);
                }
            }
        }
        cout<<endl;

    }
}

int Btree::search_customer(long long int key, string customerinfo) {
    Node* t = root;
    bool test = true;
    int br = 0;
    while(t->keys.size() and t and test){
        int i = findinsertplace(t->keys,key);
        br+=1;
        if(t->keys.size() and t->keys[i] == key){

            DataBase::NodeBase* curr_racun = t->values[i];
            while(curr_racun){
                string out = "";
                out +=customerinfo;
                out+=to_string(curr_racun->val.CAID);
                out+="|";
                out+=to_string(curr_racun->val.CID);
                out+="|";
                out+=to_string(curr_racun->val.CATAXID);
                out+="|";
                out+=to_string(curr_racun->val.BAL);
                cout<<out<<endl;
                curr_racun=curr_racun->listnext;
            }
            return br;
        }
        else if(t->isLeaf()){
            test = false;
        }
        if(not t->isLeaf()){
            t = t->next[i];
        }
    }
    customerinfo+="null|null|null|null";
    cout<<customerinfo<<endl;
    return br;

}

void Btree::remove_account(long long int key, long long int CAID,DataBase &base) {
    Node* t = root;
    bool test = true;
    while(t->keys.size() and t and test){
        int i = findinsertplace(t->keys,key);
        if(t->keys.size() and t->keys[i] == key){
            //NASLI SMO CVOR SAD TRAZIMO RACUN DA GA OBRISEMO
            DataBase::NodeBase* curr_racun = t->values[i];
            DataBase::NodeBase* prev_racun = nullptr;
            while(curr_racun->listnext){
                if(curr_racun->val.CAID == CAID){
                    if(not prev_racun){
                        t->values[i] = curr_racun->listnext;
                        if(not t->values[i]){
                            base.remove(curr_racun);
                            remove_all_accounts(key,base);
                            return;
                        }
                    }
                    else{
                        prev_racun->listnext = curr_racun->listnext;
                    }
                    base.remove(curr_racun);
                    return;
                }
                prev_racun = curr_racun;
                curr_racun=curr_racun->listnext;
            }
            return;
        }
        else if(t->isLeaf()){
            test = false;
        }
        if(not t->isLeaf()){
            t = t->next[i];
        }
    }
}
