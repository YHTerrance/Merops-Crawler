import requests
import pdb
from bs4 import BeautifulSoup
import pandas as pd
import os

from datetime import datetime

# Amino Acid dictionary that maps protein to its abbreviation
AA_table = {
    "Ala": "A", "Arg": "R", "Asn": "N", "Asp": "D", "Cys": "C", "Gln": "Q",
    "Glu": "E", "Gly": "G", "His": "H", "Ile": "I", "Leu": "L", "Lys": "K",
    "Met": "M", "Phe": "F", "Pro": "P", "Pyl": "O", "Ser": "S", "Sec": "U",
    "Thr": "T", "Trp": "W", "Tyr": "Y", "Val": "V", "Asx": "B", "Glx": "Z",
    "Xaa": "X", "Xle": "J", "-" : "*"
}

def replace_AA(s, table):
    # Replace amino acid with its abbr.
    for k, v in iter(table.items()):
        s = s.replace(k, v)

    # Replace with * if it is an unknown AA
    if len(s) > 1:
        s = "*"
    return s

baseUrl = "https://www.ebi.ac.uk/merops/cgi-bin/peptidase_specificity"

r = requests.get(baseUrl)

webContent = r.text;

soup = BeautifulSoup(webContent, 'lxml')
table = soup.find_all('table')[0]

merops = []

# Find all rows
iterrows = iter(table.find_all('tr'))
next(iterrows)
for row in iterrows:
    columns = row.find_all('td');
    merops.append([columns[0].get_text(), columns[1].get_text()])

# For every different merop ID
for merop in merops:

    url = "https://www.ebi.ac.uk/merops/cgi-bin/substrates?id=" + merop[0]

    r = requests.get(url)

    webContent2 = r.text

    soup = BeautifulSoup(webContent2, 'lxml')
    table = soup.find_all('table')[0]
    iterrows = iter(table.find_all('tr'))
    next(iterrows)

    # Write id and name
    print(f"{merop[0]},{merop[1]}")

    # Iterate through the rows
    for row in iterrows:
        columns = row.find_all('td')
        cleavageSite = columns[3].get_text()

        # Store the line that will be written
        line = cleavageSite + ','

        # Append the cut site residues
        for i in range(6, 14):
            replaced = replace_AA(str(columns[i].get_text()), AA_table)
            line += f"{replaced}"

        print(line)

