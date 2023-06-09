/*
IMPORTANT: INPUT FILES SEEMS TO NEED AN EMPTY NEWLINE AT THE END TO DON'T LOSE DATA.
NEEDS CHECKING
COMPILE USING g++ -Wall -O3 unifyMatrices.cpp -larmadillo -fopenmp
*/

#include <armadillo>
#include <cmath>  //To overload abs()
#include <fstream>
#include <iostream>
#include <unordered_map>

using namespace std;

int main(int argc, char* argv[]) {
    cout << "Starting...\n";
    auto start = chrono::high_resolution_clock::now();

    ifstream relFreqFile("p.csv");
    ofstream unifiedMatrix("FANTOM_unified.csv");
    if (relFreqFile.fail() || unifiedMatrix.fail()) {
        cout << "Error opening the file!\n";
        exit(EXIT_FAILURE);
    }
    cout << "Opened the streams\n";

    // Loading the matrix
    arma::mat pearson;
    arma::field<string> names;
    cout << "Loading the matrix... ";
    bool check = pearson.load(arma::csv_name("m.csv", names));
    if (check == false) {
        cout << "Error loading the matrix!\n";
        exit(EXIT_FAILURE);
    }
    // pearson.shed_row(0);
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
    unifiedMatrix << "Seed;Leaf;RelativeFrequency;PearsonCorrelation;relfreq-|pearson|\n";
    bool guard = true;
    while (guard) {
        getline(relFreqFile, t1, ';');
        getline(relFreqFile, t2, ';');
        getline(relFreqFile, freq, '\n');
        if (!relFreqFile.eof()) {
            unifiedMatrix << t1 << ';' << t2 << ';' << freq;
            int posx, posy;
            try {
                posx = dict.at(t1);
                posy = dict.at(t2);
            } catch (const out_of_range& e) {
                cout << "Not found " << t1 << "or" << t2 << " in the dict" << endl;
                exit(1);
            }
            double coeff;
            if (posx > posy) {
                try {
                    coeff = pearson(posx, posy);
                } catch (const out_of_range& e) {
                    cout << "Error in arma matrix range " << posx << ',' << posy << endl;
                    exit(1);
                }
            } else if (posy > posx) {
                try {
                    coeff = pearson(posy, posx);
                } catch (const out_of_range& e) {
                    cout << "Error in arma matrix range " << posx << ',' << posy << endl;
                    exit(1);
                }
            } else {
                cout << "This is strange: a gene interacting with itself was found: " << t1 << '&' << t2 << endl;
                exit(EXIT_FAILURE);
            }
            unifiedMatrix << fixed << ';' << coeff << ';' << (stod(freq) - abs(coeff)) << '\n';
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