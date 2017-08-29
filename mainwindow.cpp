#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    frame = new QFrame(this);
    QHBoxLayout *hLayout = new QHBoxLayout;
    //QPushButton *button = new QPushButton;
    SetupBlocks();
    PaintLine();
    SetupMenu();
    hLayout->addWidget(frame);
    hLayout->addLayout(ui->Dial);
    setLayout(hLayout);
    //this->setCentralWidget(frame);
    this->setFixedSize(frame->width()+155,frame->height());//+ui->statusBar->height()+ui->menuBar->height());
    this->setWindowTitle("Suduko Game!");
}

void MainWindow::SetupBlocks()
{
    for(int i = 0; i < 9; i ++)
    {
        for(int j = 0; j < 9; j ++)
        {
            block[i][j] = new Block(frame);
            block[i][j]->setPos(i,j);
            block[i][j]->move(j*50, i*50);
        }
    }
    int width = 50*9 + 1;
    frame->setMinimumSize(width, width);
}

void MainWindow::PaintLine()
{
    GLine *horizontalLine[4], *verticalLine[4];
    QColor color;
    QRgb rgb=qRgb(0,140,0);
    color.setRgb(rgb);
    for(int i = 0; i < 4; i ++)
    {
        horizontalLine[i] = new GLine(frame);
        verticalLine[i] = new GLine(frame);
        horizontalLine[i]->setPen(3,color);
        verticalLine[i]->setPen(3,color);
        horizontalLine[i]->draw(1,frame->height());
        verticalLine[i]->draw(0,frame->width());
        if(i>0)
        {
            horizontalLine[i]->move(block[0][i*3+1]->pos().x()+block[0][i*3+1]->width()-2,0);
            verticalLine[i]->move(0,block[i*3-1][0]->pos().y()+block[i*3-1][0]->height()+ui->menuBar->height()-2);
        }
        else
        {
            horizontalLine[i]->move(0,0);
            verticalLine[i]->move(0,0);
        }
    }
}

void MainWindow::SetupMenu()
{
    operaMenu=new QMenu(tr("操作(&O)"),this);
    ui->menuBar->addMenu(operaMenu);

    runAction=new QAction(tr("执行"),this);
    runAction->setShortcut(tr("Ctrl+r"));
    runAction->setStatusTip("计算数独解法");
    //connect(runAction,SIGNAL(triggered()),this,SLOT(run()));
    operaMenu->addAction(runAction);

    saveAction = new QAction(tr("储存"), this);
    operaMenu->addAction(saveAction);

    clearAction=new QAction(tr("初始化"),this);
    clearAction->setShortcut(tr("ctrl+d"));
    clearAction->setStatusTip("初始化当前数独表");
    //connect(clearAction,SIGNAL(triggered()),this,SLOT(clear()));
    operaMenu->addAction(clearAction);

    quitAction=new QAction(tr("退出"),this);
    quitAction->setShortcut(tr("ctrl+q"));
    quitAction->setStatusTip("退出程序");
    //connect(quitAction,SIGNAL(triggered()),this,SLOT(close()));
    operaMenu->addAction(quitAction);

    helpMenu=new QMenu(tr("帮助(&H)"),this);
    ui->menuBar->addMenu(helpMenu);
    aboutAction=new QAction(tr("关于"),this);
    aboutAction->setShortcut(tr("Ctrl+I"));
    aboutAction->setStatusTip("关于数独计算器");
    //connect(aboutAction,SIGNAL(triggered()),this,SLOT(about()));
    helpMenu->addAction(aboutAction);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_restartButton_clicked()
{
    for(int i = 0; i < 9; i ++)
    {
        for(int j = 0; j < 9; j ++)
        {
            block[i][j]->clearBlock();
        }
    }
}
