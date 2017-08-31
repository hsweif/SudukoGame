#ifndef STEP_H
#define STEP_H

#include <QString>
#include <QObject>
#include <QPoint>

class Step: public QObject
{
    Q_OBJECT;
public:
    Step();
    void SetInstruct(QString);
    void SetPos(int, int);
    void SetValue(int);
    QString Instruct()const;
    QPoint Pos()const;
    int Value()const;

private:
    QString instruct;
    QPoint pos;
    int value;
};

#endif // STEP_H
