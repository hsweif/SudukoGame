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
    bool marked;
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
    void RemoveTail();
    void SetContent(QString);
    QString Content()const;
private slots:
    void dataChange(const QString & data);
    void Highlight(int, int, int, char);

signals:
    void Chosen(int, int);

private:
    QTextBrowser *blockNum;
    QFont font;
    int number;
    QPoint p;
    QString content;
    void mousePressEvent(QMouseEvent*);
    //void mouseReleaseEvent(QMouseEvent*);
};

#endif // BLOCK_H
