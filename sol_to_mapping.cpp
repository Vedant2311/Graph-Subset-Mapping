// Author : rush_29
// complexity : O(VeVp)
#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <algorithm>
using namespace std;

#define forn(i,n) for(int i=0;i<n;i++)
#define forsn(i,s,n) for(int i=s;i<n;i++)

void getMapping(int phoneUserCount, int mailUserCount, string name) {

    ofstream output;
    output.open(name+".mapping");

    ifstream satOutputFile;
    satOutputFile.open(name+".satoutput");
    string sat; 
    satOutputFile >> sat;
    if(sat!="SAT") {
        output << 0;
        satOutputFile.close();
        return;
    } 

    bool satMatrix[mailUserCount+1][phoneUserCount+1];
    string input;
    forn(i,mailUserCount+1) {
        forn(j,phoneUserCount+1) {
            if(i==0||j==0) satMatrix[i][j]=false;
            else {
                satOutputFile >> input;
                if(input[0]=='-') satMatrix[i][j] = false;
                else satMatrix[i][j] = true;
            }
        }
    }

    satOutputFile.close();

    forsn(i,1,mailUserCount+1) {
        forsn(j,1,phoneUserCount+1) {
            if(satMatrix[i][j]) output << i << " " << j << "\n";
        }
    }
}

// -TODO- use ifstream ; try fast i/o
int main(int argc, char* argv[]) {
    
    int phoneUserCount, mailUserCount;
    
    string name;
    ifstream nameFile("filename");
    nameFile >> name;
    nameFile.close();

    ifstream sizeFile;
    sizeFile.open(name+".size");
    sizeFile >> phoneUserCount >> mailUserCount;
    sizeFile.close();

    
    getMapping(phoneUserCount, mailUserCount, name);

    return 0;
}