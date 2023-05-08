#include <iostream>
#include <fstream>
#include <map>

using namespace std;

int main(int argc,char* argv[]){
    ifstream in("hgnc_data_mat.csv");
    map<pair<string,string>,float> matrix; //Usare set invece di int per evitare duplicati? Il set è ordinato
    string buffer;
    getline(in,buffer); //Skipping the header
    while(in>>buffer){

    }
    return EXIT_SUCCESS;
}