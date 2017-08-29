#ifndef BLOCK_H
#define BLOCK_H
#include <QLineEdit>
#include <QRegExp>
#include <QValidator>
#include <QtGui>
#include <QPoint>

class Block :public QWidget
{
    Q_OBJECT

public:
    Block(QWidget *parent=0);
    void changeColor(const QColor &color);
    int data();
    QPoint getPos();
    void setPos(const QPoint &p);
    void setPos(int x,int y);
    void setValue(int a);
    void setEna(bool ok);
private slots:
    void dataChange(const QString & data);


private:
    QLineEdit *nubEidt;
    int da;
    QPoint p;
};

#endif // BLOCK_H
