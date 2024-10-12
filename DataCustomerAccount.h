#pragma once
#include <iostream>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class DataCustomerAccount {
public:
    long long CAID{},CABID{},CID{};
    double BAL{};
    string NAME;
    int CATAXID{};

    friend ostream & operator<<(ostream& os, DataCustomerAccount & data ){
        return os<<data.CAID<<"|"<<data.CABID<<"|"<<data.CID<<"|"<<data.NAME<<"|"<<data.CATAXID<<"|"<<data.BAL;
    }
};



