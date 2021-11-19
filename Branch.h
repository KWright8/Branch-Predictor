#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>


using namespace std;

class Branch {

    public:
        int smithCounter; // B
        void smithNBitCounterPredictor(int N, string trace_file);
        void setSmithCounter(int flag);
        int getSmithCounter();
        void incrementSmithCounter();
        void decrementSmithCounter();
        void printSmith();
        void oneBitSmith(char actual, int max); 
        string getBinaryString(string address);
        string command;

      
        void bimodalPredictor(int PCBits, string trace_file);
        void printBimodal(vector<int> predictionTable);
        int bimodalPCBits; // M2

        void gsharePredictor(int PCBits, int globalbranchHitory, string trace_file);
        int gsharePCBits;
        int gshareGlobalBranchHisgtoryBits;

        char actualOutcome;
        string hexAddress;
        int totalPredictions;
        int missPredictions;
        

        Branch();

    


};