// complexity : O(Em + Ep + Vp + Vm + Vm*Vp + Ve + Em*Vp*Vp + Vp*Vp*Vm + Vm*Vm*Vp) = O(Vp*Vp*(Vm + Em))
#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <algorithm>
using namespace std;

#define forn(i,n) for(long long i=0; i<n; i++)
#define forsn(i,s,n) for(long long i=s; i<n; i++)
bool phoneCallMatrix[1005][1005];
bool mailSentMatrix[1005][1005];
#define adjList vector<vector<long long>>

void getCNF(string fileName) {

    // GETS INPUT

    long long fromUser, toUser, phoneCallCount, mailSentCount, phoneUserCount, mailUserCount, maximum=0, negativeLiteralCount=0;
    vector<long long> fromPhoneList, toPhoneList, fromMailList, toMailList;
    
    ifstream inputFile(fileName+".graphs");
    if (!inputFile) {
        cerr << "Unable to open file";
        exit(1);
    }

    // gets phone graph (G') edge wise
    inputFile >> fromUser >> toUser;
    cerr << fromUser << " " << toUser << "\n";
    while(!(fromUser == 0 && toUser == 0)) {
        fromPhoneList.push_back(fromUser);
        toPhoneList.push_back(toUser);
        maximum = max(fromUser,maximum);
        maximum = max(toUser,maximum);
        inputFile >> fromUser >> toUser;
        cerr << fromUser << " " << toUser << "\n";
    }
    // creates a adj list with highest node value size for phone
    phoneUserCount = maximum;
    adjList graphPhone(phoneUserCount+1);
    phoneCallCount = fromPhoneList.size();
    forn(i,phoneCallCount) {
        fromUser = fromPhoneList[i], toUser = toPhoneList[i];
        graphPhone[fromUser].push_back(toUser);
    }

    maximum=0;
    
    // gets mail graph (G) edge wise
    while(inputFile >> fromUser >> toUser) {
        cerr << fromUser << " " << toUser << "\n";
        fromMailList.push_back(fromUser);
        toMailList.push_back(toUser);
        maximum = max(fromUser,maximum);
        maximum = max(toUser,maximum);
    }
    inputFile.close();
    // creates an adj List with highest node value size for mail
    mailUserCount = maximum;
    adjList graphMail(mailUserCount+1);
    mailSentCount = fromMailList.size();

    forn(i,mailSentCount) {
        fromUser = fromMailList[i], toUser = toMailList[i];
        graphMail[fromUser].push_back(toUser);
    }
    cerr << mailUserCount << " " << mailSentCount << " " << phoneUserCount << " " << phoneCallCount << "\n";
    
    // INDEGREE AND OUTDEGREE MEASUREMENT

    long long inDegreePhone[phoneUserCount+1], outDegreePhone[phoneUserCount+1], inDegreeMail[mailUserCount+1], outDegreeMail[mailUserCount+1];
    
    forn(i, phoneUserCount+1) {
        outDegreePhone[i] = 0;
        inDegreePhone[i] = 0;
    }
    forn(i, mailUserCount+1) {
        outDegreeMail[i] = 0;
        inDegreeMail[i] = 0;
    }

    forn(i,phoneCallCount) {
        outDegreePhone[fromPhoneList[i]]++;
        inDegreePhone[toPhoneList[i]]++;
    }
    forn(i,mailSentCount) {
        outDegreeMail[fromMailList[i]]++;
        inDegreeMail[toMailList[i]]++;
    }

    // rows - mail users, cols - phone users, true means that jth can be mapped to ith
    bool satMatrix[mailUserCount+1][phoneUserCount+1];
    forn(i,mailUserCount+1) {
        forn(j,phoneUserCount+1) {
            if(i==0||j==0) satMatrix[i][j] = false;
            else if((inDegreeMail[i]<=inDegreePhone[j]) && (outDegreeMail[i]<=outDegreePhone[j])) satMatrix[i][j] = true;
            else {
                satMatrix[i][j] = false;
                negativeLiteralCount++;
            }
        }
    }

    ofstream outfile(fileName+".size");
    outfile << phoneUserCount << " " << mailUserCount;
    
    // SAT INPUT GENERATION : GENERATES OUTPUT

    FILE *satOutputFile;
    string name = fileName+".satinput";
    char nameOfFile[name.length()];
    forn(i,name.length()) nameOfFile[i]=name[i];
    satOutputFile = fopen(nameOfFile,"w");
    long long numberofVariables = phoneUserCount*mailUserCount;
    long long numberOfClauses = negativeLiteralCount + mailUserCount + mailSentCount*(phoneUserCount*(phoneUserCount-1)-phoneCallCount) + phoneCallCount*(mailUserCount*(mailUserCount-1)-mailSentCount) + mailUserCount*phoneUserCount*(phoneUserCount+mailUserCount-2);
    fprintf(satOutputFile,"p cnf %lld %lld\n",numberofVariables,4);
    
    // unary clauses // make a count
    forsn(i,1,mailUserCount+1) {
        forsn(j,1,phoneUserCount+1) {
            if(satMatrix[i][j]==false) fprintf(satOutputFile,"-%lld 0\n",(i-1)*phoneUserCount+j);
        }
    }

    // row clauses
    forsn(i,1,mailUserCount+1) {
        forsn(j,1,phoneUserCount+1) {
                fprintf(satOutputFile,"%lld ",(i-1)*phoneUserCount+j);
            }
        fprintf(satOutputFile,"%d\n",0);
    }
    
    // getting edgeConsistencyClauses
    forn(i,phoneUserCount+1) {
        forn(j,phoneUserCount+1) {
            phoneCallMatrix[i][j]=false;
        }
    }
    forn(i,mailUserCount+1) {
        forn(j,mailUserCount+1) {
            mailSentMatrix[i][j]=false;
        }
    }
    forn(i,phoneCallCount) phoneCallMatrix[fromPhoneList[i]][toPhoneList[i]]=true;
    forn(i,mailSentCount) mailSentMatrix[fromMailList[i]][toMailList[i]]=true;

    forn(i,mailSentCount) {
        forsn(j,1,phoneUserCount+1) {
            forsn(k,1,phoneUserCount+1) {
                if(j==k) continue;
                if(!phoneCallMatrix[j][k]) {
                    fprintf(satOutputFile,"-%lld -%lld 0\n",(fromMailList[i]-1)*phoneUserCount+j,(toMailList[i]-1)*phoneUserCount+k);
                }
            }
        }
    }

    forn(i,phoneCallCount) {
        forsn(j,1,mailUserCount+1) {
            forsn(k,1,mailUserCount+1) {
                if(j==k) continue;
                if(!mailSentMatrix[j][k]) {
                    fprintf(satOutputFile,"-%lld -%lld 0\n",(j-1)*phoneUserCount+fromPhoneList[i],(k-1)*phoneUserCount+toPhoneList[i]);
                }
            }
        }
    }

    // one value for each row : two can't be simultaneously true in one row
    forsn(i,1,mailUserCount+1) {
    	forsn(j,1,phoneUserCount+1) {
    		forsn(k,1,phoneUserCount+1) {
    			if(j==k) continue;
    			fprintf(satOutputFile, "-%lld -%lld 0\n", (i-1)*phoneUserCount+j, (i-1)*phoneUserCount+k);
    		}
    	}
    }

    // one value for each coloumn : two can't be simultaneously in one coloumn
    forsn(i,1,mailUserCount+1) {
    	forsn(j,1,mailUserCount+1) {
            if(i==j) continue;
    		forsn(k,1,phoneUserCount+1) {
    			fprintf(satOutputFile, "-%lld -%lld 0\n", (i-1)*phoneUserCount+k, (j-1)*phoneUserCount+k);
    		}
    	}
    }
}

int main(int argc, char* argv[]) {
    
    // open a satInputFile file named fileName.satinput
    string name = argv[1];
    ofstream outfile("filename");
    outfile << name;
    
    string file(argv[1]);
    getCNF(file);
    return 0;
}
