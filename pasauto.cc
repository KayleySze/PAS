//PAS PAH aggregation statistics
//created by Kayley Sze
//only for molecules that only contain hydrogen and carbon
//only molecule can be used are phenalenyl, pyrene, olympicenyl, pentacene, nonacene, zethene17, kubo, triangulene, fluorenyl

//input needs to be in .xyz format with C or H then the three coordinates separated by only spaces (default name is i.xyz)
//for each molecule, all the carbons must be listed, then the hydrogens

//another input file of the list of molecule must be included
//(default name is list.txt and format is:
//number molecule name
//etc example- 5 pentacene)

//if the ring detection function is not working properly, the molecules in a ring can be manaully inputted (default name is rings.txt)
//each line represents one ring with no newlines needed between molecules
//if rings.txt exists, the program will automatically use it and not use the ring detection function

//analysis is printed into the output.txt file
//contains:
//THRESHOLDS- py-py:3.4, ph-ph: 3.0556, ol-ol: 3.195918, pe-pe: 3.692, no-no: 3.418
//  -The center of masses of all the rings are calculated. Between two molecules, the shortest distance between a ring of one molecule and an ring in the other molecule is calculated. If half of these values are shorter than the threshold, they are considered to be in a stack together.
//  -prints out the molecules in each stack
//  -prints the percentage of molecules out of the total that are in a stack with a certain number of molecules in said stack
//a.exe can be ignored

//each of the input file's names may be changed with command line arguments, example call
//    ./a.exe i.xyz list.txt rings.txt

//*****MAKE SURE TO CHANGE THRESHOLD, RING.TXT, AND LIST.TXT INPUT FILE****

#include <bits/stdc++.h>
#include <vector>
#include <cmath>
#include <cstdio>
#include <string>
#include <sstream>
#include <string>
#include <iostream>

using namespace std;

char temp;
float tempx, tempy, tempz;
ofstream out("output.txt");
ofstream comout("centofmassdist.txt");
ofstream paired("pairdistances.txt");
ofstream rfound("ringsfound.txt");
vector<vector<vector<float>>> centerofmass;
vector<vector<vector<float>>> coor;
vector<vector<int>> stacks;
vector<vector<int>> type;
vector<vector<float>> c;
vector<vector<vector<int>>> rings;
float threshold=3.4;

void readin(int argc, char *argv[]){
    string name;
    if (argc <= 1){name="i.xyz";}
    else{name=argv[1];}
    ifstream in(name);
    string l;
    if (argc <= 2){l="list.txt";}
    else{l=argv[2];}
    ifstream lists(l);
	float x=0, y=0, z=0, mass=0;
	int number;
	string molecule;
	lists >> number >> molecule;
	bool go=true;
	string str;
	getline(in,str);
	getline(in,str);
	while(go){
		int times;
		if(molecule=="pyrene")times=26;
		if(molecule=="phenalenyl")times=22;
		if(molecule=="olympicenyl")times=30;
		if(molecule=="pentacene")times=36;
		if(molecule=="nonacene")times=60;
		if(molecule=="zethene17")times=44;
		if(molecule=="kubo")times=48;
		if(molecule=="triangulene")times=34;
		if(molecule=="fluorenyl")times=22;
		for(int i=0; i<number; i++){
			vector<vector<float>> moleculetemp;
			vector<int> temptype;
			for(int j=0; j<times; j++){
				in >> temp >> tempx >> tempy >> tempz;
				if(temp=='C') temptype.push_back(0);
				if(temp=='H') temptype.push_back(1);
				vector<float> vtemp;
				vtemp.push_back(tempx);
				vtemp.push_back(tempy);
				vtemp.push_back(tempz);
				moleculetemp.push_back(vtemp);
			}
			type.push_back(temptype);
			coor.push_back(moleculetemp);
		}
		number=-1;
		lists >> number >> molecule;
		if(number==-1) go=false;
	}
}

void readrings(int argc, char *argv[]){
    string name;
    if (argc <= 3){name="rings.txt";}
    else{name=argv[3];}
    ifstream readring(name);
	for(int i=0; i<coor.size(); i++){
		vector<vector<int>> tempring;
		int r;
		if(coor[i].size()==26)r=4;
		if(coor[i].size()==22)r=3;
		if(coor[i].size()==30)r=5;
		if(coor[i].size()==36)r=5;
		if(coor[i].size()==60)r=9;
		if(coor[i].size()==44)r=7;
		if(coor[i].size()==48)r=9;
		if(coor[i].size()==34)r=6;
		if(coor[i].size()==22)r=3;
		for(int j=0; j<r; j++){
			vector<int> tempmol;
			int tempr;
			string str;
			getline(readring,str);
			stringstream ss(str);
			while(ss>>tempr) tempmol.push_back(tempr);
			tempring.push_back(tempmol);
		}
		rings.push_back(tempring);
	}
}

vector<int> neighbor(int start){
	vector<int> ans;
	for(int j=0; j<c.size(); j++){
		if(j!=start){
			float distance=(c[start][1]-c[j][1])*(c[start][1]-c[j][1]);
			distance+=(c[start][2]-c[j][2])*(c[start][2]-c[j][2]);
			distance+=(c[start][3]-c[j][3])*(c[start][3]-c[j][3]);
			distance=sqrt(distance);
			if(distance<1.52) ans.push_back(j);
		}
	}
	return ans;
}

void ringdetect(){
	for(int i=0; i<coor.size(); i++){
		vector<vector<int>> molering;
		for(int j=0; j<coor[i].size(); j++){
			if(type[i][j]==0){
				vector<float> tempcoor;
				tempcoor.push_back(j);
				tempcoor.push_back(coor[i][j][0]);
				tempcoor.push_back(coor[i][j][1]);
				tempcoor.push_back(coor[i][j][2]);
				c.push_back(tempcoor);
			}
		}
		while(c.size()>0){
			int start=0;
			vector<int> count=neighbor(start);
			while(count.size()!=2){
				start++;
				count=neighbor(start);
			}
			count.push_back(start);
			for(int j=0; j<c.size(); j++){
				if(j!=count[0] && j!=count[1] && j!=count[2]){
					float distance1=(c[count[2]][1]-c[j][1])*(c[count[2]][1]-c[j][1]);
					distance1+=(c[count[2]][2]-c[j][2])*(c[count[2]][2]-c[j][2]);
					distance1+=(c[count[2]][3]-c[j][3])*(c[count[2]][3]-c[j][3]);
					distance1=sqrt(distance1);
					float distance2=(c[count[0]][1]-c[j][1])*(c[count[0]][1]-c[j][1]);
					distance2+=(c[count[0]][2]-c[j][2])*(c[count[0]][2]-c[j][2]);
					distance2+=(c[count[0]][3]-c[j][3])*(c[count[0]][3]-c[j][3]);
					distance2=sqrt(distance2);
					float distance3=(c[count[1]][1]-c[j][1])*(c[count[1]][1]-c[j][1]);
					distance3+=(c[count[1]][2]-c[j][2])*(c[count[1]][2]-c[j][2]);
					distance3+=(c[count[1]][3]-c[j][3])*(c[count[1]][3]-c[j][3]);
					distance3=sqrt(distance3);
					if(distance1<2.52 && ((distance2<1.52 && distance3<2.92) || (distance2<2.92 && distance3<1.52))) count.push_back(j);
				}
			}
			for(int j=0; j<c.size(); j++){
				if(j!=count[0] && j!=count[1] && j!=count[2] && j!=count[3] && j!=count[4]){
					float distance1=(c[count[3]][1]-c[j][1])*(c[count[3]][1]-c[j][1]);
					distance1+=(c[count[3]][2]-c[j][2])*(c[count[3]][2]-c[j][2]);
					distance1+=(c[count[3]][3]-c[j][3])*(c[count[3]][3]-c[j][3]);
					distance1=sqrt(distance1);
					float distance2=(c[count[4]][1]-c[j][1])*(c[count[4]][1]-c[j][1]);
					distance2+=(c[count[4]][2]-c[j][2])*(c[count[4]][2]-c[j][2]);
					distance2+=(c[count[4]][3]-c[j][3])*(c[count[4]][3]-c[j][3]);
					distance2=sqrt(distance2);
					float distance3=(c[count[2]][1]-c[j][1])*(c[count[2]][1]-c[j][1]);
					distance3+=(c[count[2]][2]-c[j][2])*(c[count[2]][2]-c[j][2]);
					distance3+=(c[count[2]][3]-c[j][3])*(c[count[2]][3]-c[j][3]);
					distance3=sqrt(distance3);
					if(distance1<1.52 && distance2<1.52 && distance3<2.92) count.push_back(j);
				}
			}
			vector<int> tempring;
			for(int j=0; j<count.size(); j++) tempring.push_back(c[count[j]][0]);
			sort(tempring.begin(), tempring.end());
			if (tempring.size()!=0) molering.push_back(tempring);
			vector<int> toerase;
			for(int j=0; j<count.size(); j++){
				vector<int> tempne=neighbor(count[j]);
				if(tempne.size()==2) toerase.push_back(count[j]);
			}
			sort(toerase.begin(), toerase.end());
			for(int j=toerase.size()-1; j>=0; j--) c.erase(c.begin()+toerase[j]);
		}
		rings.push_back(molering);
	}
}

void getcenterofmass(){
	for(int i=0; i<rings.size(); i++){
		vector<vector<float>> ringtemp;
		for(int j=0; j<rings[i].size(); j++){
			tempx=0;
			tempy=0;
			tempz=0;
			int number=0;
			vector<float> vtemp;
			for(int k=0; k<rings[i][j].size(); k++){
				tempx+=coor[i][rings[i][j][k]][0];
				tempy+=coor[i][rings[i][j][k]][1];
				tempz+=coor[i][rings[i][j][k]][2];
				number++;
			}
			vtemp.push_back(tempx/number);
			vtemp.push_back(tempy/number);
			vtemp.push_back(tempz/number);
			ringtemp.push_back(vtemp);
		}
		centerofmass.push_back(ringtemp);
	}
}


void makestack(){
	for(int i=0; i<centerofmass.size(); i++){
		for(int j=i+1; j<centerofmass.size(); j++){
			for(int k=0; k<centerofmass[i].size(); k++){
				for(int l=0; l<centerofmass[j].size(); l++){
					float distance=(centerofmass[i][k][0]-centerofmass[j][l][0])*(centerofmass[i][k][0]-centerofmass[j][l][0]);
					distance+=(centerofmass[i][k][1]-centerofmass[j][l][1])*(centerofmass[i][k][1]-centerofmass[j][l][1]);
					distance+=(centerofmass[i][k][2]-centerofmass[j][l][2])*(centerofmass[i][k][2]-centerofmass[j][l][2]);
					distance=sqrt(distance);
					comout << i << " " << k << " " << j << " " << l << " " << distance << "\n";
				}
			}
		}
	}
	for(int i=0; i<centerofmass.size(); i++){
		for(int j=i+1; j<centerofmass.size(); j++){
			paired << i << j << ": \n";
			int count1=0;
			vector<int> used1;
			for(int k=0; k<centerofmass[i].size(); k++){
				float lowest=100;
				int tempused;
				for(int l=0; l<centerofmass[j].size(); l++){
					bool ifused=false;
					for(int m=0; m<used1.size(); m++){
						if(l==used1[m]) ifused=true;
					}
					if(ifused==false){
						float distance=(centerofmass[i][k][0]-centerofmass[j][l][0])*(centerofmass[i][k][0]-centerofmass[j][l][0]);
						distance+=(centerofmass[i][k][1]-centerofmass[j][l][1])*(centerofmass[i][k][1]-centerofmass[j][l][1]);
						distance+=(centerofmass[i][k][2]-centerofmass[j][l][2])*(centerofmass[i][k][2]-centerofmass[j][l][2]);
						distance=sqrt(distance);
						if(distance<lowest){
							lowest=distance;
							tempused=l;
						}
					}
				}
				used1.push_back(tempused);
				if(lowest<threshold){
					count1++;
				}
				paired << k << " " << tempused << "-" << lowest << "\n";
			}
			paired << j << i << ": \n";
			int count2=0;
			vector<int> used2;
			for(int k=0; k<centerofmass[j].size(); k++){
				float lowest=100;
				int tempused;
				for(int l=0; l<centerofmass[i].size(); l++){
					bool ifused=false;
					for(int m=0; m<used2.size(); m++){
						if(l==used2[m]) ifused=true;
					}
					if(ifused==false){
						float distance=(centerofmass[j][k][0]-centerofmass[i][l][0])*(centerofmass[j][k][0]-centerofmass[i][l][0]);
						distance+=(centerofmass[j][k][1]-centerofmass[i][l][1])*(centerofmass[j][k][1]-centerofmass[i][l][1]);
						distance+=(centerofmass[j][k][2]-centerofmass[i][l][2])*(centerofmass[j][k][2]-centerofmass[i][l][2]);
						distance=sqrt(distance);
						if(distance<lowest){
							lowest=distance;
							tempused=l;
						}
					}
				}
				used2.push_back(tempused);
				if(lowest<threshold){
					count2++;
				}
				paired << k << " " << tempused << "-" << lowest << "\n";
			}
			paired << count1 << " "<< count2 << " ";
			float count=max(count1, count2);
			paired << count << " ";
			paired << centerofmass[i].size()/2.0 << " " << centerofmass[j].size()/2.0 << "\n\n";
			if(count>=centerofmass[i].size()/2.0 || count>=centerofmass[j].size()/2.0){
				//cout << "Y\n";
				int addi=-1;
				int addj=-1;
				for(int k=0; k<stacks.size(); k++){
					for(int l=0; l<stacks[k].size(); l++){
						if(stacks[k][l]==i){
							addj=k;
						}
						if(stacks[k][l]==j){
							addi=k;
						}
					}
				}
				if(addi==-1 && addj==-1){
					vector<int> tempstack;
					tempstack.push_back(i);
					tempstack.push_back(j);
					stacks.push_back(tempstack);
				}
				if(addi!=-1)
					stacks[addi].push_back(i);
				if(addj!=-1)
					stacks[addj].push_back(j);

			}
		}
	}
}

void checkstack(){
	for(int i=0; i<stacks.size(); i++){
		for(int j=i+1; j<stacks.size(); j++){
			bool same=false;
			for(int k=0; k<stacks[i].size(); k++){
				for(int l=0; l<stacks[j].size(); l++){
					if(stacks[i][k]==stacks[j][l]) same=true;
				}
			}
			if(same){
				for(int l=0; l<stacks[j].size(); l++){
					stacks[i].push_back(stacks[j][l]);
				}
				vector<int> toerase;
				stacks.erase(stacks.begin()+j);
				for(int k=0; k<stacks[i].size(); k++){
					for(int l=k+1; l<stacks[i].size(); l++){
						if(stacks[i][k]==stacks[i][l]) toerase.push_back(l);
					}
				}
				sort(toerase.begin(), toerase.end());
				for(int k=toerase.size()-1; k>=0; k--){
					stacks[i].erase(stacks[i].begin()+toerase[k]);
				}
				j--;
			}
		}
	}
}
void printout(){
	out << "stacks:\n";
	for(int i=0; i<stacks.size(); i++){
		sort(stacks[i].begin(), stacks[i].end());
		for(int j=0; j<stacks[i].size(); j++){
			out << stacks[i][j];
		}
		out << "\n";
	}
	int tempsize[coor.size()];
	for(int i=0; i<coor.size(); i++){
		tempsize[i]=0;
	}
	for(int i=0; i<stacks.size(); i++){
		tempsize[stacks[i].size()-1]+=stacks[i].size();
	}
	for(int i=0; i<coor.size(); i++){
		if(tempsize[i]!=0){
			float temp;
			temp=static_cast<float>(tempsize[i])/coor.size();
			out << "\npercentage of PAH in " << i+1 << " stack: " << temp;
		}
	}
}
void sigma(){
	out << "\nsigma bonds:\n";
	for(int i=0; i<coor.size(); i++){
		for(int j=i+1; j<coor.size(); j++){
			for(int k=0; k<coor[i].size(); k++){
				for(int l=0; l<coor[j].size(); l++){
					if(type[i][k]==0 && type[j][l]==0){
						float distance=(coor[i][k][0]-coor[j][l][0])*(coor[i][k][0]-coor[j][l][0]);
						distance+=(coor[i][k][1]-coor[j][l][1])*(coor[i][k][1]-coor[j][l][1]);
						distance+=(coor[i][k][2]-coor[j][l][2])*(coor[i][k][2]-coor[j][l][2]);
						distance=sqrt(distance);
						if(distance<=1.63)
							out << i << " " << j << " " << "\n";
					}
				}
			}
		}
	}
}

int main(int argc, char *argv []){
	readin(argc, argv);
	ifstream file;
    if (argc <= 3){file.open("rings.txt");}
    else{file.open(argv[3]);}
	if(file) readrings(argc, argv);
	else ringdetect();
	getcenterofmass();
	comout << "distances between each two ring center of masses where the rings are in different molecules\n";
	comout << "molecule 1 index, corresponding ring index, molecule index 2, coresponding ring index, distance between their centers of masses\n";
	comout << "example: 0 1 2 3 4.5 means the distance between the first ring of molecule 0 and the third ring of molecule 2 are 4.5 apart\n";
	paired << "the two numbers before the : are the two molecules the distance is between and the order\nthe next few rows are the rings used and their distance\nthe next line is the counts:\nthe first value is the number of distances below the threshold of the first pairing and the second is the number for the second pairing\nthe third number is the higher value of the two and is the value used\nfourth and fifth is the threshold to be considered in a stack (the first value is for first molecule and the next for the second molecule)\n";
	makestack();
	rfound << ("each paragraph is a molecule, each line it is the molecules in that ring\n");
	for(int i=0; i<rings.size(); i++){
		for(int j=0; j<rings[i].size(); j++){
			for(int k=0; k<rings[i][j].size(); k++) rfound << rings[i][j][k] << " ";
			rfound << "\n";
		}
		rfound << "\n";
	}
	checkstack();
	printout();
	sigma();
}
