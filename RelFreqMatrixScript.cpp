#include <chrono>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

class Coords {
   public:
    string iso1;
    string iso2;

    // Don't know how legit this actually is, kind of spaghetti code, but it's required to define the < operator in mapsy
    bool operator<(const Coords &other) const {
        if (iso1 < other.iso1)
            return true;
        if (other.iso1 < iso1)
            return false;
        return iso2 < other.iso2;
    }
};

int main(int argc, char *argv[]) {
    cout << "Starting...\n";
    auto start = chrono::high_resolution_clock::now();

    // Reading all files in the current directory
    if(system("ls > files.txt")!=0)
        cout<<"Error finding the input files!\n";

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
        if (buffer.find(".expansion") != string::npos) {
            filenames.push_back(buffer);
        }
    }
    pre.close();
    filenames.shrink_to_fit();

    // Creating the matrix: coords are the isoforms, float is the rel frequency
    map<Coords, float> matrix;
    fstream isoform_file;
    Coords coords;
    string id1;
    float frel;
    
    // Opening and parsing the files
    //#pragma omp parallel for
    for (auto f : filenames) {
        isoform_file.open(f, ios::in);
        // Extracting the isoform id
        for (int i = 0; i < 3; i++)
            isoform_file >> id1;
        isoform_file.get();  // To skip blank space
        getline(isoform_file, id1, '-'); //To delete the gene name

        // For coherence with the other isoforms ids
        id1[0] = tolower(id1[0]);

        // Skipping the first two rows
        getline(isoform_file, buffer);
        getline(isoform_file, buffer);
        bool guard = true;
        #pragma omp parallel while
        while (guard) {
            // Getting the second isoform name
            getline(isoform_file, buffer, ',');
            getline(isoform_file, buffer, ',');
            if (!isoform_file.eof()) {  // Control for the last row
                coords.iso2 = buffer;
                coords.iso1 = id1;
                // cout << coords.iso1 << " ";
                // cout << buffer << " ";
                //   Another dummy reading
                getline(isoform_file, buffer, ',');
                // Getting the relative frequency
                getline(isoform_file, buffer, ',');
                frel = stof(buffer);
                // cout << frel << endl;
                matrix[coords] = frel;
            } else {
                guard = false;
            }
        }
        isoform_file.close();
    }

    // Saving the matrix on a csv file
    fstream csv;
    csv.open("matrix.csv", ios::out);
    for (const auto &elem : matrix) {
        csv << setprecision(14) << elem.first.iso1 << ';' << elem.first.iso2 << ';' << elem.second << '\n';
    }

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "Done! Running time " << duration.count() << " milliseconds\n";

    return 0;
}