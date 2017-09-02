#ifndef SOLVER_H
#define SOLVER_H

#include <QDebug>
#include <ctime>
#include <algorithm>
#include <vector>
#include "sudukomap.h"
#define Next(x,y) y == 8 ? DFS(x+1, 0) : DFS(x, y+1)

class Solver
{
public:
    Solver();
    SudukoMap Solve(SudukoMap);
    bool Check(SudukoMap mp);
    SudukoMap GenerateMap(int level);

private:
    SudukoMap todoMap, ansMap, outputMap;
    void DFS(int x, int y);
    bool col[9][10], row[9][10], block[9][10];
    int Area(int x, int y);
    bool Fill(int x, int y, int k);
    void Delete(int x, int y, int k);
    void Init();
    /**
     * 以下是数独生成器的相关函数
     */
    std::vector <int> srcArr;
    void CornerInit();
    void CenterInit();
    void CrossInit();
    int count;
};

#endif // SOLVER_H
