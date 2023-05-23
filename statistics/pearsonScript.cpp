#include <armadillo>
#include <boost/functional/hash.hpp>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    cout << "Starting...\n";
    auto start = chrono::high_resolution_clock::now();
    unordered_map<pair<string, string>, float, boost::hash<pair<string, string>>> corMatrix;

    //Getting all tids names
    vector<string> rownames;
    string buffer;
    ifstream in("/home/silvio/Documents/OneGenE_FANTOM/hgnc_data_mat.csv");
    getline(in, buffer);  // skipping header
    while (getline(in, buffer, ',')) {
        rownames.push_back(buffer);
        getline(in, buffer);  // skip rest of the row
    }

    string t1, t2;
    arma::mat samples;
    arma::field<string> header;
    samples.load(arma::csv_name("/home/silvio/Documents/OneGenE_FANTOM/hgnc_data_mat.csv", header, arma::csv_opts::trans));  // Loads transposed matrix

    for (int i = 0; i < samples.n_cols; i++) {
        for (int j = 0; j < samples.n_cols; j++) {
            if (i != j && (corMatrix.find(minmax(rownames.at(i), rownames.at(j))) == corMatrix.end())) {  // Not the same isoform and not yet calculated
                corMatrix[minmax(t1, t2)] = arma::as_scalar(arma::cor(samples.col(i), samples.col(j)));
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