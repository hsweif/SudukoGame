#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "block.h"
#include <QFrame>
#include <QPushButton>
#include <QPainter>
#include <QLayout>
#include <QGridLayout>
#include <QAction>
#include <QMenu>
#include <QString>
#include <QPoint>
#include <QObject>
#include <QSignalMapper>
#include <QTimer>
#include <QFont>
#include <QStack>
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
    SudukoMap CurrentMap();
    SudukoMap CurrentState();
    ~MainWindow();

protected:
    /**
     * @brief protected member
     * 保护类型成员主要包含：
     * 1. 往后可能需要扩展的函数，例如Redo(), Undo()
     * 2. 自定义的槽函数
     * 透过虚函数的设置使往后扩展功能时可以透过子类继承重写来实现。
     */
    virtual void SetupMenu();
    virtual void Undo();
    virtual void Redo();
    void SetGame();
    void FillMap(SudukoMap tmpMap);
    virtual char HighlightType();
    virtual void PushStep(int&,int&,int,QString);

protected slots:
    void UpdateCurBlock(int,int);
    void KeyPressed(int);
    void UpdateTime();
    void CheckResult();
    void ClearMap();
    void SolveGame();
    void SolveMode();
    void PlayMode();
    void VainGame();

signals:
    void BlockChosen(int,int,int,char);
    void Check();

private:
    /**
     * @brief private member:
     * 私有数据成员主要包含：
     * 	1. 构成主视窗的各式控件
     *  2. 弹出窗口的控件指针
     *  3. 游戏进程中的资料与状态记录标记
     *  4. 求解器（生成与求解数独地图）
     * 私有函数成员主要包含：
     *  1. 对窗口部件进行的初始化与版面操作
     *	2. Ui Designer生成的槽函数
     */
    Ui::MainWindow *ui;
    QFrame *frame;
    QPoint curBlock;
    QSignalMapper *keyboardMapper;
    QStack <Step*> undoArr, redoArr;
    QMenu *operaMenu;
    QMenu *helpMenu;
    QAction *runAction;
    QAction *playAction;
    QAction *quitAction;
    QAction *solveAction;
    QAction *vainAction;
    QAction *aboutAction;
    Block *block[9][9];
    InfoBox *infobox;
    Solver *sol;
    SudukoMap curMap;
    bool startFlag;
    int gameMode; //1 - game, 0 - solve
    QTimer *timer;
    int curSec, curMin;
    bool rcFlag, numFlag, processFlag;
    void SetupBlocks();
    void PaintLine();
    void KeyboardMapping();
    void TimerRestart();

private slots:
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
};

#endif // MAINWINDOW_H
