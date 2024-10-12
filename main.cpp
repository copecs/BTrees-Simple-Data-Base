#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include "Btree.h"
#include "DataBase.h"

using namespace std;


void print_menu(){
    cout<<"1.Podesi datoteku za customer account"<<endl;
    cout<<"2.Kreiraj indexnu strukturu nad customer account"<<endl;
    cout<<"3.Ispisi indexnu struktur"<<endl;
    cout<<"4.Nadji korisnika i sve njegove racune pomocu C_ID"<<endl;
    cout<<"5.Unos novog customer accounta"<<endl;
    cout<<"6.Brisanje jednog racuna za nekog customera"<<endl;
    cout<<"7.Brisanje svih racuna customera"<<endl;
    cout<<"END - TERMINACIJA"<<endl;
}

pair<string,int> customer_info(string CIDtoFIND){
    ifstream input;
    input.open("Customer.txt");
    string CID,NAME,LNAME,EMAIL,AD;
    int br=0;
    pair<string,int> res;
    res.first =  "C_ID|C_F_NAME|C_L_NAME|C_EMAIL_1|";
    while(getline(input,CID,'|')){
        getline(input,NAME,'|');
        getline(input,LNAME,'|');
        getline(input,EMAIL,'|');
        getline(input,AD,'\n');
        br+=1;
        if(CID == CIDtoFIND){

            res.first=CID+"|"+NAME+"|"+LNAME+"|"+EMAIL+"|";
            res.second=br;
            input.close();
            return res;
        }
    }
    res.second =br;
    input.close();
    return res;
}

int main() {
    ifstream input;
    string imedatoteke;
    string izbor;
    DataBase* base = nullptr;
    Btree *drvo = nullptr;
    int m = 3;
    while(1){
        print_menu();
        cin>>izbor;
        if(izbor == "1"){
            base = new DataBase;
            cout<<"Unesite ime datoteke"<<endl;
            cin>>imedatoteke;
            input.open(imedatoteke);
            if(not input.is_open()){
                cout<<"GRESKA";
                exit(0);
            }
            base->input(input);
            input.close();
            cout<<"Baza podataka Custommer Account uspesno ucitana!"<<endl;
        }
        else if(izbor == "2" and base){
            cout<<"Unesite parametar m za B stablo:"<<endl;
            cin>>m;
            drvo = new Btree(m);
            DataBase::NodeBase* curr = base->head;
            while(curr){
                drvo->insert(curr->val.CID,curr);
                curr = curr->next;
            }
            cout<<"Stablo uspesno kreirano!"<<endl;
        }
        else if(izbor == "3" and drvo){
            drvo->print();
        }
        else if(izbor == "4" and drvo) {
            string key;
            cout<<"Unesite CID korisnika cije racune zelite da vidite:"<<endl;
            cin>>key;
            pair<string,int> res = customer_info(key);
            int pristupdisku = drvo->search_customer(stoll(key),res.first);
            cout<<"Podaci o customeru nadjeni u datoteci za:"<<res.second<<" koraka"<<endl;
            cout<<"Podaci o njegovim racunima u stablu nadjeni za:"<<pristupdisku<<" pristupa disku"<<endl;

        }
        else if(izbor == "5" and drvo and base){
            cout<<"Unesite sve podatke za unos novog zapisa u formatu:"<<endl;
            cout<<"CA_ID|CA_B_ID|CA_C_ID|CA_NAME|CA_TAX_ST|CA_BAL"<<endl;
            cout<<"longlong|longlong|longlong|str|longlong|double"<<endl;
            DataCustomerAccount data;
            string str;
            getline(cin,str,'|');
            data.CAID =stoll(str);
            getline(cin,str,'|');
            data.CABID = stoll(str);
            getline(cin,str,'|');
            data.CID = stoll(str);
            getline(cin,data.NAME,'|');
            getline(cin,str,'|');
            data.CATAXID = stoi(str);
            getline(cin,str,'\n');
            data.BAL = stod(str);
            base->insert(new DataBase::NodeBase(data));
            bool isunique = drvo->insert(data.CID,base->last);
            if(not isunique){
                cout<<"GRESKA(Primary key CAID vec postoji)"<<endl;
            }
        }

        else if(izbor == "6" and drvo and base){
            cout<<"Unesite sve podatke za brisanje zapisa zapisa u formatu:"<<endl;
            cout<<"CA_ID|CA_B_ID|CA_C_ID|CA_NAME|CA_TAX_ST|CA_BAL"<<endl;
            cout<<"longlong|longlong|longlong|str|longlong|double"<<endl;
            DataCustomerAccount data;
            string str;
            getline(cin,str,'|');
            data.CAID =stoll(str);
            getline(cin,str,'|');
            data.CABID = stoll(str);
            getline(cin,str,'|');
            data.CID = stoll(str);
            getline(cin,data.NAME,'|');
            getline(cin,str,'|');
            data.CATAXID = stoi(str);
            getline(cin,str,'\n');
            data.BAL = stod(str);
            drvo->remove_account(data.CID,data.CAID,*base);
            cout<<"Uspesno ukljonjeno"<<endl;
        }
        else if(izbor =="7" and drvo and base){
            cout<<"Unesite CID customera cije sve racune zelite da obrisete";
            long long CID;
            cin>>CID;
            drvo->remove_all_accounts(CID,*base);
            cout<<"Uspesno obrisano!"<<endl;
        }
        else if(izbor =="END"){
            break;
        }
        else{
            cout<<"NEVALIDAN UNOS!"<<endl;
        }


    }
    delete base;

    return 0;
}
