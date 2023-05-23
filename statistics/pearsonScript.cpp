#include <armadillo>
#include <boost/functional/hash.hpp>
#include <fstream>
#include <iostream>
#include <unordered_map>

using namespace std;

int main(int argc, char* argv[]) {
    cout << "Starting...\n";
    auto start = chrono::high_resolution_clock::now();
    unordered_map<pair<string, string>, float, boost::hash<pair<string, string>>> corMatrix;

    string t1, t2;
    arma::mat samples;
    arma::field<string> header;
    samples.load(arma::csv_name("/home/silvio/Documents/OneGenE_FANTOM/hgnc_data_mat.csv", header, arma::csv_opts::trans));  // Loads transposed matrix
    // auto corMatrix = arma::cor(samples);
    // corMatrix.brief_print();
    for (int i = 0; i < samples.n_cols; i++) {
        for (int j = 0; j < samples.n_cols; j++) {
            if (i != j && (corMatrix.find(minmax(t1, t2)) == corMatrix.end())) {  // Not the same isoform and not yet calculated
                //Calcola corr e inserisci in hashmap
            }
        }
    }
    cout << header;

    // Calculating the running time
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "Done! Running time " << duration.count() << " milliseconds\n";

    return EXIT_SUCCESS;
}