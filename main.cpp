#include <iostream>
#include "Solution.h"
using namespace std;
const int k[6]={5,8,28,72,12,49};
   int logTabu(int i){
       ofstream outfile;
       outfile.open("Logtabusearch.csv",ios::app);
       AdjVertexList adjVertexList = readmatrix(instDir+instance[i]);
       Solution solution(adjVertexList,k[i],1415365819);
       time_t start;
       time_t end;
       time(&start);
       solution.tabusearch(100000000);
       time(&end);
       double cost = difftime(end, start);
       outfile << instance[i] << ",";
       outfile << "tabusearch" << ",";
       outfile <<1415365819 << ",";
       outfile << k[i] << ",";
       outfile << cost << ",";
       outfile << solution.iter << ",";
       for (int j = 0; j < solution.m; ++j) {
           outfile << solution.sol[j] << " ";
       }
       outfile << endl;
       //cout<<k[i]<<endl;
       outfile.close();
       return 0;
   }
   int logtabusearch(){
       ofstream outfile;
       outfile.open("Logtabusearch.csv");
       outfile<<"Instance"<<","<<"Algorithm"<<","<<"Randseed"<<","<<"k"<<","<<"time"<<","
              <<"Itercount"<<","<<"Solution"<<","<<endl;
       outfile.close();
       for(int i =0;i<4;i++){
           logTabu(i);
       }
       return 0;
   }
int HEA(AdjVertexList adjVertexList,int k){
    Solution solution[8];
    for (int i = 0; i <8 ; ++i) {
        solution[i] = new Solution(adjVertexList,k,1415365819+i);

    }

};

int main() {
   /*AdjVertexList adjVertexList = readmatrix(instDir+instance[3]);
    Solution solution(adjVertexList,72,1415365819);
    cout<<solution.conflictedge;
    solution.tabusearch(100000000);*/
    logTabu(5);


}
