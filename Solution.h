//
// Created by 肖晓宇 on 2018/4/13.
//

#ifndef GCP_SOLUTION_H
#define GCP_SOLUTION_H
#include <iostream>
#include <fstream>
#include <vector>

typedef std::vector<int> AdjVertex;
typedef std::vector< AdjVertex > AdjVertexList;//定义邻接表
const std::string instDir = "../instance/";
const int maxLength = 9999;
const std::string instance[10] = {
        "DSJC125.1.col",    // 0
        "DSJC250.1.col",    // 1
        "DSJC250.5.col",    // 2
        "DSJC250.9.col",    // 3
        "DSJC500.1.col",    // 41
        "DSJC500.5.col",    // 5
        "DSJC500.9.col",    // 6
        "DSJC1000.1.col",   // 7
        "DSJC1000.5.col",   // 8
        "DSJC1000.9.col",   // 19

};

class Solution{
public:
    int adjColorList[1000][100];//邻接颜色表
    int tabulist[1000][100];//禁忌表
    int sol[1000];//解
    int conflictedge = 0;
    int conflictpoint[1000];//冲突节点表
    int conflictPointPosition[1000];//冲突节点位置
    int conflictPointNumber = 0;//冲突节点数
    int k = 0;//定义颜色数
    int iter =0;
    int m = 0;//定义节点数
    AdjVertexList adjVertexListin;

    Solution(AdjVertexList adjVertexList,int k,int seed);
    void tabusearch(int Maxiter);
};

extern AdjVertexList readmatrix(const std::string &fileName);
extern int tabusearch(AdjVertexList adjVertexList, int k,int* solution);
#endif //GCP_SOLUTION_H
