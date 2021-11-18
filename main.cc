#include <stdio.h>
#include "Branch.h"



int main(int argc, char *argv[]) {
	// sim smith <B> <tracefile>   
	// sim bimodal <M2> <tracefile>
    //sim gshare <M1> <N> <tracefile>

	Branch branchPredictor =  Branch();

	string branchType = argv[1];
	if (branchType == "smith"){
		int N =  std::stoi(argv[2]);
		string trace_file =  argv[3];
		branchPredictor.smithNBitCounterPredictor(N, trace_file);

	} else if(branchType == "bimodal"){
		int PCBits =  std::stoi(argv[2]);
		string trace_file =  argv[3];
		branchPredictor.bimodalPredictor(PCBits, trace_file);
		
	} else {
		if (branchType == "gshare") {
			int PCBits =  std::stoi(argv[2]);
			int globalbranchHitory =  std::stoi(argv[3]);
			string trace_file =  argv[4];
			branchPredictor.gsharePredictor(PCBits, globalbranchHitory, trace_file);
		} else {
			printf("Check your input");
		}
	}

	// ifstream myfile;
    // myfile.open (trace_file);
    // string line;

    // while ( getline (myfile,line) )
    // {
    //   memory.at(0).doStuff(line);
    // }
    // myfile.close();

}
