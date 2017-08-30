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
    void setEna(bool ok);
    void clearBlock();
private slots:
    void dataChange(const QString & data);
    void Highlight(int, int);

signals:
    void Chosen(int, int);

private:
    //QLineEdit *blockNum;
    QTextBrowser *blockNum;
    //QToolButton *blockNum;
    //QLabel *blockNum;
    int number;
    QPoint p;
    void mousePressEvent(QMouseEvent*);
    //bool eventFilter(QObject *watched, QEvent *event);
};

#endif // BLOCK_H
