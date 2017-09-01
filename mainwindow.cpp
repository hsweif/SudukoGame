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
    ReadData();
    /*
     *	UI界面的初始化配置，混用UI Designer和代码控制
     * 	包括一些排版以及选单的配置
     */
    KeyboardMapping();
    hLayout->addWidget(frame);
    hLayout->addLayout(ui->Dial);
    setLayout(hLayout);
    this->setFixedSize(frame->width()+180,frame->height());
    this->setWindowTitle("Suduko Game!");
    ui->levelBox->addItem("Hard");
    ui->levelBox->addItem("Normal");
    ui->levelBox->addItem("Easy");
    //ui->centralWidget->setMouseTracking(true);
    /*
     * 计时器的初始化设置
     */
    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(UpdateTime()));
    curMin = 0; curSec = 0;
    /*
     * 其他数据成员的初始化区域
     */
    rcFlag = false; numFlag = false;
    curBlock.setX(-1);
    curBlock.setY(-1);
    sol = new Solver;
}

void MainWindow::UpdateTime()
{
    QString tmpS, tmpM;
    curSec ++;
    if(curSec >= 60)
    {
        curMin ++;
        curSec -= 60;
    }
    if(curSec < 10)
    {
        tmpS.append(QString::number(0));
    }
    if(curMin < 10)
    {
        tmpM.append(QString::number(0));
    }
    tmpS.append(QString::number(curSec));
    tmpM.append(QString::number(curMin));
    ui->minute->setText(tmpM);
    ui->second->setText(tmpS);
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
            connect(block[i][j], SIGNAL(Chosen(int,int)), this, SLOT(UpdateCurBlock(int,int)));
            connect(this, SIGNAL(BlockChosen(int,int,int,char)), block[i][j], SLOT(Highlight(int,int,int,char)));
        }
    }
    int width = 50*9 + 1;
    frame->setMinimumSize(width, width);
}

void MainWindow::KeyboardMapping()
{
    keyboardMapper = new QSignalMapper(this);
    connect(ui->toolButton_1, SIGNAL(clicked(bool)), keyboardMapper, SLOT(map()));
    keyboardMapper->setMapping(ui->toolButton_1, 1);
    connect(ui->toolButton_2, SIGNAL(clicked(bool)), keyboardMapper, SLOT(map()));
    keyboardMapper->setMapping(ui->toolButton_2, 2);
    connect(ui->toolButton_3, SIGNAL(clicked(bool)), keyboardMapper, SLOT(map()));
    keyboardMapper->setMapping(ui->toolButton_3, 3);
    connect(ui->toolButton_4, SIGNAL(clicked(bool)), keyboardMapper, SLOT(map()));
    keyboardMapper->setMapping(ui->toolButton_4, 4);
    connect(ui->toolButton_5, SIGNAL(clicked(bool)), keyboardMapper, SLOT(map()));
    keyboardMapper->setMapping(ui->toolButton_5, 5);
    connect(ui->toolButton_6, SIGNAL(clicked(bool)), keyboardMapper, SLOT(map()));
    keyboardMapper->setMapping(ui->toolButton_6, 6);
    connect(ui->toolButton_7, SIGNAL(clicked(bool)), keyboardMapper, SLOT(map()));
    keyboardMapper->setMapping(ui->toolButton_7, 7);
    connect(ui->toolButton_8, SIGNAL(clicked(bool)), keyboardMapper, SLOT(map()));
    keyboardMapper->setMapping(ui->toolButton_8, 8);
    connect(ui->toolButton_9, SIGNAL(clicked(bool)), keyboardMapper, SLOT(map()));
    keyboardMapper->setMapping(ui->toolButton_9, 9);
    connect(keyboardMapper, SIGNAL(mapped(int)), this, SLOT(KeyPressed(int)));
}

void MainWindow::KeyPressed(int num)
{
    int && _x = curBlock.x(), && _y = curBlock.y();
    if(_x > 0 && _y > 0 && block[_x][_y]->Enable()) {
        block[_x][_y]->AddValue(num);
    }
    PushStep(_x, _y, num, "click");
    emit BlockChosen(_x, _y, num, HighlightType());
    //CheckCurBlock();
}

//Todo...
void MainWindow::Undo()
{
    Step *tmp = undoArr.pop();
    int && _x = tmp->Pos().x(), && _y = tmp->Pos().y(), && _v = tmp->Value();
    redoArr.push(tmp);
    QString && qstr = tmp->Instruct();
    if(qstr == "click") {
        block[_x][_y]->RemoveTail();
        if(block[_x][_y]->Content().size() == 0)
            block[_x][_y]->AddValue(-1);
    }
    else if(qstr == "mark") {
        bool tmpMark = block[_x][_y]->marked;
        block[_x][_y]->marked = !tmpMark;
    }
    else if(_v == -2) {
        block[_x][_y]->SetContent(tmp->Instruct());
    }
    curBlock.setX(_x);
    curBlock.setY(_y);
    int && _n = block[_x][_y]->num();
    if(_n > 0)
        emit BlockChosen(_x, _y, _n, HighlightType());
    else
        emit BlockChosen(_x, _y, 0, HighlightType());

}

void MainWindow::Redo()
{
    Step *tmp = redoArr.pop();
    int && _x = tmp->Pos().x(), && _y = tmp->Pos().y(), && _n = tmp->Value();
    undoArr.push(tmp);
    QString && qstr = tmp->Instruct();
    if(qstr == "click") {
        block[_x][_y]->AddValue(_n);
    }
    else if(qstr == "mark") {
        bool tmpMark = block[_x][_y]->marked;
        block[_x][_y]->marked = !tmpMark;
    }
    else if(_n == -2) {
        block[_x][_y]->clearBlock();
    }
    curBlock.setX(_x);
    curBlock.setY(_y);
    emit BlockChosen(_x, _y, block[_x][_y]->num(), HighlightType());
}

/*void MainWindow::CheckCurBlock()
{
    if(block[curBlock.x()][curBlock.y()]->num() != -1)
    {
        for(int i = 0; i < 9; i++)
        {
            if(block[curBlock.x()][curBlock.y()]->num() == block[curBlock.x()][i]->num()
                    && curBlock.y() != i)
            {
                block[curBlock.x()][curBlock.y()]->changeColor(Qt::red);
                break;
            }
            if(block[curBlock.x()][curBlock.y()]->num() == block[i][curBlock.y()]->num()
                    && curBlock.x() != i)
            {
                block[curBlock.x()][curBlock.y()]->changeColor(Qt::red);
                break;
            }
            //九宫格的待填充
        }
    }

}*/

void MainWindow::UpdateCurBlock(int _x, int _y)
{
    curBlock.setX(_x);
    curBlock.setY(_y);
    emit BlockChosen(_x, _y,block[_x][_y]->num(),HighlightType());
    timer->start(1000);
}

/**
 * @brief MainWindow::HighlightType
 * 负责判断当前时哪一种高亮形式（行列、数字等）
 * 以一个字符传递信息：
 * b(both) / r(row & column) / n(number) / x (none)
 */
char MainWindow::HighlightType()
{
    if(rcFlag && numFlag) {
        return 'b';
    }
    else if(rcFlag) {
        return 'r';
    }
    else if(numFlag) {
        return 'n';
    }
    else {
        return 'x';
    }
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

void MainWindow::PushStep(int &x, int &y, int num, QString qstr)
{
    //不知道为啥得用指针
    Step *tmpStep = new Step;
    tmpStep->SetInstruct(qstr);
    tmpStep->SetPos(x, y);
    tmpStep->SetValue(num);
    undoArr.push(tmpStep);
    redoArr.clear();
}

//Todo...there are lots of bugs bug don't know why...
void MainWindow::ReadData()
{
    SudukoMap tmpMap;
    QFile file(":/gameMap_1.txt");
    QTextStream in(&file);
    QString tmpstr;
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug()<<"Can't open the file!";
    }
    /*while( !in.atEnd()){
        qDebug()<<"open the file!";
        QString line = in.readLine();
        qDebug() << line;
    }*/
    while(!in.atEnd())
    {
        tmpMap.Clear();
        for(int t = 0; t < 9; t++)
        {
            int count = 0;
            tmpstr = in.readLine();
            //qDebug() << tmpstr;
            for(int i = 0; i < tmpstr.size(); i++)
            {
               if(count >= 9)
                   break;
               if(tmpstr[i] == ' ')
                   continue;
               if(tmpstr[i] == '_')
                   tmpMap.SetData(t, count++, -1);
               else {
                   std::string stdstr = tmpstr.toStdString();
                   int tmpNum = stdstr[i] - '0';
                   //qDebug() << tmpNum;
                   tmpMap.SetData(t, count++, tmpNum);
               }
            }
        }
        gameData.push_back(tmpMap);
        for(int i = 0; i < 3; i++)//读取分割线与空白
            tmpstr = in.readLine();
     }
    file.close();
    //qDebug()<<"current applicationDirPath: "<<QCoreApplication::applicationDirPath();
    //qDebug()<<"current currentPath: "<<QDir::currentPath();
}

//Test, to do...
void MainWindow::SetGame()
{
    ClearMap();
    int sz = gameData.size();
    int randNum = rand() % sz;
    if(!gameData.empty())
    {
        SudukoMap & tmpMap = gameData[randNum];
        FillMap(tmpMap);
        /*for(int i = 0; i < 9; i ++)
        {
            for(int j = 0; j < 9; j++)
            {
                if(tmpMap.Data(i,j) == -1) {
                    block[i][j]->SetEna(true);
                }
                else
                {
                    block[i][j]->AddValue(tmpMap.Data(i,j));
                    block[i][j]->SetEna(false);
                    block[i][j]->changeColor(Qt::gray);
                }
            }
        }*/
    }
}

SudukoMap MainWindow::CurrentMap()
{
    SudukoMap tmp;
    for(int i = 0; i < 9; i ++)
    {
        for(int j = 0; j < 9; j++)
        {
            if(!block[i][j]->Enable())
                tmp.SetData(i, j, block[i][j]->num());
        }
    }
    return tmp;
}

void MainWindow::ClearMap()
{
    for(int i = 0; i < 9; i ++)
    {
        for(int j = 0; j < 9; j ++)
        {
            block[i][j]->clearBlock();
            block[i][j]->changeColor(Qt::white);
            block[i][j]->SetEna(true);
        }
    }
}

void MainWindow::FillMap(SudukoMap &tmpMap)
{
    ClearMap();
    for(int i = 0; i < 9; i ++)
    {
        for(int j = 0; j < 9; j++)
        {
            if(tmpMap.Data(i,j) == -1) {
                block[i][j]->SetEna(true);
            }
            else
            {
                block[i][j]->AddValue(tmpMap.Data(i,j));
                block[i][j]->SetEna(false);
                block[i][j]->changeColor(Qt::gray);
            }
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_restartButton_clicked()
{
    ClearMap();
    QTimer *tmp = timer;
    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(UpdateTime()));
    disconnect(tmp, SIGNAL(timeout()), this, SLOT(UpdateTime()));
    delete tmp;
    curSec =0; curMin = 0;
    ui->minute->setText("00");
    ui->second->setText("00");
}

void MainWindow::on_clearButton_clicked()
{
    int &&x = curBlock.x(), &&y = curBlock.y();
    QString tmpStr = block[x][y]->Content();
    block[x][y]->clearBlock();
    PushStep(x, y, -2, tmpStr);//-2 as a signal.
}

void MainWindow::on_startButton_clicked()
{
    timer->start(1000);
    SetGame();
}

void MainWindow::on_Pause_clicked()
{
    timer->stop();
}

void MainWindow::on_Resume_clicked()
{
    timer->start(1000);
}


void MainWindow::on_markButton_clicked()
{
    int && _x = curBlock.x(), && _y = curBlock.y(), && _n = block[_x][_y]->num();
    bool flag = block[_x][_y]->marked;
    block[_x][_y]->marked = !flag;
    int tmp = block[_x][_y]->num();
    emit BlockChosen(_x, _y, tmp, HighlightType());
    PushStep(_x, _y, _n, "mark");
}

void MainWindow::on_checkRC_clicked(bool checked)
{
    rcFlag = checked;
    int tmp = block[curBlock.x()][curBlock.y()]->num();
    emit BlockChosen(curBlock.x(), curBlock.y(), tmp, HighlightType());
}

void MainWindow::on_checkNum_clicked(bool checked)
{
    numFlag = checked;
    int tmp = block[curBlock.x()][curBlock.y()]->num();
    emit BlockChosen(curBlock.x(), curBlock.y(), tmp, HighlightType());
}

void MainWindow::on_undoButton_clicked()
{
    if(!undoArr.empty())
        Undo();
}

void MainWindow::on_redoButton_clicked()
{
    if(!redoArr.empty())
        Redo();
}

//Todo
void MainWindow::on_solveButton_clicked()
{
    //SudukoMap tmpMap = sol->Solve(CurrentMap());
    //FillMap(tmpMap);
}
