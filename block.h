#ifndef BLOCK_H
#define BLOCK_H
#include <QLineEdit>
#include <QRegExp>
#include <QValidator>
#include <QtGui>
#include <QPoint>
#include <QTextBrowser>
#include <QMouseEvent>
#include <QEvent>
#include <QTextBrowser>
#include <QDebug>

class Block :public QWidget
{
    Q_OBJECT

public:
    Block(QWidget *parent=0);
    void changeColor(const QColor &color);
    int num()const;
    QPoint getPos();
    void setPos(const QPoint &p);
    void setPos(int x,int y);
    void setValue(int a);
    void AddValue(int);
    void setEna(bool ok);
    void clearBlock();
private slots:
    void dataChange(const QString & data);
    void Highlight(int, int);

signals:
    void Chosen(int, int);

private:
    QTextBrowser *blockNum;
    QFont font;
    QString content;
    int number;
    QPoint p;
    void mousePressEvent(QMouseEvent*);
    //void mouseReleaseEvent(QMouseEvent*);
};

#endif // BLOCK_H
