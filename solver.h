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
    /**
     * 求解器的公有成员主要为主视窗中sol会调用的函数
     * 即为地图的生成与求解。
     */
    Solver();
    virtual SudukoMap Solve(SudukoMap);
    SudukoMap GenerateMap(int level);
    bool Check(SudukoMap mp);

protected:
    int Area(int x, int y);//判断(x,y)属于哪一个小九宫格
    bool Fill(int x, int y, int k);
    void Delete(int x, int y, int k);
    /**
     * 以下是数独生成器的相关函数
     * 通过虚函数的生命可以在子类实现不同的初始化方式
     * 以及改进搜索的效率
     */
    virtual void CornerInit();
    virtual void CenterInit();
    virtual void CrossInit();
    virtual void DFS(int x, int y);
    void Init();
    SudukoMap todoMap, ansMap, outputMap;
    bool col[9][10], row[9][10], block[9][10];
    std::vector <int> srcArr;
    int count;
};

#endif // SOLVER_H
