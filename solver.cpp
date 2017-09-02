#include "solver.h"

Solver::Solver()
{
}

SudukoMap Solver::Solve(SudukoMap mp)
{
    todoMap = mp;
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
        todoMap.SetData(x, y, k);
        return true;
    }
}

void Solver::Delete(int x, int y, int k)
{
    col[x][k] = false;
    row[y][k] = false;
    block[Area(x,y)][k] = false;
    todoMap.SetData(x, y, -1);
}

void Solver::DFS(int x, int y)
{
    //qDebug() << x << y;
    if(x == 9 && y == 0)
    {
        //qDebug() << "hello";
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
