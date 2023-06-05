#include <armadillo>
#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, int* argv[]) {
    ifstream relfreq("");
    ofstream out("");

    arma::mat pearson;
    arma::field<string> names;
    cout << "Loading the matrix... ";
    pearson.load(arma::csv_name("", names));
    cout << "Done!\n";
    string buf1, buf2;
    string t1,t2;
    getline(relfreq, buf1);  // Reading the header
    bool guard = true;
    while (guard) {
        getline(relfreq, buf1, ';');
        getline(relfreq, buf2, ';');

    }
    return EXIT_SUCCESS;
}