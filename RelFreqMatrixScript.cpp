#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
    cout << "Starting...\n";
    auto start = chrono::high_resolution_clock::now();

    //Activate for efficiency
    /*
    if (system("rm *.interactions") != 0)
        cout << "Error deleting the .interactions files\n";
    */

    // Reading all files in the current directory
    if (system("ls > files.txt") != 0)
        cout << "Error finding the input files!\n";

    // Reading all csv file names
    fstream pre;
    pre.open("files.txt");
    if (pre.fail()) {
        cout << "Error opening the file \"files.txt\" in writing mode\n";
        exit(1);
    }
    vector<string> filenames;  // Vector containing the isoform files names
    string buffer;
    while (pre >> buffer) {
        if (buffer.find(".expansion") != string::npos) {  // Saving only the expansion files
            filenames.push_back(buffer);
        }
    }
    pre.close();
    filenames.shrink_to_fit();

    // Creating the matrix: coords are the isoforms, float is the rel frequency
    map<pair<string, string>, string> matrix;
    fstream isoform_file;
    string iso1, iso2, frel;

    // Opening and parsing the files
    for (auto f : filenames) {
        isoform_file.open(f, ios::in);
        // Extracting the isoform id
        for (int i = 0; i < 3; i++)
            isoform_file >> iso1;
        isoform_file.get();                // To skip blank space
        getline(isoform_file, iso1, '-');  // To delete the gene name

        // For coherence with the other isoforms ids
        iso1[0] = tolower(iso1[0]);

        // Skipping the first two rows
        getline(isoform_file, buffer);  // Isoform details
        getline(isoform_file, buffer);  // Header (rank,node,Fabs,Frel,Class)
        bool guard = true;
        while (guard) {
            // Getting the second isoform name
            getline(isoform_file, buffer, ',');
            getline(isoform_file, buffer, ',');
            if (!isoform_file.eof()) {  // Control for the last row
                iso2 = buffer;
                //   Another dummy reading
                getline(isoform_file, buffer, ',');
                // Getting the relative frequency
                getline(isoform_file, frel, ',');
                matrix[make_pair(iso1, iso2)] = frel;
            } else {
                guard = false;
            }
        }
        isoform_file.close();
    }

    // Saving the matrix on a csv file
    fstream csv;
    csv.open("matrix.csv", ios::out);
    csv << "Seed;Leaf;RelativeFrequency\n";
    for (const auto &elem : matrix) {
        csv << elem.first.first << ';' << elem.first.second << ';' << elem.second << '\n';
    }
    csv.close();

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "Done! Running time " << duration.count() << " milliseconds\n";

    return 0;
}