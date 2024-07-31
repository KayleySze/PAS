PAS PAH aggregation statistics
created by Kayley Sze
only for molecules that only contain hydrogen and carbon
only molecule can be used are phenalenyl, pyrene, olympicenyl, pentacene, nonacene, zethene17, kubo, triangulene, fluorenyl

input needs to be in .xyz format with C or H then the three coordinates separated by only spaces (default name is i.xyz)
for each molecule, all the carbons must be listed, then the hydrogens

another input file of the list of molecule must be included
(default name is list.txt and format is:
number molecule name
etc example- 5 pentacene)

if the ring detection function is not working properly, the molecules in a ring can be manaully inputted (default name is rings.txt)
each line represents one ring with no newlines needed between molecules
if rings.txt exists, the program will automatically use it and not use the ring detection function
  (the ring detection function uses relative distances between molecules so if rings are disfigured such as by sigma bonds, the rings will not be corrently found)

analysis is printed into the output.txt file
contains:
THRESHOLDS- py-py:3.4, ph-ph: 3.0556, ol-ol: 3.195918, pe-pe: 3.692, no-no: 3.418
  -The center of masses of all the rings are calculated. Between two molecules, the shortest distance between a ring of one molecule and an ring in the other molecule is calculated. If half of these values are shorter than the threshold, they are considered to be in a stack together.
  -prints out the molecules in each stack
  -prints the percentage of molecules out of the total that are in a stack with a certain number of molecules in said stack
a.exe can be ignored

each of the input file's names may be changed with command line arguments, example call
    ./a.exe i.xyz list.txt rings.txt 

*****MAKE SURE TO CHANGE THRESHOLD, RING.TXT, AND LIST.TXT INPUT FILE****
