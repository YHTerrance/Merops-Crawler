#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <vector>
#include <cassert>
#include <iomanip>

using namespace std;


typedef struct cut {
    string sequence;
    string meropsID;
    string meropsName;
    string cleavageSite;
}Cut;

typedef struct protein {
    string sequence;
    string name;
}Protein;


vector<string> getTokenedLine(istream& str) {
    vector<string> result;
    string line;
    getline(str, line);

    stringstream lineStream(line);
    string cell;

    // Split by ',' and store in cell
    while(getline(lineStream, cell, ','))
        result.push_back(cell);

    // Check for trailing comma with no data after it
    if(!lineStream && cell.empty())
        // Push empty element
        result.push_back("");

    return result;
}

void findCutSites(Cut& currentCut, Protein prot[], ofstream fout[]) {

    // The currentCut.sequence has no selection specificity and is therefore not considered
    if(currentCut.sequence == "********")
        return;

    // Replace all '*' with regex [A-Z]
    int index;
    while((index = currentCut.sequence.find('*')) != string::npos)
        currentCut.sequence.replace(index, 1 , "[A-Z]");

    cout << "Merops ID: " << currentCut.meropsID << endl;
    cout << "Merops Name: " << currentCut.meropsName << endl;
    cout << "Cleavage Site: " << currentCut.cleavageSite << endl;
    cout << "Cut Sequence: " << currentCut.sequence << endl;

    // Try to match sequence using regex
    regex e(currentCut.sequence.c_str());
    smatch res;

    bool found[2] = {false, false};

    for(int i = 0; i < 2; ++i) {
        string::const_iterator searchStart(prot[i].sequence.cbegin());

        while(regex_search(searchStart, prot[i].sequence.cend(), res, e)) {
            if(!found[i])
                cout << "Found in " << prot[i].name << endl;

            found[i] = true;
            int start = prot[i].sequence.find(res[0].str());

            cout << "from " << setw(5) << start + 1 << " to " <<  setw(5) << start + res[0].str().length();

            // Go back at keep searching so that it only increments by one
            cout << setw(12) << res[0] << endl;
            searchStart = res.suffix().first - 7;
        }

        if(found[i])
            cout << "========" << endl;
    }

    // Only cut sequence 1
    if(found[0] && !found[1])
        fout[0] << currentCut.meropsID << " " << currentCut.meropsName << " " << currentCut.cleavageSite << endl;

    // Only cut sequence 2
    else if(!found[0] && found[1])
        fout[1] << currentCut.meropsID << " " << currentCut.meropsName << " " << currentCut.cleavageSite << endl;
    if(found[0] || found[1])
        cout << endl;

}

void readSequence(ifstream fin[], Protein prot[]) {
    // Temporary varialbe to store every line
    string line;

    // Remove first line
    getline(fin[0], line);
    getline(fin[1], line);

    // Read in sequence data
    while(getline(fin[0], line))
        prot[0].sequence += line;
    while(getline(fin[1], line))
        prot[1].sequence += line;

    return;
}

int main(int argc, char *argv[]) {

    // Filename of sequence files
    string dataFile;
    Protein prot[2];
    prot[0].name = argv[1];
    prot[1].name = argv[2];
    dataFile = "merops.csv";

    // Check if the provided files are of .fasta format
    if(prot[0].name.find(".fasta") == string::npos || prot[1].name.find(".fasta") == string::npos) {
        cerr << "We only accept .fasta files for sequences!" << endl;
        exit(-1);
    }

    // Open Merops data file
    ifstream finCSV;
    finCSV.open(dataFile);

    if(!finCSV) {
        cerr << "Can not open " << dataFile << endl;
        exit(-1);
    }

    // Input file
    ifstream fin[2];
    fin[0].open(prot[0].name);
    fin[1].open(prot[1].name);

    if(!fin[0] || !fin[1]) {
        cerr << "File not found" << endl;
        exit(-1);
    }

    // Remove extensions
    prot[0].name = prot[0].name.substr(0, prot[0].name.find('.', 0));
    prot[1].name = prot[1].name.substr(0, prot[1].name.find('.', 0));

    // Output file
    ofstream fout[2];
    fout[0].open(prot[0].name + ".result");
    fout[1].open(prot[1].name + ".result");
    if(!fout[0] || !fout[1]) {
        cerr << "Can not create file" << endl;
        exit(-1);
    }

    // Read in designated .fasta files
    readSequence(fin, prot);

    Cut currentCut;

    vector<string> line;
    // Get first line
    line = getTokenedLine(finCSV);
    int n = 1000; // Specify the lines processed

    while(n-- && line.size() >= 2) {

        // if the line specfifies the merops ID
        if(line[0].find('.') != string::npos) {
            currentCut.meropsID = line[0];
            currentCut.meropsName = line[1];
        }

        // if the line specfifies a cleavage site of the merops ID specified before
        else {
            currentCut.cleavageSite = line[0];

            findCutSites(currentCut, prot, fout);

            // Reset fin[]
        }
    }

    // Close all file descriptors
    finCSV.close();
    fin[0].close();
    fin[1].close();
    fout[0].close();
    fout[1].close();

    return 0;
}
