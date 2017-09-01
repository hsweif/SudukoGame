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

private:
    int data[81];
};

#endif // SUDUKOMAP_H
