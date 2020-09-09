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

    ifstream fin;
    fin.open("Peptidases.txt");

    ofstream fout;
    fout.open("CleavageSiteCut.txt");

    if(!fin) {
        cout << "Cannot open Peptidases.txt" << endl;
        return -1;
    }

    unordered_map<string, pii> peptidaseCut;
    unordered_map<string, pii> cleavageSiteCut;

    string input;
    string meropsID, cleavageSite;
    int count = 0;
    bool isAce2;
    while(fin >> input) {
        //cout << input << endl;
        if(input == "========================") {
            if(count != 0) {
                if(isAce2)
                    cleavageSiteCut[meropsID + " " + cleavageSite].first += count;
                else
                    cleavageSiteCut[meropsID + " " + cleavageSite].second += count;
            }
            count = 0;
        }
        else if(input == "MeropsID:") {
            fin >> input;
            meropsID = input;
            //cout << "meropsID is" << meropsID << endl;
        }
        else if(input == "CleavageSite:") {
            fin >> input;
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

    for(auto it = cleavageSiteCut.begin(); it != cleavageSiteCut.end(); it++) {
        fout << it -> first << " " << (it -> second).first << " " << (it -> second).second << endl;
    }

    fin.close();
    fout.close();

    fin.open("Peptidases.txt");
    fout.open("PeptidaseCut.txt");
    count = 0;
    while(fin >> input) {
        //cout << input << endl;
        if(input == "========================") {
            if(count != 0) {
                if(isAce2)
                    peptidaseCut[meropsID].first += count;
                else
                    peptidaseCut[meropsID].second += count;
            }
            count = 0;
        }
        else if(input == "MeropsID:") {
            fin >> input;
            meropsID = input;
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

    for(auto it = peptidaseCut.begin(); it != peptidaseCut.end(); it++) {
        fout << it -> first << " " << (it -> second).first << " " << (it -> second).second << endl;
    }

    fin.close();
    fout.close();

    return 0;
}
