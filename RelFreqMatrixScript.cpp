#include <dirent.h>

#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

#define N_ISOFORMS 88000

void get_dictionary(string path, map<string, string> &dictionary) {
    ifstream file(path);
    if (file.fail()) {
        cout << "Error opening the dictionary file!\n";
        exit(EXIT_FAILURE);
    }
    string tid, name;
    while (!file.eof()) {
        getline(file, tid, '|');
        getline(file, name, '|');
        getline(file, name);  // Make sure it ends with the line
        dictionary[tid] = name;
    }
}

// Avoiding recurring function calls as much as possible for efficiency
int main(int argc, char *argv[]) {
    cout << "Starting...\n";
    auto start = chrono::high_resolution_clock::now();

    // For storage and fs efficiency
    // static_cast<void>(system("rm *.interactions"));

    // Getting all the expansion filenames
    vector<string> filenames;  // Vector containing the isoform files names
    filenames.reserve(89000);  // Number of isoforms
    DIR *dir;
    struct dirent *entry;
    if ((dir = opendir("./")) != nullptr) {
        while ((entry = readdir(dir)) != nullptr) {
            string filename = entry->d_name;
            if (filename.find(".expansion") != string::npos) {
                filenames.push_back(filename);
            }
        }
        closedir(dir);
    } else {
        cout << "Error opening directory";
        exit(EXIT_FAILURE);
    }
    filenames.shrink_to_fit();  // Useless?
    if (filenames.empty()) {
        cout << "No isoform file found\n";
        exit(EXIT_FAILURE);
    }

    map<string, string> dictionary;
    get_dictionary("/storage/shared/fantom/tcode-gene.csv", dictionary);

    fstream isoform_file;
    string seed_transcript, leaf_transcript, frel, seed_gene, leaf_gene;
    ofstream csv("/storage/shared/fantom/FANTOM_RelativeFrequencyMatrix.csv");
    csv << "Seed;Leaf;RelativeFrequency\n";

    // Opening and parsing the files
    for (string f : filenames) {
        isoform_file.open(f, ios::in);
        if (isoform_file.fail()) {
            cout << "Errore nell'apertura delle isoforme\n";
            exit(EXIT_FAILURE);
        }
        // Extracting the isoform id
        for (int i = 0; i < 3; i++)
            isoform_file >> seed_transcript;
        isoform_file.get();                           // To skip blank space
        getline(isoform_file, seed_transcript, '-');  // To get the transcript id from the TID-GeneName string

        // Skipping the first two rows
        string buffer;
        getline(isoform_file, buffer);  // Deleting isoform details
        getline(isoform_file, buffer);  // Deleting the header (rank,node,Fabs,Frel,Class)
        bool guard = true;
        while (guard) {
            // Getting the second isoform name
            getline(isoform_file, buffer, ',');
            getline(isoform_file, buffer, ',');
            if (!isoform_file.eof()) {  // Control for the last row
                leaf_transcript = buffer;
                leaf_transcript[0] = toupper(leaf_transcript[0]);  // For coherence with the other isoform id
                getline(isoform_file, buffer, ',');                // Another dummy reading
                getline(isoform_file, frel, ',');                  // Getting the relative frequency
                seed_gene = dictionary.at(seed_transcript);
                leaf_gene = dictionary.at(leaf_transcript);
                csv << seed_gene << ';' << leaf_gene << ';' << frel << '\n';
            } else {
                guard = false;
            }
        }
        isoform_file.close();
        /*
        // Deleting the file after reading for fs efficiency, activate if needed
        string s = "rm " + f;
        if (system(s.c_str()) != 0) {
            cout << "Error deleting the isoform file";
        }
        */
    }
    csv.close();

    // Calculating the running time
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "Done! Running time " << duration.count() << " milliseconds\n";

    return EXIT_SUCCESS;
}