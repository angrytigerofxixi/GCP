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
       Solution solution(adjVertexList,k[i],7345455347);
       time_t start;
       time_t end;
       time(&start);
       solution.tabusearch(900000000);
       time(&end);
       double cost = difftime(end, start);
       outfile << instance[i] << ",";
       outfile << "tabusearch" << ",";
       outfile <<7345455347<< ",";
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
    int solutions[20][1000];//用于保存解
    int optima =10000;//用于保存最优值
    int optimaindex = 0;//用于保存最优值索引  便于打印

    //随机初始化解
    for(int i= 0;i<M;i++){
        for(int j=0;j<m;j++){
            solutions[i][j]=rand()%k;
        }
    }
    int collideEdges[20];//用于记录冲突边数
    //----------------------------initialize the population and get conflictEdges-----------------------------------------
    for (int i = 0; i < M; ++i) {
        collideEdges[i]= tabusearch(adjVertexList,k,solutions[i]);
    }
    int iter = 0;
    int size = M;//用于记录种群值
    time_t start;
    time_t  end;
    time(&start);
    while (iter < Maxiter && optima > 0) {
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

                //记录每个颜色有多少个节点
                for (int j = 0; j < m; ++j) {
                    int color = s1[j];
                    if (color == -1) {
                        colorset[k] += 1;
                    } else {
                        colorset[color] += 1;
                    }
                }

               //用于选出拥有最多节点数的颜色集合
                float s1number = 1.0;
                for (int n = 0; n < k; ++n) {
                    if (colorset[n] > maxcolorsize) {
                        maxcolorsize = colorset[n];
                        maxcolor = n;
                        s1number =1.0;
                    } else if(colorset[n] == maxcolorsize){
                        s1number += 1;
                        if(rand()/(RAND_MAX+1.0)<1/s1number){
                            maxcolor = n;
                        }
                    }
                }

                //将节点分配给子代 并从父代中去除这些节点
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

                for(int c =0;c<=k;c++){
                    cout<<colorset[c]<<" ";
                }
                cout<<endl;

                float  s2number = 1.0;
                for (int n = 0; n < k; ++n) {
                    if (colorset[n] > maxcolorsize) {
                        maxcolorsize = colorset[n];
                        maxcolor = n;
                        s2number =1.0;
                    } else if(colorset[n] == maxcolorsize){
                        s2number += 1;
                        if(rand()/(RAND_MAX+1.0)<1/s2number){
                            maxcolor = n;
                        }
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
        //将剩余节点随机分配
        for (int i = 0; i < m; ++i) {
            if (s0[i] == -1) {
                s0[i] = rand() % k;
            }
        }
        //将子代用禁忌搜索调优
        int f = tabusearch(adjVertexList,k,s0);
        for(int i=0;i<m;i++){
            cout<<s0[i]<<" ";
        }
        cout<<endl;
        //----------------------------update the population-------------------------------------------------------------
        cout << " f是 " << f << endl;

        //选出种群中的最大值
        float bestcollideedge = 0;
        int bestindex = 0;
        float randnumber =1.0;
        for (int i = 0; i < size; ++i) {
            if (collideEdges[i] > bestcollideedge) {
                bestcollideedge = collideEdges[i];
                bestindex = i;
                randnumber = 1.0;
            } else if(collideEdges[i] == bestcollideedge){
                randnumber ++;
                if(rand()/(RAND_MAX+1.0)<1/randnumber){
                    bestindex = i;
                }
            }
        }

        //如果子代小于种群中的最大值，则替换
        if (f < collideEdges[bestindex]) {
            for(int i=0;i<m;i++){
                solutions[bestindex][i] = s0[i];
            }
            collideEdges[bestindex] = f;
            cout<<"执行最优策略"<<endl;
        }

        //否则则直接添加
        else if(size<2*M){
             for(int i =0;i<m;i++){
              solutions[size][i] = s0[i];
            }
            collideEdges[size] = f;
            cout<<"单纯添加"<<endl;
            ++size;
        }

        //当种群大于初始种群的两倍时，则剔除最差的初始种群数的解，并扰乱剩余的一个解
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
                int adjColorTable[1000][100] = {0};
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

        iter+=1;

        /* 用于调试时的代码
        for (int i1 = 0; i1 < size; ++i1) {
            cout << " " << collideEdges[i1] << " ";

        }
        cout << "第"<<iter<<"代"<<endl;
        iter+=1;
        cout<<endl;
       */


        //更新最优值
        for(int i=0;i<size;i++){
            if(collideEdges[i]<optima){
                optima =collideEdges[i];
                optimaindex = i;
            }
        }
    }

    //打印相关信息
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
        outfile << solutions[optimaindex][j]<< " ";
    }
    outfile << endl;
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
    HEA(48, 2000, adjVertexList, 8);
}

int main() {
    AdjVertexList adjVertexList = readmatrix(instDir + instance[5]);
     srand(7345455347);
   /*   int sol[1000];
     for(int i =0;i<500;i++){
      sol[i] = rand()%49;
   }
    tabusearch(adjVertexList,49,sol);*/

    logHEA();
   // cout<<4%2;

}




