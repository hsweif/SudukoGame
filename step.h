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
    QString Instruct()const;
    QPoint Pos()const;

private:
    QString instruct;
    QPoint pos;
};

#endif // STEP_H
