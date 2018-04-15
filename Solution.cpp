//
// Created by 肖晓宇 on 2018/4/13.
//


#include"Solution.h"
using namespace std;

// read the instance and get the  Adjacent vertex list;
AdjVertexList readmatrix(const string &fileName)
{
    int vertexNum;
    int edgenumber;
    int vertex1;
    int vertex2;
    char head;
    char str[maxLength];
    ifstream readfile(instDir +fileName);
    while(true){
        readfile >> head;
        if(head == 'c'){
            readfile.getline(str,maxLength);
        } else{
            break;
        }
    }
    readfile >> str >> vertexNum >> edgenumber;
    AdjVertexList adjVertexList(vertexNum,vector<int>());
    while(edgenumber--){
        readfile >> head >> vertex1 >> vertex2;
        vertex1 -= 1;
        vertex2 -= 1;
        adjVertexList[vertex1].push_back(vertex2);
        adjVertexList[vertex2].push_back(vertex1);
    }

    readfile.close();
    return adjVertexList;

}



Solution::Solution(AdjVertexList adjVertexList,int colornumber,int seed) {
    adjVertexListin = adjVertexList;
    m = adjVertexList.size();
    k = colornumber;
    //初始化解
    srand(seed);
    for (int i = 0; i <m ; ++i) {
        int color = rand()%k;
        sol[i] =color;
    }
    //初始化邻接颜色表
    for(int i= 0;i < m;i++){
        for (int j =0;j < adjVertexList[i].size() ; ++j) {
            int color = sol[adjVertexList[i][j]];
            adjColorList[i][color]++;
        }
    }
    //初始化邻接冲突节点表和冲突边数
    for (int i = 0; i < m; ++i) {
        int possiblecollide = adjColorList[i][sol[i]];
        if( possiblecollide > 0) {
            conflictedge += possiblecollide;
            conflictpoint[conflictPointNumber] = i;
            conflictPointPosition[i] = conflictPointNumber;
            conflictPointNumber ++ ;
        }
    }
    conflictedge = conflictedge/2;
}

Solution:: Solution(AdjVertexList adjVertexList,int colornumber,int *sol1){
    cout<<"初始conflictedge是"<<conflictedge<<endl;
    adjVertexListin = adjVertexList;
    m = adjVertexList.size();
    k = colornumber;
    for(int i =0;i<m;i++){
        sol[i] = sol1[i];
    }
    //初始化邻接颜色表
    int modifycolorlist [1000][100];
    memcpy(adjColorList,modifycolorlist, sizeof(modifycolorlist));
    memcpy(tabulist,modifycolorlist, sizeof(modifycolorlist));
    int modifyConflit[1000];
    memcpy(conflictpoint,modifyConflit, sizeof(modifyConflit));
    memcpy(conflictPointPosition,modifyConflit, sizeof(modifyConflit));

    for(int i= 0;i < m;i++){
        for (int j =0;j < adjVertexList[i].size() ; ++j) {
            int color = sol[adjVertexList[i][j]];
            adjColorList[i][color]++;
        }
    }
    //初始化邻接冲突节点表和冲突边数
    for (int i = 0; i < m; ++i) {
        int possiblecollide = adjColorList[i][sol[i]];
        if( possiblecollide > 0) {
            conflictedge += possiblecollide;
            conflictpoint[conflictPointNumber] = i;
            conflictPointPosition[i] = conflictPointNumber;
            conflictPointNumber ++ ;
        }
    }
    conflictedge = conflictedge/2;
}

void Solution::tabusearch(int Maxiter){
    int bestConflictedge =conflictedge;
    while(conflictedge >0 && iter < Maxiter) {
        int bestvertex = 0;
        int bestmove = sol[0];
        int bestlocalf = m;
        int tabubestvertex = 0;
        int tabubestmove =sol[0];
        int tabubestlocalf = m;
        float number =1;
        float tabunumber = 1;
        //---------------------findbestmove function------------------------------------------------------------------------
        for (int u = 0; u <conflictPointNumber ; ++u) {
            int choosepoint = conflictpoint[u];
            for (int i = 0; i <k ; ++i) {
                if(i != sol[conflictpoint[u]]) {
                    if (iter >= tabulist[choosepoint][i]) {
                        if (adjColorList[choosepoint][i] < bestlocalf) {
                            bestvertex = choosepoint;
                            bestmove = i;
                            bestlocalf = adjColorList[choosepoint][i];
                            number = 1.0;
                        } else if (adjColorList[choosepoint][i] == bestlocalf) {
                            number += 1;
                            if (rand()/(RAND_MAX+1.0) < 1 / number) {
                                bestmove = i;
                                bestvertex = choosepoint;
                            }
                        }
                    } else {
                        if (adjColorList[choosepoint][i] < tabubestlocalf) {
                            tabubestvertex = choosepoint;
                            tabubestlocalf = adjColorList[choosepoint][i];
                            tabubestmove = i;
                            tabunumber = 1.0;
                        } else if (adjColorList[choosepoint][i] == tabubestlocalf) {
                            tabunumber += 1;
                            if (rand()/(RAND_MAX+1.0)  < 1 / tabunumber) {
                                tabubestmove = i;
                                tabubestvertex = choosepoint;
                            }
                        }
                    }
                }
            }
        }
        if(tabubestlocalf < bestlocalf && (conflictedge - adjColorList[tabubestvertex][sol[tabubestvertex]]
                                           + tabubestlocalf < bestConflictedge)) //解禁策略
        {
            bestmove = tabubestmove;
            bestlocalf = tabubestlocalf;
            bestvertex = tabubestvertex;
        }
        //-------------------------------------makemove function -----------------------------------------------------------
        iter += 1;
        if(bestlocalf != m){
            if (conflictedge < bestConflictedge)
                bestConflictedge = conflictedge;
            if (bestlocalf == 0)
            {
                conflictpoint[conflictPointPosition[bestvertex]] = conflictpoint[conflictPointNumber-1];
                conflictPointPosition[conflictpoint[conflictPointNumber-1]]=conflictPointPosition[bestvertex];
                conflictPointNumber--;
            }
            conflictedge = conflictedge - adjColorList[bestvertex][sol[bestvertex]] +bestlocalf;
            int oldsol = sol[bestvertex];
            sol[bestvertex] = bestmove;
            //更新邻接颜色表
            for(int adjacentvertex:adjVertexListin[bestvertex])
            {
                int x =adjColorList[adjacentvertex][sol[adjacentvertex]];
                adjColorList[adjacentvertex][bestmove] +=1;
                adjColorList[adjacentvertex][oldsol] -= 1;
                if(x!= 0 && adjColorList[adjacentvertex][sol[adjacentvertex]]==0) {
                    conflictpoint[conflictPointPosition[adjacentvertex]] = conflictpoint[conflictPointNumber-1];
                    conflictPointPosition[conflictpoint[conflictPointNumber-1]]=conflictPointPosition[adjacentvertex];
                    conflictPointNumber--;
                }
                if(x==0&&adjColorList[adjacentvertex][sol[adjacentvertex]]!=0) {
                    conflictpoint[conflictPointNumber] = adjacentvertex;
                    conflictPointPosition[adjacentvertex] = conflictPointNumber;
                    conflictPointNumber++;
                }
            }
            tabulist[bestvertex][oldsol] = iter + conflictedge + rand() % 11    ;
        }
        cout<<conflictedge<<" "<<iter<<" "<<bestConflictedge<<endl;

    }
      cout<<"经过tabusearch的"<<conflictedge<<endl;
    }

//因为觉得Solution类的构造对于hybird evoluation search可能并不太实用，所以采用函数形式的 tabu search；
int tabusearch(AdjVertexList adjVertexList, int k,int* sol){
    int  m = adjVertexList.size();
    //初始化邻接颜色表
    int adjColorList [1000][100] ={0};
    int tabulist[1000][100]={0};
    int conflictedge = 0;
    int conflictpoint[1000]={0};
    int conflictPointPosition[1000]={0};
    int conflictPointNumber =0;
    for(int i= 0;i < m;i++){
        for (int j =0;j < adjVertexList[i].size() ; ++j) {
            int color = sol[adjVertexList[i][j]];
            adjColorList[i][color]++;
        }
    }
    //初始化邻接冲突节点表和冲突边数
    for (int i = 0; i < m; ++i) {
        int possiblecollide = adjColorList[i][sol[i]];
        if( possiblecollide > 0) {
            conflictedge += possiblecollide;
            conflictpoint[conflictPointNumber] = i;
            conflictPointPosition[i] = conflictPointNumber;
            conflictPointNumber ++ ;
        }
    }
    conflictedge = conflictedge/2;
    int bestConflictedge =conflictedge;
    int iter =0;
    int localoptimaSolution[1000];
    while(conflictedge >0 && iter <100000) {
        int bestvertex = 0;
        int bestmove = sol[0];
        int bestlocalf = m;
        int tabubestvertex = 0;
        int tabubestmove =sol[0];
        int tabubestlocalf = m;
        float number =1;
        float tabunumber = 1;
        //---------------------findbestmove function------------------------------------------------------------------------
        for (int u = 0; u <conflictPointNumber ; ++u) {
            int choosepoint = conflictpoint[u];
            for (int i = 0; i <k ; ++i) {
                if(i != sol[conflictpoint[u]]) {
                    if (iter >= tabulist[choosepoint][i]) {
                        if (adjColorList[choosepoint][i] < bestlocalf) {
                            bestvertex = choosepoint;
                            bestmove = i;
                            bestlocalf = adjColorList[choosepoint][i];
                            number = 1.0;
                        } else if (adjColorList[choosepoint][i] == bestlocalf) {
                            number += 1;
                            if (rand()/(RAND_MAX+1.0) < 1 / number) {
                                bestmove = i;
                                bestvertex = choosepoint;
                            }
                        }
                    } else {
                        if (adjColorList[choosepoint][i] < tabubestlocalf) {
                            tabubestvertex = choosepoint;
                            tabubestlocalf = adjColorList[choosepoint][i];
                            tabubestmove = i;
                            tabunumber = 1.0;
                        } else if (adjColorList[choosepoint][i] == tabubestlocalf) {
                            tabunumber += 1;
                            if (rand()/(RAND_MAX+1.0)  < 1 / tabunumber) {
                                tabubestmove = i;
                                tabubestvertex = choosepoint;
                            }
                        }
                    }
                }
            }
        }
        if(tabubestlocalf < bestlocalf && (conflictedge - adjColorList[tabubestvertex][sol[tabubestvertex]]
                                           + tabubestlocalf < bestConflictedge)) //解禁策略
        {
            bestmove = tabubestmove;
            bestlocalf = tabubestlocalf;
            bestvertex = tabubestvertex;
        }
        //-------------------------------------makemove function -----------------------------------------------------------
        iter += 1;
        if(bestlocalf != m){
            if (conflictedge < bestConflictedge) {
                bestConflictedge = conflictedge;
                for (int i = 0; i < m; i++) {
                    localoptimaSolution[i] = sol[i];

                }
            }
            if (bestlocalf == 0)
            {
                conflictpoint[conflictPointPosition[bestvertex]] = conflictpoint[conflictPointNumber-1];
                conflictPointPosition[conflictpoint[conflictPointNumber-1]]=conflictPointPosition[bestvertex];
                conflictPointNumber--;
            }
            conflictedge = conflictedge - adjColorList[bestvertex][sol[bestvertex]] +bestlocalf;
            int oldsol = sol[bestvertex];
            sol[bestvertex] = bestmove;
            //更新邻接颜色表
            for(int adjacentvertex:adjVertexList[bestvertex])
            {
                int x =adjColorList[adjacentvertex][sol[adjacentvertex]];
                adjColorList[adjacentvertex][bestmove] +=1;
                adjColorList[adjacentvertex][oldsol] -= 1;
                if(x!= 0 && adjColorList[adjacentvertex][sol[adjacentvertex]]==0) {
                    conflictpoint[conflictPointPosition[adjacentvertex]] = conflictpoint[conflictPointNumber-1];
                    conflictPointPosition[conflictpoint[conflictPointNumber-1]]=conflictPointPosition[adjacentvertex];
                    conflictPointNumber--;
                }
                if(x==0&&adjColorList[adjacentvertex][sol[adjacentvertex]]!=0) {
                    conflictpoint[conflictPointNumber] = adjacentvertex;
                    conflictPointPosition[adjacentvertex] = conflictPointNumber;
                    conflictPointNumber++;
                }
            }
            tabulist[bestvertex][oldsol] = iter + conflictedge + rand() % 10    ;
        }
    }
    cout<<"经过tabusearch的"<<bestConflictedge<<endl;
    for(int i =0; i<m;i++){
        sol[i] = localoptimaSolution[i];
    }
    return bestConflictedge;



}

