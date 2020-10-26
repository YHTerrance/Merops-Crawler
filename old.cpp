#include <cstring>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <regex>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

vector<string> getNextLineAndSplitIntoTokens(istream& str)
{
    vector<string> result;
    string line;
    getline(str,line);

    stringstream lineStream(line);
    string cell;

    while(getline(lineStream, cell, ','))
    {
        result.push_back(cell);
    }
    // This checks for a trailing comma with no data after it.
    if (!lineStream && cell.empty())
    {
        // If there was a trailing comma then add an empty element.
        result.push_back("");
    }
    return result;
}

void findCutSites(string& meropsID, string& cleavageSite, string& cutSequence, ofstream& foutSpike, ofstream &foutAce2)
{
    ifstream fin1, fin2;
    fin1.open("spSequence");
    fin2.open("ace2Sequence");
    if(!fin1)
        cout << "Can not open spSequence" << endl;
    if(!fin2)
        cout << "Can not open ace2Sequence" << endl;

    string ace2Sequence, spSequence, input;

    while(getline(fin1, input))
        spSequence += input;
    int spLen = spSequence.length();

    while(getline(fin2, input))
        ace2Sequence += input;
    int ace2Len = ace2Sequence.length();

    if(cutSequence.find("!") != string::npos || cutSequence == "********")
        return;

    while(cutSequence.find("*") != string::npos)
    {
        int index = cutSequence.find("*");
        cutSequence.replace(index, 1, "[A-Z]");
    }

    #ifdef DEBUG
    cout << "cutSequence " << cutSequence << endl;
    #endif

    regex e(cutSequence.c_str());
    smatch res;

    string::const_iterator searchStart( spSequence.cbegin() );
    bool found1 = false;
    while (regex_search( searchStart, spSequence.cend(), res, e ))
    {
        if(!found1)
        {
            cout << "========================" << endl;
            cout << "MeropsID: " << meropsID << endl;
            cout << "CleavageSite: " << cleavageSite << endl;
            cout << "CutSequence: " << cutSequence << endl;
            cout << "Found in the Spike protein sequence: " << endl;
        }

        found1 = true;
        int start = spSequence.find(res[0].str());
        cout << "from " << setw(5) << start + 1 << " to " <<  setw(5) << start + res[0].str().length();
        cout << setw(12) << res[0] << endl;
        searchStart = res.suffix().first - 7;
    }
    if(found1)
        cout << "========================" << endl;

    searchStart = ace2Sequence.cbegin();
    bool found2 = false;
    while (regex_search( searchStart, ace2Sequence.cend(), res, e ))
    {
        if(!found2)
        {
            cout << "========================" << endl;
            cout << "MeropsID: " << meropsID << endl;
            cout << "CleavageSite: " << cleavageSite << endl;
            cout << "CutSequence: " << cutSequence << endl;
            cout << "Found in the Ace2 protein sequence: " << endl;
        }

        found2 = true;
        int start = ace2Sequence.find(res[0].str());
        cout << "from " << setw(5) << start + 1 << " to " <<  setw(5) << start + res[0].str().length();
        cout << setw(12) << res[0] << endl;
        searchStart = res.suffix().first - 7;
    }
    if(found2)
        cout << "========================" << endl;

    if(found1 && !found2)
        foutSpike << meropsID << " " << cleavageSite << endl;
    if(!found1 && found2)
        foutAce2 << meropsID << " " << cleavageSite << endl;
    if(found1 || found2)
        cout << endl;
    fin1.close();
    fin2.close();
}



int main()
{
    string seq1;

    ifstream finCSV;
    ofstream foutSpike, foutAce2;
    finCSV.open("merops.csv");
    foutSpike.open("OnlySpike.txt");
    foutAce2.open("OnlyAce2.txt");

    if(!finCSV)
        cout << "Can not open CSV" << endl;
    string meropsID, cleavageSite;

    vector<string> line;
    line = getNextLineAndSplitIntoTokens(finCSV);
    int n = 200000;
    while(n-- && line.size() >= 3)
    {
        if(line.size() == 3){
            meropsID = line[0];
        }
        else
        {
            cleavageSite = line[1];
            string cutSequence, delimeters = "\"[\'] ";

            for(int i = 2; i < line.size(); i++){
                for(char& c : delimeters)
                {
                    line[i].erase(remove(line[i].begin(), line[i].end(), c), line[i].end());
                }
                if(line[i].size() > 1)
                    line[i] = "!";
                cutSequence += line[i];
            }
            findCutSites(meropsID, cleavageSite, cutSequence, foutSpike, foutAce2);
        }
        line = getNextLineAndSplitIntoTokens(finCSV);
    }
    finCSV.close();
    foutSpike.close();
    foutAce2.close();
    return 0;
}
