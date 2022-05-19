#ifndef __BRANCH_H_
#define __BRANCH_H_

#include <iostream>
using namespace std;
#include <map>
class Predictor{
    private:

        unsigned int addrBits; //number of bits of address used to index  
        unsigned int historyBits; //number of bits used for global history
        unsigned int globalHistory; //record of global histroy
        int correct;
        int total;
        int n;
        bool debug;
	      unsigned int hexToInt(string);
        unsigned int truncateAddress (unsigned int);
        void updateGlobalHistory(bool); //update the history
        void updatePredictorState(int,int,int,int);
       
    public:
        Predictor(unsigned int, unsigned int, unsigned int, bool);
        bool makePrediction(string, bool);
        void printStats();
        map<int, map<int, int> > predictionMap;
        map<int, map<int, int> > countTable;
        void PredictorMap();
        long unsigned entries;

};

enum {
  NOTTAKEN = 0,
  TAKEN = 1, 
};
#endif
