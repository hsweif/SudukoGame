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
#include <QString>
#include <QPoint>
#include <QObject>
#include <QSignalMapper>
#include <QTimer>
#include <QFont>
#include <QStack>
#include <QTextStream>
#include <QFile>
#include <fstream>
#include <string>
#include "gridLine.h"
#include "step.h"
#include "sudukomap.h"
#include "solver.h"
#include "infobox.h"

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
    //void Highlight(int,int);
    void UpdateCurBlock(int,int);
    void KeyPressed(int);
    void UpdateTime();
    void CheckResult();
    void ClearMap();
    void SolveGame();
    void SolveMode();
    void PlayMode();
    void VainGame();
    /*
     *	Ui Designer生成的槽函数
     */
    void on_startButton_clicked();
    void on_restartButton_clicked();
    void on_Pause_clicked();
    void on_clearButton_clicked();
    void on_Resume_clicked();
    void on_markButton_clicked();
    void on_checkRC_clicked(bool checked);
    void on_checkNum_clicked(bool checked);
    void on_undoButton_clicked();
    void on_redoButton_clicked();

signals:
    void BlockChosen(int,int,int,char);
    void Check();

private:
    Ui::MainWindow *ui;
    QFrame *frame;
    QPoint curBlock;
    QSignalMapper *keyboardMapper;
    QTimer *timer;
    QStack <Step*> undoArr, redoArr;
    QAction *runAction;
    QAction *playAction;
    QAction *quitAction;
    QAction *solveAction;
    QAction *vainAction;
    QMenu *operaMenu;
    QMenu *helpMenu;
    QAction *aboutAction;
    //QVector <SudukoMap> gameData;
    int curSec, curMin;
    Block *block[9][9];
    InfoBox *infobox;
    Solver *sol;
    SudukoMap curMap;
    bool startFlag;
    int gameMode; //1 - game, 0 - solve
    bool rcFlag, numFlag, processFlag;
    void SetupBlocks();
    void PaintLine();
    void KeyboardMapping();
    void SetupMenu();
    void Undo();
    void Redo();
    void TimerRestart();
    //void ReadData();
    void SetGame();
    void FillMap(SudukoMap tmpMap);
    char HighlightType();
    SudukoMap CurrentMap();
    SudukoMap CurrentState();
    void PushStep(int&,int&,int,QString);
    //void CheckCurBlock();
};

#endif // MAINWINDOW_H
