#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "block.h"
#include <QFrame>
#include <QPushButton>
#include <QPainter>
#include <QLayout>
#include <QGridLayout>
#include <cstring>
#include <QAction>
#include <QMenu>
#include <QPoint>
#include <QObject>
#include "gridLine.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_restartButton_clicked();
    //void Highlight(int,int);
    void UpdateCurBlock(int,int);

signals:
    void BlockChosen(int,int);
private:
    Ui::MainWindow *ui;
    QFrame *frame;
    QPoint curBlock;
    QAction *clearAction;
    QAction *runAction;
    QAction *quitAction;
    QAction *aboutAction;
    QAction *saveAction;
    QMenu *operaMenu;
    QMenu *helpMenu;
    Block *block[9][9];
    void SetupBlocks();
    void PaintLine();
    void SetupMenu();
};

#endif // MAINWINDOW_H
