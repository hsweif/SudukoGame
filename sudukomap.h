#ifndef SUDUKOMAP_H
#define SUDUKOMAP_H
#include <cstring>
class SudukoMap
{
public:
    SudukoMap();
    int	Data(int,int)const;
    void SetData(int,int,int);
    void Clear();
    bool Original(int x, int y);
    void SetOriginal(int x, int y, bool flag);
    bool BlankMap();

private:
    int data[81];
    bool original[81];
};

#endif // SUDUKOMAP_H
