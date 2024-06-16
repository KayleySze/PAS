# PAS
PAS: PAH aggregation statistics
created by Kayley Sze in C++
only for molecules that only contain hydrogen and carbon
only molecule can be used are phenalenyl, pyrene, olympicenyl, pentacene, nonacene, zethene17, kubo, triangulene, fluorenyl
input needs to be in .xyz format with C or H then the three coordinates separated by only spaces (default name is i.xyz, can be changed in line 28)
another input file of the list of molecule must be included (default name is list.txt and format is:
number name
  example- 5 pentacene
each different type of molecule should be in its own line)
analysis is printed into the output.txt file
contains:
THRESHOLDS- py-py:3.4, ph-ph: 3.0556, ol-ol: 3.195918, pe-pe: 3.692, no-no: 3.418
  -The center of masses of all the rings are calculated. Between two molecules, the shortest distance between a ring of one molecule and an ring in the other molecule is calculated. If half of these values are shorter than the threshold, they are considered to be in a stack together.
  -prints out the molecules in each stack 
  -prints the percentage of molecules out of the total that are in a stack with a certain number of molecules in said stack
  a.exe can be ignored

*****MAKE SURE TO CHANGE THRESHOLD, RING.TXT, AND LIST.TXT INPUT FILE****
