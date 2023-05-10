#include <iostream>
#include <fstream>
#include <map>

using namespace std;

bool exists(map<pair<string,string>,float>* m, pair<string,string>* p){
    pair<string,string> reverse;
    reverse.first=p->second;
    reverse.second=p->first;
    return m->count(*p)!=0 || m->count(reverse)!=0;
}

int main(int argc,char* argv[]){
    ifstream in("hgnc_data_mat.csv");
    map<pair<string,string>,float> matrix; //Usare set invece di int per evitare duplicati? Il set è ordinato
    string buffer;
    getline(in,buffer); //Skipping the header
    while(in>>buffer){

    }
    return EXIT_SUCCESS;
}