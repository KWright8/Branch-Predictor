#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include <string>
#include <math.h> 
#include "Branch.h"

using namespace std;

Branch::Branch() {
    totalPredictions = 0;
    missPredictions = 0;
}

void Branch::incrementSmithCounter(){
    smithCounter++;
}

void Branch::decrementSmithCounter(){
    smithCounter--;
}

void Branch::smithNBitCounterPredictor(int N, string trace_file) {
    command = "./sim smith ";
    command += to_string(N);
    command += " ";
    command += trace_file.c_str();

    int m;

    // initilize counter 
    switch(N) {
        case 1  :
            smithCounter = 1;
            m = 1;
        break; 
        case 2  :
            smithCounter = 2;
            m = 4;

        break; 
        case 3  :
            smithCounter = 4;
            m = 8;
        break; 
        case 4  :
            smithCounter = 8; 
            m = 16;
        break;  
        case 5 :
            smithCounter = 16; 
            m = 32;
        case 6 :
            smithCounter = 32; 
            m = 64;
        default : 
            printf("CHECK YOUR INPUT");
            break;
    }

    ifstream myfile;
    myfile.open (trace_file);
    string line;
    char prediction;
   
    while ( getline (myfile,line) )
    {
        totalPredictions++;
        char actual = line.back();
        
        if (N == 1){
            // call the specal case 
            oneBitSmith(actual,m);
        } else {
            // int OC = smithCounter;
            if (smithCounter >= (m/2)){
                prediction = 't';
              
            } else {
                prediction = 'n';
            }   
   
            if (prediction != actual){
                missPredictions++;    
            } 

            if (actual == 'n') {
                if(smithCounter > 0) {
                    decrementSmithCounter();
                }
            } else {
                if(smithCounter < (m-1)) {
                    incrementSmithCounter();
                }  
            }           
        }              
    }

    myfile.close();
    printSmith();
}

void Branch::oneBitSmith(char actual, int m) {
    char prediction;
    if (smithCounter > (m/2)){
        prediction = 't';
    } else {
        prediction = 'n';
    }      

    if (prediction != actual){
        missPredictions++;
        if (actual == 'n') {
            smithCounter = 0;
        } else {
            smithCounter = 1;
        }
    }
}

void Branch::printSmith() {
    printf("COMMAND\n");
    printf("%s\n", command.c_str());
    printf("OUTPUT\n");
    printf("number of predictions:		%d\n", totalPredictions);
    printf("number of mispredictions:	%d\n", missPredictions);
    printf("misprediction rate:		%.2f%%\n", (((float)missPredictions/(float)totalPredictions)*100));
    printf("FINAL COUNTER CONTENT:		%d\n", smithCounter);

}
// bimodal
void Branch::bimodalPredictor(int PCBits, string trace_file) {
    command = "./sim bimodal ";
    command += to_string(PCBits);
    command += " ";
    command += trace_file.c_str();

    vector<int> predictionTable;
    predictionTable.assign(pow(2.0, (float)PCBits), 4);
    
    ifstream myfile;
    myfile.open (trace_file);
    string line;
    char prediction;
    char actual;

    while ( getline (myfile,line) )
    {
        string address;
        for (int i  = 0; i < line.length(); i++){
            if (isalpha(line.at(i))|| isdigit(line.at(i))){
                if( line.at(i) == 'n' or line.at(i) == 't') {
                    actual = line.at(i);
                } else {
                    address.push_back(line.at(i));
                }
            }
        }

        string binaryAddress =  getBinaryString(address);

        binaryAddress.pop_back();
        binaryAddress.pop_back();

        string temp;
        int in = (binaryAddress.length() - 1);

        for (int i  = 0; i < PCBits; i++) {

            temp = binaryAddress.at(in) + temp;
            in--;
        }

        int index = std::stoull(temp, NULL,2);
        totalPredictions++;

        if (predictionTable.at(index) >= 4){
            prediction = 't';
            
        } else {
            prediction = 'n';
        }   

        if (prediction != actual){
            missPredictions++;    
        } 

        if (actual == 'n') {
            if(predictionTable.at(index) > 0) {

                int inter =  predictionTable.at(index) - 1;
                predictionTable.at(index) = inter;
            }
        } else {
            if(predictionTable.at(index) < 7) {
                int interadd =  predictionTable.at(index) + 1;
                predictionTable.at(index) =  interadd;
            }  
        }  
    }
    printBimodal(predictionTable);
}

void Branch::printBimodal(vector<int> predictionTable) {
    printf("COMMAND\n");
    printf("%s\n", command.c_str());
    printf("OUTPUT\n");
    printf("number of predictions:		%d\n", totalPredictions);
    printf("number of mispredictions:	%d\n", missPredictions);
    printf("misprediction rate:		%.2f%%\n", (((float)missPredictions/(float)totalPredictions)*100));
    printf("FINAL BIMODAL CONTENTS\n");

    for (int i = 0; i < predictionTable.size(); i++){
        printf("%d   %d\n",i, predictionTable.at(i));
    }

}
// gshare
void Branch::gsharePredictor(int PCBits, int globalbranchHitory, string trace_file) {
    command = "./sim gshare ";
    command += to_string(PCBits);
    command += " ";
    command += to_string(globalbranchHitory);
    command += " ";
    command += trace_file.c_str();

    vector<int> predictionTable;
    predictionTable.assign(pow(2.0, (float)PCBits), 4);
    initGBH(globalbranchHitory);
    
    ifstream myfile;
    myfile.open (trace_file);
    string line;
    char prediction;
    char actual;
    int numMSB = PCBits - globalbranchHitory;

    int count = 0;
    while ( getline (myfile,line) )
    {   
        count++;
        string address;
        totalPredictions++;
        for (int i  = 0; i < line.length(); i++){
            if (isalpha(line.at(i))|| isdigit(line.at(i))){
                if( line.at(i) == 'n' or line.at(i) == 't') {
                    actual = line.at(i);
                } else {
                    address.push_back(line.at(i));
                }
            }
        }

        string binaryAddress =  getBinaryString(address);
        binaryAddress.pop_back();
        binaryAddress.pop_back();

        string temp;
        int in = (binaryAddress.length() - 1);
    
        for (int i  = 0; i < PCBits; i++) {

            temp = binaryAddress.at(in) + temp;
            in--;
        }
    

        string MSB;
        for (int i = 0; i < numMSB; i++) {
            MSB.push_back(temp.at(i));
        }

        string LSB;
        for (int i = 1; i <= globalbranchHitory; i++) {
            LSB = temp.at((temp.length()) - i) + LSB;
        }

        string xorvalue = xorstring(gbh, LSB);
        string xortemp; 
    
        if (xorvalue.size() > globalbranchHitory){
            int numRemove = xorvalue.size() - globalbranchHitory;
            int ind =  numRemove;
            for (int i = 0; i < globalbranchHitory; i++) {
                xortemp = xortemp + xorvalue.at(ind) ;
                ind = ind + 1;
            } 
        } else {

            if (xorvalue.size() < globalbranchHitory){
                int numz =  globalbranchHitory - xorvalue.size();
                string temp;
                for (int i = 0; i < numz; i++) {
                    temp = temp + "0";
                }
                xortemp =  temp + xorvalue;
            } else {
                xortemp = xorvalue;
            }
        }

        string m_Index = MSB + xortemp;
        int index = std::stoull(m_Index, NULL,2);

        if (predictionTable.at(index) >= 4){
            prediction = 't';
            
        } else {
            prediction = 'n';
        }   

        string outcome;
        if (prediction != actual){
            missPredictions++; 
        }
    
        if (actual == 'n') {
            if(predictionTable.at(index) > 0) {

                int inter =  (predictionTable.at(index) - 1);
                predictionTable.at(index) = inter;
            }
            // shift in 0 not taken
            outcome =  outcome + "0"; 
            for (int i = 0; i < (gbh.length()-1); i++) {
                outcome = outcome + gbh.at(i);
            }
        } else {
            if(predictionTable.at(index) < 7) {
                int interadd =  (predictionTable.at(index) + 1);
                predictionTable.at(index) =  interadd;
            }  

            // shift in 1 for taken
            outcome =  outcome + "1"; 
             for (int i = 0; i < (gbh.length()-1); i++) {
                outcome = outcome + gbh.at(i);
            }
        } 
        gbh = outcome;
    }
    
    printGShare(predictionTable);
}

void Branch::initGBH(int size) {
    for (int i = 0; i < size; i++){
        gbh.push_back('0');
    }
}

void Branch::printGShare(vector<int> predictionTable) {
    printf("COMMAND\n");
    printf("%s\n", command.c_str());
    printf("OUTPUT\n");
    printf("number of predictions:		%d\n", totalPredictions);
    printf("number of mispredictions:	%d\n", missPredictions);
    printf("misprediction rate:		%.2f%%\n", (((float)missPredictions/(float)totalPredictions)*100));
    printf("FINAL GSHARE CONTENTS\n");

    for (int i = 0; i < predictionTable.size(); i++){
        printf("%d   %d\n",i, predictionTable.at(i));
    }
}

string Branch::getBinaryString(string address){
    string temp;
    for (int i = 0; i < address.length(); i++) {
        switch(address.at(i)) {
            case '0':
                temp.append("0000");
                break;
            case '1':
                temp.append("0001");
                break;
            case '2':
                temp.append("0010");
                break;
            case '3':
                temp.append("0011");
                break;
            case '4':
                temp.append("0100");
                break;
            case '5':
                temp.append("0101");
                break;
            case '6':
                temp.append("0110");
                break;
            case '7':
                temp.append("0111");
                break;
            case '8':
                temp.append("1000");
                break;
            case '9':
                temp.append("1001");
                break;
            case 'A':
            case 'a':
                temp.append("1010");
                break;
            case 'B':
            case 'b':
                temp.append("1011");
                break;
            case 'C':
            case 'c':
                temp.append("1100");
                break;
            case 'D':
            case 'd':
                temp.append("1101");
                break;
            case 'E':
            case 'e':
                temp.append("1110");
                break;
            case 'F':
            case 'f':
                temp.append("1111");
                break;
            default:
                printf("OH NO");
        }
    }
    return temp;
}

string Branch::xorstring(string value1, string value2) {
    // convert to int
    int v1 = std::stoull(value1, NULL,2);
    int v2 = std::stoull(value2, NULL,2);
    int xor1 = v1^v2;

    stringstream ss;
    ss << std::hex << xor1; // int decimal_value
    std::string res ( ss.str() );
    string final_value = res;

    return getBinaryString( final_value);
}

int Branch::getMispredictions() {
    return missPredictions;
}

int Branch::gettotalPredictions() {
    return totalPredictions;
}









