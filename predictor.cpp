#include "predictor.h"
#include <sstream>
#include <iomanip>
#include<math.h>
Predictor::Predictor(unsigned int m, unsigned int n, unsigned int addrLength, bool debug){
    this->historyBits = m; // Size of global history
    this->globalHistory = 0; 
    this->addrBits = addrLength;
    this->debug = debug;
    this->correct = 0;
    this->total = 0;
    this->n=n;
    printf("BHT: %u-bit\n", n);
    printf("Address length: %u\n", this->addrBits);
    printf("History length: %u\n", this->historyBits);
    printf("Debug mode: %u\n", this->debug);
};
void Predictor::PredictorMap() {                 
    int columns = (int) pow(2, addrBits);                                                                                                                         
    int rows = (int) pow(2, historyBits);
    //int columns = (int) pow(2, addrBits);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            predictionMap[i][j] = 0;
        }
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            countTable[i][j] = 0;
        }
    }
  }
 

  void Predictor::updatePredictorState(int actual_branch, int predicted_branch, int address, int globalHistory){
    if(n==1)
    {
        switch (actual_branch){
        case TAKEN:
        if(predictionMap[address][globalHistory] == 0) {
	        predictionMap[address][globalHistory]=1;
        }
        break;
        case NOTTAKEN:
        if(predictionMap[address][globalHistory] == 1) {
	        predictionMap[address][globalHistory]=0;
        }
        break;
    
        }
    }
    else if(n==2)
    {
        switch (actual_branch){
        case TAKEN:
        if (predicted_branch==0){
            predictionMap[address][globalHistory]=1;
        }
        else if (predicted_branch==1){
            predictionMap[address][globalHistory]=3;
        }
        else if (predicted_branch==2){
            predictionMap[address][globalHistory]=3;

        }

        break;

        case NOTTAKEN:
        if (predicted_branch==1){
            predictionMap[address][globalHistory]=0;
        }
        else if (predicted_branch==2){
            predictionMap[address][globalHistory]=0;
            
        }
        else if (predicted_branch==3){
            predictionMap[address][globalHistory]=2;
            
        }
        
        break; 
        }
    }
    
    
  }


void Predictor::updateGlobalHistory(bool expected){
    globalHistory = globalHistory << 1;
    globalHistory = globalHistory | expected;
    unsigned int mask = (1 << this->historyBits) - 1;
    globalHistory = globalHistory & mask; 
}

bool Predictor::makePrediction(string input, bool expected){
    // Convert Hex address to integer address
    unsigned int address = truncateAddress(hexToInt(input));
    
    // Currently, this simple branch predictor simulator simply takes 
    // the previous observed branch direction as the next prediction.
    // Predict branch based on last observed branch
    //bool predicted = globalHistory & 1; 
    int p=0;
    int predicted = predictionMap[address][globalHistory];
    if(countTable[address][globalHistory]==0)
    {
        countTable[address][globalHistory]=1;
        entries+=1;
    }
    
    if(n==1){
        if(predicted == int(expected))
        this->correct++;
    }
    else if(n==2)
    {
        if(predicted==0)
        p=0;
        else if(predicted==1)
        p=0;
        else if(predicted==2)
        p=1;
        else if(predicted==3)
        p=1;
        if(p == int(expected)){
        this->correct++;
        }
    }
    this->total++;
    if(expected)
    {
        updatePredictorState(TAKEN, predicted,address,globalHistory);
    }
    else
    {
        updatePredictorState(NOTTAKEN, predicted,address,globalHistory);
    }
    // Update global history 
    
    updateGlobalHistory(expected);
    
    if(n==2)
    {
        return p;
    }
    return predicted;
}

/*
 * Print out branch predictor statistics
 */
void Predictor::printStats(){ 
    printf("------------------------------------------\n"); 
    
    if(total == 0){
        printf("Error, Cannot get rate \n");
    }
    else{
        printf("Misclassification rate: %.2f%%\n", 100.00 * (1 - (double)(this->correct)/this->total));
    } 
    
    printf("correct: %d\n", this->correct);
    printf("total: %d\n",this->total);


    // Update the following line to print out the number of BHT entires used.
    printf("BHT used %lu entries\n",entries);  
}
/*
 * DO NOT MODIFY BELOW THIS
 */


/*
 * Convert Hex string from trace file to integer address
 */
unsigned int Predictor::hexToInt(string input){
    istringstream converter(input);
    unsigned int result;
    converter >> std::hex >> result; 
    return result;
}

/*
 * Truncate Address to specified number of address bits
 */
unsigned int Predictor::truncateAddress(unsigned int input){
    unsigned int mask = (1 << this->addrBits) - 1;
    unsigned int result = input & mask;

    return result;

}

