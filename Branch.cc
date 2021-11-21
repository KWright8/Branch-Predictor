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

// Smith n-bit counter predictor
int Branch::getSmithCounter(){
    return smithCounter;

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
                    smithCounter--;
                }
            } else {
                if(smithCounter < (m-1)) {
                    smithCounter++;
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

       // printf("DEBUG: actual %c, address %s\n", actual, address.c_str());
        // printf("DEBUG: address: %s\n", address.c_str());
        string binaryAddress =  getBinaryString(address);

        // printf("DEBUG: binary address: %s\n ", binaryAddress.c_str());
        // get rid of last 2 zeros
        binaryAddress.pop_back();
        binaryAddress.pop_back();

        // printf("DEBUG: binary address without last 2 zeros: %s\n ", binaryAddress.c_str());

        string temp;
        int in = (binaryAddress.length() - 1);
        // printf("LENGTH: %lu\n", binaryAddress.length());
        for (int i  = 0; i < PCBits; i++) {

            temp = binaryAddress.at(in) + temp;
            in--;
        }

        // printf("DEBUG: binary address: %s\n ", temp.c_str());

        int index = std::stoull(temp, NULL,2);
        // printf("DEBUG: Index address: %d\n ", index);
        totalPredictions++;
        int m = 8;
    

        int OC = predictionTable.at(index);

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
    
        // int NC = predictionTable.at(index);   
 
    }

    printBimodal(predictionTable);

   


    // initilize a vector of size n
    // initilize all counters to 4 weak taken
    // figure out how to index
    // use same incrment and decrment logic


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
    // printf("PARAMS gshare %d, %d, %s\n", PCBits, globalbranchHitory, trace_file.c_str());
    command = "./sim gshare ";
    command += to_string(PCBits);
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

        // printf("DEBUG: actual %c\n", actual);
        // printf("DEBUG: address: %s\n", address.c_str());
        string binaryAddress =  getBinaryString(address);

        // printf("DEBUG: binary address: %s\n", binaryAddress.c_str());
        // get rid of last 2 zeros
        binaryAddress.pop_back();
        binaryAddress.pop_back();

        // printf("DEBUG: binary address without last 2 zeros: %s\n", binaryAddress.c_str());

        string temp;
        int in = (binaryAddress.length() - 1);
        // printf("LENGTH: %lu\n", binaryAddress.length());
        for (int i  = 0; i < PCBits; i++) {

            temp = binaryAddress.at(in) + temp;
            in--;
        }

        // printf("DEBUG: TEMP: %s\n", temp.c_str());

        string MSB;
        for (int i = 0; i < numMSB; i++) {
            MSB.push_back(temp.at(i));
        }


        string LSB;
        for (int i = 1; i <= globalbranchHitory; i++) {

            LSB = temp.at((temp.length()) - i) + LSB;

        }


        // printf("DEBUG: MSB: %s LSB %s\n", MSB.c_str(), LSB.c_str());

        string xorvalue = xorstring(gbh, LSB);


        // printf("DEBUG: xor %s\n", xorvalue.c_str());

        // printf("DEBUG: GBH %s\n", gbh.c_str());
        // printf("DEBUG: gbh %d\n ", globalbranchHitory);

        string xortemp;

        if (xorvalue.size() != globalbranchHitory){

            // remove excess leading 0s
            for (int i = 0; i < globalbranchHitory; i++) {
                xortemp =  xorvalue.at(globalbranchHitory - i) + xortemp;
            }
           
        } else {
            xortemp = xorvalue;
        }

        // printf("DEBUG XORTEMP %s\n", xortemp.c_str());

        string m_Index = MSB + xortemp;
        // printf("DEBUG: INDEX BINARY %s\n", m_Index.c_str());
        int index = std::stoull(m_Index, NULL,2);
        // printf("DEBUG index %d\n", index);

        // check table
        if (predictionTable.at(index) >= 4){
            prediction = 't';
            
        } else {
            prediction = 'n';
        }   

        string outcome;
        if (prediction != actual){
            missPredictions++; 
       
        }
        // printf("DEBUG:  outcome gbh %s\n", gbh.c_str());
   

        if (actual == 'n') {
            if(predictionTable.at(index) > 0) {

                int inter =  (predictionTable.at(index) - 1);
                predictionTable.at(index) = inter;
            }

            outcome =  outcome + "0"; 
            for (int i = 0; i < (gbh.length()-1); i++) {
                outcome = outcome + gbh.at(i);
            }
        } else {
            if(predictionTable.at(index) < 7) {
                int interadd =  predictionTable.at(index) + 1;
                predictionTable.at(index) =  interadd;
            }  

            // shift in 1 for taken
            outcome =  outcome + "1"; 
             for (int i = 0; i < (gbh.length()-1); i++) {
                outcome = outcome + gbh.at(i);
            }
        } 

        gbh = outcome;
        // printf("\n\n");
        // printf("DEBUG gbh %s\n", gbh.c_str());


        // shift right once put actual value ar front
        // taken shift in 1? not taken shift in 0?
    }

    printGShare(predictionTable);
    
}

void Branch::initGBH(int size) {
        // initilize  gbh 0
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
    printf("FINAL BIMODAL CONTENTS\n");

    // for (int i = 0; i < predictionTable.size(); i++){
    //     printf("%d   %d\n",i, predictionTable.at(i));
    // }

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
    int i;

    // printf("DEBUG v1: %d v2: %d v1 XOR v2: %d\n", v1, v2, xor1);
    stringstream ss;
    ss << std::hex << xor1; // int decimal_value
    std::string res ( ss.str() );
    string final_value = res;
    //printf("DEBUG final: %s\n", final_value.c_str());



    return getBinaryString( final_value);
}









