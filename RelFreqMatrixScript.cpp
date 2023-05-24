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
    while (!file.eof()) {
        getline(file, tid, '|');
        if (tid[0] != 'T') {
            cout << tid << " does not start with T\n";
        }
        getline(file, name, '|');  // Dummy reading
        getline(file, name);       // Make sure it ends with the line
        dictionary.insert({tid,name});
    }
}

void get_filenames(const string path, vector<string> &filenames) {
    filenames.reserve(89000);  // Number of isoforms
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
        cout << "Error opening directory";
        exit(EXIT_FAILURE);
    }
    filenames.shrink_to_fit();  // Useless?
    if (filenames.empty()) {
        cout << "No isoform file found\n";
        exit(EXIT_FAILURE);
    }
}

// Avoiding recurring function calls as much as possible for efficiency
int main(int argc, char *argv[]) {
    bool use_names = false;
    if (argc > 1) {
        use_names = true;
        cout << "Using names instead of TIDs\n";
    }

    // Opening error log
    ofstream error_log("/storage/shared/fantom/error_log.txt");

    cout << "Starting...\n";
    auto start = chrono::high_resolution_clock::now();

    // Getting all the expansion filenames
    vector<string> filenames;
    get_filenames("/storage/shared/fantom/hs_all_results/", filenames);

    // Replace TID with real names
    unordered_map<string, string> dictionary;
    if (use_names) {
        dictionary.reserve(N_ISOFORMS);
        get_dictionary("/storage/shared/fantom/tcode-gene.csv", dictionary);
    }

    int n_tids_notfound = 0;
    fstream isoform_file;
    string seed_transcript, leaf_transcript, frel, seed_gene, leaf_gene;
    ofstream csv("/storage/shared/fantom/FANTOM_RelativeFrequencyMatrix.csv");
    if (csv.fail()) {
        cout << "Errore nell'apertura del file di output\n";
        exit(EXIT_FAILURE);
    }
    csv << "Seed;Leaf;RelativeFrequency\n";

    // Opening and parsing the files
    string dir = "/storage/shared/fantom/hs_all_results/";
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
                        error_log << "The following transcript was not found in the TID->name file: " << seed_transcript << '\n';
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