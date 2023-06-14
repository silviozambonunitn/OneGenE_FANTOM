/*
This script counts the number of Pearson correlations greater than 0.046
Compile using g++ -Wall -O3 -o countOcc.o countOccurences.cpp -larmadillo -llapack -lblas -fopenmp
*/

#include <armadillo>
#include <iostream>

int main() {
    arma::mat m;
    arma::field<std::string> header;

    std::cout << "Loading the matrix... " << std::flush;
    const bool success = m.load(arma::csv_name("/storage/shared/FANTOM_PearsonMatrix_triangular.csv", header));
    if (success) {
        std::cout << "Done!\n";
    } else {
        std::cout << "Failed!\n";
    }

    double threshold = 0.046;
    arma::uvec indices = arma::find(m > threshold);
    std::size_t count = indices.n_elem;

    std::cout << "Number of occurrences greater than " << threshold << ": " << count << std::endl;

    return 0;
}
