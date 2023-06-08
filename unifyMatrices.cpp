#include <armadillo>
#include <fstream>
#include <iostream>
#include <unordered_map>

using namespace std;

int main(int argc, int* argv[]) {
    ifstream relFreqFile("/storage/shared/fantom/FANTOM_RelativeFrequencyMatrix.csv");
    ofstream out("/storage/shared/fantom/FANTOM_unified.csv");

    // Loading the matrix
    arma::mat pearson;
    arma::field<string> names;
    cout << "Loading the matrix... ";
    pearson.load(arma::csv_name("/storage/shared/fantom/FANTOM_PearsonMatrix_triangular.csv", names));
    pearson.shed_row(0);
    cout << "Done!\n";

    unordered_map<string, int> dict;
    dict.reserve(88000);
    int i = 0;
    for (string n : names) {
        dict.insert({n, i});
        i++;
    }

    string t1, t2, freq;
    getline(relFreqFile, t1);  // Reading the header
    out << "Seed;Leaf;RelativeFrequency;PearsonCorrelation;relfreq-|pearson|";
    bool guard = true;
    while (guard) {
        getline(relFreqFile, t1, ';');
        getline(relFreqFile, t2, ';');
        getline(relFreqFile, freq, '\n');
        if (!relFreqFile.eof()) {
            out << t1 << ';' << t2 << ';' << freq;
            int posx = dict.at(t1);
            int posy = dict.at(t2);
            double coeff;
            if (posx > posy) {
                coeff = pearson(posx, posy);
            } else if (posy > posx) {
                coeff = pearson(posy, posx);
            } else {
                cout << "This is strange: a gene interacting with itself was found: " << posx << '&' << posy << endl;
                exit(EXIT_FAILURE);
            }
            out << fixed << ';' << coeff << (stod(freq) - coeff) << '\n';
        } else {
            guard = false;
        }
    }

    relFreqFile.close();
    out.close();
    return EXIT_SUCCESS;
}