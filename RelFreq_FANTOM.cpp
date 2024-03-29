#include <dirent.h>

#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

#define N_ISOFORMS 88000

void get_dictionary(const string path, unordered_map<string, string> &dictionary) {
    ifstream file(path);
    if (file.fail()) {
        cout << "Error opening the TIDs-genenames file!\n";
        exit(EXIT_FAILURE);
    }
    string tid, name;
    while (getline(file, tid, '|')) {
        if (tid[0] != 'T') {
            cout << tid << " does not start with T\n";
        }
        getline(file, name, '|');  // Dummy reading
        getline(file, name);       // Make sure it ends with the line
        dictionary.emplace(tid, name);
    }
}

void get_filenames(const string path, vector<string> &filenames) {
    filenames.reserve(N_ISOFORMS);
    DIR *dir;
    struct dirent *entry;
    if ((dir = opendir(path.data())) != nullptr) {
        while ((entry = readdir(dir)) != nullptr) {
            string filename = entry->d_name;
            if (filename.find(".expansion") != string::npos) {
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
    // Checks whether to use names instead of tids
    bool use_names = false;
    unordered_map<string, string> dictionary;
    if (argc > 1) {
        use_names = true;
        cout << "Using names instead of TIDs\n";
        dictionary.reserve(N_ISOFORMS);
        get_dictionary("/storage/shared/fantom/tcode-gene.csv", dictionary);
    }

    // Opening error log
    ofstream error_log("/storage/shared/fantom/error_log.txt");

    cout << "Starting...\n";
    auto start = chrono::high_resolution_clock::now();

    // Getting all the expansion filenames
    vector<string> filenames;
    string dir = "/storage/shared/fantom/hs.FANTOM/";  // Folder of the expansions
    get_filenames(dir, filenames);

    int n_tids_notfound = 0;
    fstream isoform_file;
    string seed_transcript, leaf_transcript, frel, seed_gene, leaf_gene;
    ofstream csv("/storage/shared/fantom/FANTOM_RelativeFrequencyMatrix_tid.csv");
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
                getline(isoform_file, frel, ',');
                if (use_names) {
                    try {
                        seed_gene = dictionary.at(seed_transcript);
                        leaf_gene = dictionary.at(leaf_transcript);
                        csv << seed_gene << ';' << leaf_gene << ';' << frel << '\n';
                    } catch (const out_of_range &e) {
                        error_log << "The following transcripts were not found in the TID->name file: "
                                  << seed_transcript << ' ' << leaf_transcript << '\n';
                        ++n_tids_notfound;
                    }
                } else {
                    csv << seed_transcript << ';' << leaf_transcript << ';' << frel << '\n';
                }
            } else {
                guard = false;
            }
        }
        isoform_file.close();
    }
    csv.close();
    error_log.close();

    // Calculating the running time
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::minutes>(stop - start);
    cout << "Done! Running time " << duration.count() << " minutes\n";
    if (n_tids_notfound > 0) {
        cout << "Number of tids not found: " << n_tids_notfound;
    }

    return EXIT_SUCCESS;
}