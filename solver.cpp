#include "solver.h"

Solver::Solver()
{
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0 ;j < 10; j++)
        {
            col[i][j] = false;
            row[i][j] = false;
            block[i][j] = false;
        }
    }
}

SudukoMap Solver::Solve(SudukoMap mp)
{
    ansMap = mp;
    for(int i = 0 ; i < 9; i ++)
    {
        for(int j = 0; j < 0; j ++)
        {
            int n = ansMap.Data(i, j);
            if(n != -1)
            {
               col[i][n] = true;
               row[j][n] = true;
               block[Area(i,j)][n] = true;
            }
        }
    }
    DFS(0,0);
    return ansMap;
}

int Solver::Area(int x, int y)
{
    return (x/3)*3 + (y/3) + 1;
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
        ansMap.SetData(x, y, k);
        return true;
    }
}

void Solver::Delete(int x, int y, int k)
{
    col[x][k] = false;
    row[y][k] = false;
    block[Area(x,y)][k] = false;
    ansMap.SetData(x, y, -1);
}

//这个DFS不对，重写
void Solver::DFS(int x, int y)
{
    /*if(x == 9 || y == 9)
        return;
    if(ansMap.Data(x, y) != -1)
        Next(x, y);
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
    }*/
}
