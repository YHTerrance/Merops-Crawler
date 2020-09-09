#include <cstring>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <regex>
#include <vector>
#include <sstream>
#include <iomanip>
#include <unordered_map>

using namespace std;

typedef pair<int, int> pii;
typedef pair<string, string> pss;

int main() {

    ifstream finPeptidases;
    finPeptidases.open("Peptidases.txt");

    ofstream foutPeptidaseCut;
    foutPeptidaseCut.open("PeptidaseCut.txt");

    if(!finPeptidases) {
        cout << "Cannot open Peptidases.txt" << endl;
        return -1;
    }


    unordered_map<string, pii> peptidaseToCut;
    string input;
    string meropsID, cleavageSite;
    int count = 0;
    bool isAce2;
    while(finPeptidases >> input) {
        //cout << input << endl;
        if(input == "========================") {
            if(count != 0) {
                if(isAce2)
                    peptidaseToCut[meropsID + " " + cleavageSite].first = count;
                else
                    peptidaseToCut[meropsID + " " + cleavageSite].second = count;
            }
            count = 0;
        }
        else if(input == "MeropsID:") {
            finPeptidases >> input;
            meropsID = input;
            //cout << "meropsID is" << meropsID << endl;
        }
        else if(input == "CleavageSite:") {
            finPeptidases >> input;
            cleavageSite = input;
        }
        else if(input == "Ace2") {
            isAce2 = true;
        }
        else if(input == "Spike") {
            isAce2 = false;
        }
        else if(input == "from") {
            count++;
        }
    }

    for(auto it = peptidaseToCut.begin(); it != peptidaseToCut.end(); it++) {
        foutPeptidaseCut << it -> first << " " << (it -> second).first << " " << (it -> second).second << endl;
    }

    return 0;
}
