#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>


using namespace std;

class Branch {

    public:       
        Branch();
        int getMispredictions();
        int gettotalPredictions();
        void gsharePredictor(int PCBits, int globalbranchHitory, string trace_file);
        void smithNBitCounterPredictor(int N, string trace_file);
        void bimodalPredictor(int PCBits, string trace_file);
        
    private:
        int smithCounter; 
        int totalPredictions;
        int missPredictions;
        string command;
        string hexAddress;
        string gbh;

        void incrementSmithCounter();
        void decrementSmithCounter();
        void initGBH(int size);
        void printBimodal(vector<int> predictionTable);
        void printGShare(vector<int> predictionTable);
        void printSmith();
        void oneBitSmith(char actual, int max); 
        string xorstring(string value1, string value2);
        string getBinaryString(string address);
};