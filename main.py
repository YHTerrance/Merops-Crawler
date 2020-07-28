import requests
import pdb
from bs4 import BeautifulSoup
import pandas as pd
import os

from datetime import datetime 

AA_table = {
    "Ala": "A", "Arg": "R", "Asn": "N", "Asp": "D", "Cys": "C", "Gln": "Q",
    "Glu": "E", "Gly": "G", "His": "H", "Ile": "I", "Leu": "L", "Lys": "K",
    "Met": "M", "Phe": "F", "Pro": "P", "Pyl": "O", "Ser": "S", "Sec": "U",
    "Thr": "T", "Trp": "W", "Tyr": "Y", "Val": "V", "Asx": "B", "Glx": "Z",
    "Xaa": "X", "Xle": "J", "-" : "*"
}

def replace_AA(s, table):
    for k, v in iter(table.items()):
        s = s.replace(k, v)
    return s

replace_AA("Ala", AA_table)


path = os.getcwd()
filename = os.path.join(path,'merops.csv')    

if os.path.exists(filename):
    file = open(filename,"r+")
    file.truncate(0)
    file.close()

baseUrl = "https://www.ebi.ac.uk/merops/cgi-bin/peptidase_specificity"

r = requests.get(baseUrl)

webContent = r.text;

soup = BeautifulSoup(webContent, 'lxml')
table = soup.find_all('table')[0]

merops = []

iterrows = iter(table.find_all('tr'))
next(iterrows)
for row in iterrows:
    columns = row.find_all('td');    
    merops.append([columns[0].get_text(), columns[2].get_text()])

for merop in merops:
    url = "https://www.ebi.ac.uk/merops/cgi-bin/substrates?id=" + merop[0]
    r = requests.get(url)
    webContent2 = r.text
    soup = BeautifulSoup(webContent2, 'lxml')
    table = soup.find_all('table')[0]
    iterrows = iter(table.find_all('tr'))
    next(iterrows)
    cleavageSites = []
    Ps = []
    for row in iterrows:
        columns = row.find_all('td')
        cleavageSites.append(columns[3].get_text())
        P = [columns[6].get_text(), columns[7].get_text(), columns[8].get_text(), columns[9].get_text(), columns[10].get_text(), columns[11].get_text(), columns[12].get_text(), columns[13].get_text()]
        for i in range(len(P)):
            P[i] = replace_AA(P[i], AA_table)
        Ps.append(P)
    
    df = pd.DataFrame(
    {"cleavageSites" : cleavageSites,
    "P4~P4'" : Ps,
    })
    df.head()
    f = open(filename, "a")
    f.write(merop[0])
    f.close()
    df.to_csv(filename, mode = 'a')



