/*
IMPORTANT:
Compile using the flags for armadillo, lapack e blas
g++ -Wall -O3 -o pearson.out pearsonScript.cpp -larmadillo -llapack -lblas -fopenmp
*/

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

    // Getting all tids names
    vector<string> rownames;
    rownames.reserve(88000);
    string buffer;
    ifstream in("/storage/shared/fantom/hgnc_data_mat.csv");
    getline(in, buffer);  // skipping header
    while (getline(in, buffer, ',')) {
        rownames.push_back(buffer);
        getline(in, buffer);  // skip rest of the row
    }
    in.close();

    arma::mat samples;
    arma::field<string> header;
    samples.load(arma::csv_name("/storage/shared/fantom/hgnc_data_mat.csv",
                                header,
                                arma::csv_opts::trans));  // Loads transposed matrix
    samples.shed_row(0);                                  // Deleting the names, actually just 0s
    ofstream out("/storage/shared/fantom/pearsonMatrix2.csv");
    vector<string> res;
    res.reserve(7744000000);
#pragma omp parallel for schedule(dynamic)
    for (arma::uword i = 0; i < samples.n_cols; i++) {
        for (arma::uword j = 0; j < samples.n_cols; j++) {
            /*
            if (i != j && (corMatrix.find(minmax(rownames.at(i), rownames.at(j))) == corMatrix.end())) {  // Not the same isoform and not yet calculated
                corMatrix[minmax(rownames.at(i), rownames.at(j))] = arma::as_scalar(arma::cor(samples.col(i), samples.col(j)));
            }
            */
            auto p = arma::cor(samples.col(i), samples.col(j)).max();  // Must double check
            string s = rownames.at(i) + ',' + rownames.at(j) + ',' + to_string(p) + '\n';
            res.push_back(s);
        }
    }
    out.close();

    /*ofstream out("/storage/shared/fantom/pearsonMatrix2.csv");
    for (auto x : corMatrix) {
        out << x.first.first << ',' << x.first.second << ',' << x.second << '\n';
    }
    out.close();
    */

    ofstream out("/storage/shared/fantom/pearsonMatrix2.csv");
    for (string s : res) {
        out << s;
    }

    // Calculating the running time
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::minutes>(stop - start);
    cout << "Done! Running time " << duration.count() << " minutes\n";

    return EXIT_SUCCESS;
}