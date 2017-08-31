#include "step.h"

Step::Step()
{

}

void Step::SetPos(int _x, int _y)
{
    pos.setX(_x);
    pos.setY(_y);
}

void Step::SetInstruct(QString ins)
{
    instruct = ins;
}

QString Step::Instruct()const
{
    return instruct;
}

QPoint Step::Pos() const
{
    return pos;
}
