/*
IMPORTANT:
[DEPRECATED]
Produces a Coordinates List formatted matrix
This version outputs a 270 GB csv file, run only if there's enough space
Compile using the flags for armadillo, lapack e blas:
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
    corMatrix.reserve(88000);

    // Getting all tids names
    vector<string> rownames;
    rownames.reserve(88000);
    string buffer;
    ifstream in("/storage/shared/fantom/hgnc_data_mat.csv");
    if (in.fail()) {
        cout << "Error opeing the input file!\n";
        exit(EXIT_FAILURE);
    }
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
    cout << "Done loading the matrix\n";

    arma::mat cor = arma::cor(samples);
    cout << "Done calculating the cor\n";

    ofstream o("/storage/shared/fantom/cor.csv");
    cor.save(o, arma::coord_ascii);
    o.close();
    cout << "Done saving the matrix\n";

    // Calculating the running time
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::minutes>(stop - start);
    cout << "Done! Running time " << duration.count() << " minutes\n";

    return EXIT_SUCCESS;
}