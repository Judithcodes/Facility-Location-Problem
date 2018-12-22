#include "FLP.h"

int main() {
  FLP flp;
  time_t startTime = clock();
  //flp.greedy();
  flp.SA();
  time_t endTime = clock();
  float  runTime = endTime - startTime;
  cout << endl << "Total Run Time: " << runTime << " ms." << endl;
  
  return 0;
}