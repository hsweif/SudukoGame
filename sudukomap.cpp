#include "sudukomap.h"

SudukoMap::SudukoMap()
{
    //To check
    Clear();
}

int SudukoMap::Data(int x, int y)const
{
    return data[x*9 + y];
}

void SudukoMap::SetData(int x, int y, int num)
{
    data[x*9 + y] = num;
}

void SudukoMap::Clear()
{
    for(int i = 0; i < 81; i++)
    {
        data[i] = -1;
        original[i] = true;
    }
}

bool SudukoMap::Original(int x, int y)
{
    return original[x*9 + y];
}

void SudukoMap::SetOriginal(int x, int y, bool flag)
{
    original[x*9 + y] = flag;
}
