#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Branch.h"

using namespace std;

Branch::Branch() {
    totalPredictions = 0;
    missPredictions = 0;
}

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
   
//    printf("|     OC      |      P          |         A         |         NC         |\n");
//    printf("--------------------------------------------------------------------------\n");
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

            // int NC =  smithCounter;

        //printf("|     %d     |      %c          |         %c         |         %d         |\n", OC, prediction, actual,NC );

           
        }              
    }
    myfile.close();

    printSmith();

}

void Branch::bimodalPredictor(int PCBits, string trace_file) {
    printf("PARAMS BIMODAL %d, %s", PCBits,trace_file.c_str());



}

void Branch::gsharePredictor(int PCBits, int globalbranchHitory, string trace_file) {
     printf("PARAMS BIMODAL %d, %d, %s", PCBits, globalbranchHitory, trace_file.c_str());
    // Model a gshare branch predictor with parameters {m,n}, where:
    // o m is the number of low-order PC bits used to form the prediction table index. Note: discard the lowest two bits of the PC, since they are always zero, i,e., use bits m+1 through 2 of the PC.
    // o n is the number of bits in the global branch history register. Note: n<=m. Note: n
    // may equal zero, in which case we have the simplest bimodal branch predictor
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


