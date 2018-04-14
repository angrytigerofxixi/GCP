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
int HEA(AdjVertexList adjVertexList,int k,int seed){
    int m = adjVertexList.size();
    Solution solutions[8]={Solution(adjVertexList,k,seed),Solution(adjVertexList,k,seed+1),
                          Solution(adjVertexList,k,seed+2),Solution(adjVertexList,k,seed+3),Solution(adjVertexList,k,seed+4),
                          Solution(adjVertexList,k,seed+5),Solution(adjVertexList,k,seed+6),Solution(adjVertexList,k,seed+7)};
    int maxgeneration = 10000;
    int generation = 0;
    int optima =100000000;
    int M =8;
    while (generation<maxgeneration && optima>0){
        //choose parents，the first parent choosen randomly , the second parent choosen the best;
        int x1 = rand() % M;
        int x2 = ((x1==0)?1:0);
        int minconflict = 1000000;
        float number = 1;
        for(int i = 0;i<M;i++){
            if(i!=x1){
                if(solutions[x1].conflictedge<minconflict) {
                    minconflict = solutions[x1].conflictedge;
                    x2 = i;
                    number = 1.0;
                } else if(solutions[x1].conflictedge==minconflict){
                    number ++;
                    if (rand()/(RAND_MAX+1.0)<1/number){
                        x2 = i;
                    }
                }
            }

        }
        int s1[1000];
        int s2[1000];
        memcpy(s1,solutions[x1].sol, sizeof(solutions[x1].sol));
        memcpy(s2,solutions[x2].sol, sizeof(solutions[x2].sol));
        int s0[1000];
        for (int j = 0; j < m; ++j) {
            s0[j] = -1;
        }
        //combine two parents
        for (int i = 0; i < k; ++i) {
            if (i % 2 != 0) {
                vector<int> colorset(k + 1);
                int maxcolorsize = 0;
                int maxcolor = 0;
                for (int j = 0; j < m; ++j) {
                    int color = s1[j];
                    if (color == -1) {
                        colorset[k] += 1;
                    } else {
                        colorset[color] += 1;
                    }
                }
                for (int n = 0; n < k; ++n) {
                    if (colorset[n] >= maxcolorsize) {
                        maxcolorsize = colorset[n];
                        maxcolor = n;
                    }
                }
                for (int l = 0; l < m; ++l) {
                    if (s1[l] == maxcolor) {
                        s0[l] = i;
                        s1[l] = -1;
                        s2[l] = -1;
                    }

                }
            } else {
                vector<int> colorset(k + 1);
                int maxcolorsize = 0;
                int maxcolor = 0;
                for (int j = 0; j < m; ++j) {
                    int color = s2[j];
                    if (color == -1) {
                        colorset[k] += 1;
                    } else {
                        colorset[color] += 1;
                    }
                }
                for (int n = 0; n < k; ++n) {
                    if (colorset[n] >= maxcolorsize) {
                        maxcolorsize = colorset[n];
                        maxcolor = n;
                    }
                }
                for (int l = 0; l < m; ++l) {
                    if (s2[l] == maxcolor) {
                        s0[l] = i;
                        s2[l] = -1;
                        s1[l] = -1;
                    }

                }
            }
        }

        for (int i = 0; i < m; ++i) {
            if (s0[i] == -1) {
                s0[i] = rand() % k;
            }
        }
        Solution offspring(adjVertexList,k,s0);

    }

};

int main() {
   /*AdjVertexList adjVertexList = readmatrix(instDir+instance[3]);
    Solution solution(adjVertexList,72,1415365819);
    cout<<solution.conflictedge;
    solution.tabusearch(100000000);*/
    logTabu(5);


}
