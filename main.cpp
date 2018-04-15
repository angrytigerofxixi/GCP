#include <iostream>
#include <set>
#include "Solution.h"
using namespace std;
const int k[6]={5,8,28,72,12,49};
//每次打印一个禁忌搜索的例子
   int logTabu(int i){
       ofstream outfile;
       outfile.open("Logtabusearch.csv",ios::app);
       AdjVertexList adjVertexList = readmatrix(instDir+instance[i]);
       Solution solution(adjVertexList,k[i],1776672059);
       time_t start;
       time_t end;
       time(&start);
       solution.tabusearch(900000000);
       time(&end);
       double cost = difftime(end, start);
       outfile << instance[i] << ",";
       outfile << "tabusearch" << ",";
       outfile <<1776672059<< ",";
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
//每次打印多个禁忌搜索的例子
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

//hybrid evoluation
void HEA(int k,int Maxiter,AdjVertexList adjVertexList,int M) {
    int m = adjVertexList.size();
    int solutions[20][1000];
    for(int i= 0;i<M;i++){
        for(int j=0;j<m;j++){
            solutions[i][j]=rand()%k;
        }
    }
    int collideEdges[20];
    //----------------------------initialize the population and get conflictEdges-----------------------------------------
    for (int i = 0; i < M; ++i) {
        collideEdges[i]= tabusearch(adjVertexList,k,solutions[i]);
    }
    int iter = 0;
    int size = M;
    time_t start;
    time_t  end;
    time(&start);
    while (iter < Maxiter) {
        //----------------select two parents, s1 is selected randomly,s2 is the best one------------------------------------
        int x1 = rand() % size;
        int x2 = ((x1==0)?1:0);
        int mincollide = 1000000;
        float number = 1;
        for(int i = 0;i<size;i++){
            if(i!=x1){
                if(collideEdges[i]<mincollide) {
                    mincollide = collideEdges[i];
                    x2 = i;
                    number = 1.0;
                } else if(collideEdges[i] == mincollide){
                    number ++;
                    if (rand()/(RAND_MAX+1.0)<1/number){
                        x2 = i;
                    }
                }
            }

        }
        int s1[1000];
        int s2[1000];
        for(int i=0;i<m;i++){
            s1[i] = solutions[x1][i];
            s2[i] =solutions[x2][i];
        }
        int s0[1000];
        for (int j = 0; j < m; ++j) {
            s0[j] = -1;
        }
        //------------------------------------------------combine two parents-----------------------------------------------
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

        int f = tabusearch(adjVertexList,k,s0);
        //----------------------------updathe the population-------------------------------------------------------------
        cout << " f是 " << f << endl;
        for(int i =0;i<m;i++){
            solutions[size][i] = s0[i];
        }
        collideEdges[size] = f;
        ++size;//此时size是包含了s0之后的size
        float bestcollideedge = 0;
        int bestindex = 0;
        for (int i = 0; i < size; ++i) {
            if (collideEdges[i] > bestcollideedge) {
                bestcollideedge = collideEdges[i];
                bestindex = i;
            }
        }
        if (bestindex != size - 1) {
            for(int i=0;i<m;i++){
                solutions[bestindex][i] = s0[i];
            }
            collideEdges[bestindex] = f;
            --size;

            cout<<"执行最优策略"<<endl;
        }
        else if(size<2*M){
            cout<<"单纯添加"<<endl;
        }
        else{
            while (size>M){
                int worst = 0;
                int worstindex =0;
                for(int i =0;i <size;i++){
                    if(collideEdges[i]>worst){
                        worst = collideEdges[i];
                        worstindex = i;
                    }
                }
                for(int i =0;i<m;i++){
                    solutions[worstindex][i] =solutions[size-1][i];
                }
                    collideEdges[worstindex] =collideEdges[size-1];
                size--;
            }
            //-------------------------种群变异策略------------------------------------------------------------------
            // srand((int)time(0));
            set<int> mutatedIndibiduals;
            int mutatenumber = 1;
            while (mutatenumber -- ){
                int individual;
                do{
                    individual = rand()%size;
                }while (mutatedIndibiduals.find(individual)!=mutatedIndibiduals.end());
                mutatedIndibiduals.insert(individual);
                //---------------个体变异策略---------------------------------------------------------------------------
                int perturbVertexNumber = rand()%m;
                for(;perturbVertexNumber>0;perturbVertexNumber--){
                    solutions[individual][rand()%m] =rand()%k;
                }//下面更新变异后的个体信息
                int adjColorTable[1000][100];
                for(int i= 0;i < m;i++){
                    for (int j =0;j < adjVertexList[i].size() ; ++j) {
                        int color = solutions[individual][adjVertexList[i][j]];
                        adjColorTable[i][color]++;
                    }
                }

                collideEdges[individual] = 0;
                for (int i = 0; i < m; ++i) {
                    int possiblecollide = adjColorTable[i][solutions[individual][i]];
                    if( possiblecollide > 0) {
                        collideEdges[individual] += possiblecollide;
                    }
                }
                collideEdges[individual] = collideEdges[individual]/2;
            }
            cout<<"执行削减策略"<<endl;
        }

        for (int i1 = 0; i1 < size; ++i1) {
            cout << " " << collideEdges[i1] << " ";

        }
        cout << "第"<<iter<<"代"<<endl;
        iter+=1;
     cout<<endl;
    }
    time(&end);
    ofstream outfile;
    outfile.open("LogHEA.csv",ios::app);
    double cost = difftime(end, start);
    outfile << instance[5] << ",";
    outfile << "tabusearch" << ",";
    outfile <<1776672059<< ",";
    outfile << k << ",";
    outfile << cost << ",";
    outfile << iter << ",";
    for (int j = 0; j < m; ++j) {
        outfile << 0<< " ";
    }
    outfile << endl;
    //cout<<k[i]<<endl;
    outfile.close();
}

//用于打印hybrid evloluation
int logHEA(){
    ofstream outfile;
    outfile.open("LogHEA.csv");
    outfile<<"Instance"<<","<<"Algorithm"<<","<<"Randseed"<<","<<"k"<<","<<"time"<<","
           <<"Itercount"<<","<<"Solution"<<","<<endl;
    outfile.close();
    AdjVertexList adjVertexList = readmatrix(instDir + instance[5]);
    HEA(48, 10000, adjVertexList, 8);
}

int main() {
    AdjVertexList adjVertexList = readmatrix(instDir + instance[5]);
    srand(17766720);
   // int sol[1000];
   // for(int i =0;i<500;i++){
     //   sol[i] = rand()%49;
   // }
   // tabusearch(adjVertexList,49,sol);*/
   // logTabu(5);

     HEA(48, 10000, adjVertexList, 8);

}




