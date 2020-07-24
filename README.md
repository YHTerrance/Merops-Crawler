# Merops-Crawler

## Brief Introduction

This crawler project collects data from [MEROPS](https://www.ebi.ac.uk/merops/index.shtml) and then organizes the peptidases that are able to cut the Spike protein or the Ace2 of nCoV-19.

### main.py

The main crawler program that crawls data from MEROPS and then stores it in merops.csv using Python 3.7.7, BeautifulSoup, and Pandas.

### main.cpp

The main program that sifts through merops.csv to find the peptidase that can cut the Ace2 protein or the Spike protein, the result is outputted in standard output, which we directed to Peptidases.txt.

### OnlySpike.txt

A list of all the meropIDs and cleavage sites of peptidases that cut only the Spike protein but not the Ace2 protein.

### OnlyAce2.txt

A list of all the meropIDs and cleavage sites of peptidases that cut only the Ace2 protein but not the Spike protein.

