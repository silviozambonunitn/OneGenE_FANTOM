#include <dirent.h>

#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

#define N_ISOFORMS 28013

void get_filenames(const string path, vector<string> &filenames) {
    filenames.reserve(N_ISOFORMS);
    DIR *dir;
    struct dirent *entry;
    if ((dir = opendir(path.data())) != nullptr) {
        while ((entry = readdir(dir)) != nullptr) {
            string filename = entry->d_name;
            if (filename.find(".exp.csv") != string::npos) {
                filenames.push_back(filename);
            }
        }
        closedir(dir);
    } else {
        cout << "Error opening directory\n";
        exit(EXIT_FAILURE);
    }
    if (filenames.empty()) {
        cout << "No isoform file found\n";
        exit(EXIT_FAILURE);
    }
}

// Avoiding recurring function calls as much as possible for efficiency
int main(int argc, char *argv[]) {
    cout << "Starting...\n";
    auto start = chrono::high_resolution_clock::now();

    // Getting all the expansion filenames
    vector<string> filenames;
    string dir = "./vitis/";  // Folder of the expansions
    get_filenames(dir, filenames);

    int n_tids_notfound = 0;
    fstream isoform_file;
    string seed_transcript, leaf_transcript, frel, seed_gene, leaf_gene;
    ofstream csv("VITIS_RelativeFrequencyMatrix.csv");
    if (csv.fail()) {
        cout << "Errore nell'apertura del file di output\n";
        exit(EXIT_FAILURE);
    }
    csv << "Seed;Leaf;RelativeFrequency\n";

    // Opening and parsing the files
    for (string f : filenames) {
        isoform_file.open(dir + f, ios::in);
        if (isoform_file.fail()) {
            cout << "Errore nell'apertura delle isoforme\n";
            exit(EXIT_FAILURE);
        }
        // Extracting the gene id
        for (int i = 0; i < 3; i++) {
            getline(isoform_file, seed_transcript, ',');
        }
        getline(isoform_file, seed_transcript, '\n');

        // Skipping the first two rows
        string buffer;
        getline(isoform_file, buffer);  // Deleting the header
        bool guard = true;
        while (guard) {
            // Getting the second isoform name
            getline(isoform_file, buffer, ',');  // Skipping rank
            getline(isoform_file, buffer, ',');
            if (!isoform_file.eof()) {  // Control for the last row
                leaf_transcript = buffer;
                getline(isoform_file, buffer, ',');  // Another dummy reading
                //If has multiple names, therefore the rest of the line is missing
                if (leaf_transcript.find("<br>") != string::npos) {
                    getline(isoform_file, frel); //Read to the end of the line
                } else {
                    getline(isoform_file, frel, ',');
                    // Skipping rest of line 
                    getline(isoform_file, buffer);
                }
                csv << seed_transcript << ';' << leaf_transcript << ';' << frel << '\n';
            } else {
                guard = false;
            }
        }
        isoform_file.close();
    }
    csv.close();

    // Calculating the running time
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::minutes>(stop - start);
    cout << "Done! Running time " << duration.count() << " minutes\n";
    if (n_tids_notfound > 0) {
        cout << "Number of tids not found: " << n_tids_notfound;
    }

    return EXIT_SUCCESS;
}