#include <armadillo>
#include <cmath>  //To overload abs()
#include <fstream>
#include <iostream>
#include <unordered_map>

using namespace std;

int main(int argc, char* argv[]) {
    cout << "Starting...\n";
    auto start = chrono::high_resolution_clock::now();

    ifstream relFreqFile("/storage/shared/fantom/FANTOM_RelativeFrequencyMatrix.csv");
    ofstream unifiedMatrix("/storage/shared/fantom/FANTOM_unified.csv");
    if (relFreqFile.fail() || unifiedMatrix.fail()) {
        cout << "Error opening the file!\n";
        exit(EXIT_FAILURE);
    }

    // Loading the matrix
    arma::mat pearson;
    arma::field<string> names;
    cout << "Loading the matrix... ";
    if (pearson.load(arma::csv_name("/storage/shared/fantom/FANTOM_PearsonMatrix_triangular.csv", names)) == false) {
        cout << "Error loading the matrix!\n";
        exit(EXIT_FAILURE);
    }
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
    unifiedMatrix << "Seed;Leaf;RelativeFrequency;PearsonCorrelation;relfreq-|pearson|";
    bool guard = true;
    while (guard) {
        getline(relFreqFile, t1, ';');
        getline(relFreqFile, t2, ';');
        getline(relFreqFile, freq, '\n');
        if (!relFreqFile.eof()) {
            unifiedMatrix << t1 << ';' << t2 << ';' << freq;
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
            unifiedMatrix << fixed << ';' << coeff << (stod(freq) - abs(coeff)) << '\n';
        } else {
            guard = false;
        }
    }

    relFreqFile.close();
    unifiedMatrix.close();

    // Calculating the running time
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::minutes>(stop - start);
    cout << "Done! Running time " << duration.count() << " minutes\n";

    return EXIT_SUCCESS;
}