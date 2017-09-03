#include "solver.h"

Solver::Solver()
{
}

SudukoMap Solver::Solve(SudukoMap mp)
{
    todoMap = mp;
    ansMap.Clear();
    Init();
    DFS(0,0);
    return ansMap;
}

void Solver::Init()
{
    for(int i = 0 ; i < 9; i ++)
    {
        for(int j = 1; j <= 9; j ++)
        {
           col[i][j] = false;
           row[i][j] = false;
           block[i][j] = false;
        }
    }
    for(int i = 0; i < 9; i ++)
    {
        for(int j = 0; j< 9; j ++)
        {
            int n = todoMap.Data(i,j);
            if(n != -1)
            {
                col[i][n] = true;
                row[j][n] = true;
                block[Area(i,j)][n] = true;
            }
        }
    }
}

int Solver::Area(int x, int y)
{
    return (int)(x/3)*3 + (int)(y/3);
}

bool Solver::Fill(int x, int y, int k)
{
    if(col[x][k] || row[y][k] || block[Area(x,y)][k])
        return false;
    else
    {
        col[x][k] = true;
        row[y][k] = true;
        block[Area(x,y)][k] = true;
        todoMap.SetData(x, y, k);
        return true;
    }
}

/**
 * @brief Solver::Check
 * @param mp
 * @return
 * 检查地图mp是否是填充完的合法地图
 * 用在游戏结果的判定
 */
bool Solver::Check(SudukoMap mp)
{
    for(int i = 0; i < 9; i ++)
    {
        for(int j = 1; j <= 9; j ++)
        {
            col[i][j] = false;
            row[i][j] = false;
            block[i][j] = false;
        }
    }
    for(int i = 0; i < 9; i ++)
    {
        for(int j = 0; j < 9; j ++)
        {
            int k = mp.Data(i,j);
            if(k == -1)
                return false;
            if(col[i][k] || row[j][k] || block[Area(i,j)][k]) {
                return false;
            }
            col[i][k] = true;
            row[j][k] = true;
            block[Area(i,j)][k] = true;
        }
    }
    return true;
}

void Solver::Delete(int x, int y, int k)
{
    col[x][k] = false;
    row[y][k] = false;
    block[Area(x,y)][k] = false;
    todoMap.SetData(x, y, -1);
}

/**
 * @brief Solver::DFS
 * @param x
 * @param y
 * 简单暴力的DFS，据说对搜索序预处理可以大幅提高搜索效率
 * 然而并未实现，通过虚函数的声明可在子类里优化DFS
 */
void Solver::DFS(int x, int y)
{
    if(x == 9 && y == 0)
    {
        ansMap = todoMap;
        return;
    }
    if(todoMap.Data(x, y) != -1)
    {
        Next(x, y);
    }
    else
    {
       for(int k = 1; k <= 9; k ++)
       {
           if(Fill(x, y, k))
           {
               Next(x, y);
               Delete(x, y, k);
           }
       }
    }
}

/**
 * @brief Solver::GenerateMap
 * @param level
 * @return
 * 地图生成函数，把个别的初始化操作封装起来提供一个统一的接口返回生成的地图
 * 具体生成方式是先随机生成中间的九宫格的数字
 * 然后通过交换次序来构造四角与十字的九宫格，如此则保证地图合法
 * 最后通过随机挖去若干个空格来设定难度，
 * 不保证解唯一然而由格子的数量可以预期足够多的生成地图是唯一解的
 * 由于DFS尚未优化，因此返回前并不特别进行唯一性的验证。
 */
SudukoMap Solver::GenerateMap(int level)
{
    outputMap.Clear();
    srcArr.clear();
    srand(time(NULL));
    CenterInit();
    CrossInit();
    CornerInit();
    level *= 15;
    while(level --)
    {
        int x = rand() % 9, y = rand() % 9;
        outputMap.SetData(x, y, -1);
        outputMap.SetOriginal(x, y, false);
    }
    return outputMap;
}

void Solver::CenterInit()
{
    for(int i = 1; i <= 9 ; i++) {
        srcArr.push_back(i);
    }
    std::random_shuffle(srcArr.begin(), srcArr.end());
    int k = 0;
    for(int i = 3; i < 6; i ++)
        for(int j = 3;j < 6; j ++) {
            outputMap.SetData(i, j, srcArr[k++]);
        }

}

void Solver::CrossInit()
{
    for(int i = 3; i < 6; i++)
    {
        int k = 0;
        for(int j = 3; j < 6; j++)
        {
            if(i == 3)
            {
                outputMap.SetData(i+1, k, outputMap.Data(i, j));
                outputMap.SetData(i+2, k+6, outputMap.Data(i, j));
                k++;
            }
            else if(i == 4)
            {
                outputMap.SetData(i+1, k, outputMap.Data(i, j));
                outputMap.SetData(i-1, k+6, outputMap.Data(i, j));
                k++;
            }
            else if(i == 5)
            {
                outputMap.SetData(i-2, k, outputMap.Data(i, j));
                outputMap.SetData(i-1, k+6, outputMap.Data(i, j));
                k++;
            }
        }
    }
    for(int j = 3; j < 6; j ++)
    {
        int k = 0;
        for(int i = 3; i < 6; i ++)
        {
            if(j == 3)
            {
                outputMap.SetData(k, j+1, outputMap.Data(i, j));
                outputMap.SetData(k+6, j+2, outputMap.Data(i, j));
                k++;
            }
            else if(j == 4)
            {
                outputMap.SetData(k, j+1, outputMap.Data(i, j));
                outputMap.SetData(k+6, j-1, outputMap.Data(i, j));
                k++;

            }
            else if(j == 5)
            {
                outputMap.SetData(k, j-2, outputMap.Data(i, j));
                outputMap.SetData(k+6, j-1, outputMap.Data(i, j));
                k++;
            }
        }
    }
}

void Solver::CornerInit()
{
    for(int i = 0; i < 3; i++)
    {
        int k = 0;
        for(int j = 3; j < 6; j++)
        {
            if(i == 0)
            {
                outputMap.SetData(i+1, k, outputMap.Data(i, j));
                outputMap.SetData(i+2, k+6, outputMap.Data(i, j));
                k++;
            }
            else if(i == 1)
            {
                outputMap.SetData(i+1, k, outputMap.Data(i, j));
                outputMap.SetData(i-1, k+6, outputMap.Data(i, j));
                k++;
            }
            else if(i == 2)
            {
                outputMap.SetData(i-2, k, outputMap.Data(i, j));
                outputMap.SetData(i-1, k+6, outputMap.Data(i, j));
                k++;
            }
        }
    }
    for(int i = 6; i < 9; i ++)
    {
        int k = 0;
        for(int j = 3; j < 6; j ++)
        {
            if(i == 6)
            {
                outputMap.SetData(i+1, k, outputMap.Data(i, j));
                outputMap.SetData(i+2, k+6, outputMap.Data(i, j));
                k++;
            }
            else if(i == 7)
            {
                outputMap.SetData(i+1, k, outputMap.Data(i, j));
                outputMap.SetData(i-1, k+6, outputMap.Data(i, j));
                k++;
            }
            else if(i == 8)
            {
                outputMap.SetData(i-2, k, outputMap.Data(i, j));
                outputMap.SetData(i-1, k+6, outputMap.Data(i, j));
                k++;
            }
        }
    }
}
