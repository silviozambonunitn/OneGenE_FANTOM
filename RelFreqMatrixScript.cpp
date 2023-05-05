#include <dirent.h>
#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

// Avoiding function calls as much as possible for efficiency
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
        exit(1);
    }
    // filenames.shrink_to_fit();  // Useless?

    fstream isoform_file;
    string iso1, iso2, frel;
    ofstream csv("FANTOM_RelativeFrequencyMatrix.csv");
    csv << "Seed;Leaf;RelativeFrequency\n";

    // Opening and parsing the files
    for (string f : filenames) {
        isoform_file.open(f, ios::in);
        if(isoform_file.fail()){
            cout<<"Errore nell'apertura delle isoforme\n";
            exit(1);
        }
        // Extracting the isoform id
        for (int i = 0; i < 3; i++)
            isoform_file >> iso1;
        isoform_file.get();                // To skip blank space
        getline(isoform_file, iso1, '-');  // To get the transcript id

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
                iso2 = buffer;
                iso2[0] = toupper(iso2[0]);          // For coherence with the other isoform id
                getline(isoform_file, buffer, ',');  // Another dummy reading
                getline(isoform_file, frel, ',');    // Getting the relative frequency
                csv << iso1 << ';' << iso2 << ';' << frel << '\n';
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

    return 0;
}