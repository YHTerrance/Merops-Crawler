# Merops-Crawler

## Brief Introduction

This repository is about a project that collects data from [MEROPS](https://www.ebi.ac.uk/merops/index.shtml) (a worldwide famous database of all proteases) and stores it in a file (merops.csv). Then, it can take in two protein sequences and output the cut site with respect to different proteases accordingly. Moreover, it can also help you identify proteases that cut only the first sequence and proteases that cut only the second sequence, which is of great help when you are trying to select proteases that cut a specific protein put not another.

## Developer

NYMU iGEM Team 2020 - Off the Crown

## Code Explanation

### main.py

The main crawler program that crawls data from MEROPS and then stores it in merops.csv using Requests and BeautifulSoup4.

### main.cpp

The main program that sifts through the merops.csv data file and outputs the cut site on respective proteins for different proteases.

### \*.result files

Files with this file format are text files that store the proteases that cut only the specific protein but not the other. The Merops ID and name of the protease are both recorded for user reference.
