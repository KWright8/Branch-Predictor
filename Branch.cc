#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
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
    // printf("PARAMS SMITH %d, %s", N,trace_file.c_str());
    int m;

    // initilize counter 
    switch(N) {
        case 1  :
            smithCounter = 1;
            m = 1;
            printf("1\n");
        break; 
        case 2  :
            smithCounter = 2;
            m = 4;
            printf("2\n");
        break; 
        case 3  :
            smithCounter = 4;
            m = 8;
            printf("3\n");
        break; 
        case 4  :
            smithCounter = 8; 
            m = 16;
            printf("4\n");
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
    printf("./sim smith 1 jpeg_trace.txt\n");
    printf("OUTPUT\n");
    printf("number of predictions:		%d\n", totalPredictions);
    printf("number of mispredictions:	%d\n", missPredictions);
    printf("misprediction rate:		%.2f%%\n", (((float)missPredictions/(float)totalPredictions)*100));
    printf("FINAL COUNTER CONTENT:		%d\n", smithCounter);

}

void Branch::bimodalPredictor(int PCBits, string trace_file) {
    // printf("PARAMS BIMODAL %d, %s\n", PCBits,trace_file.c_str());

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

        string binaryAddress =  getBinaryString(address);
        string temp;
        int in = binaryAddress.length() - 3;
        for (int i  = 0; i < PCBits; i++) {

            temp.push_back(binaryAddress.at(in));
            in--;
        }

        int index = std::stoull(temp, NULL,2);
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

    /printBimodal(predictionTable);
   


    // initilize a vector of size n
    // initilize all counters to 4 weak taken
    // figure out how to index
    // use same incrment and decrment logic


}

void Branch::printBimodal(vector<int> predictionTable) {
    printf("COMMAND\n");
    printf("./sim bimodal 6 gcc_trace.txt\n");
    printf("OUTPUT\n");
    printf("number of predictions:		%d\n", totalPredictions);
    printf("number of mispredictions:	%d\n", missPredictions);
    printf("misprediction rate:		%.2f%%\n", (((float)missPredictions/(float)totalPredictions)*100));
    printf("FINAL BIMODAL CONTENTS\n");

    for (int i = 0; i < predictionTable.size(); i++){
        printf("%d %d\n",i, predictionTable.at(i));
    }

}

void Branch::gsharePredictor(int PCBits, int globalbranchHitory, string trace_file) {
     printf("PARAMS BIMODAL %d, %d, %s", PCBits, globalbranchHitory, trace_file.c_str());
    // Model a gshare branch predictor with parameters {m,n}, where:
    // o m is the number of low-order PC bits used to form the prediction table index. Note: discard the lowest two bits of the PC, since they are always zero, i,e., use bits m+1 through 2 of the PC.
    // o n is the number of bits in the global branch history register. Note: n<=m. Note: n
    // may equal zero, in which case we have the simplest bimodal branch predictor
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









