/*
This script counts the number of Pearson correlations greater than 0.046
*/

#include <armadillo>
#include <iostream>

int main() {
    arma::mat m;
    arma::field<std::string> header;
    m.load(arma::csv_name("/storage/shared/FANTOM_PearsonMatrix_triangular.csv", header));

    float threshold = 0.046;
    arma::uvec indices = arma::find(m > threshold);
    std::size_t count = indices.n_elem;

    std::cout << "Number of occurrences greater than " << threshold << ": " << count << std::endl;

    return 0;
}
