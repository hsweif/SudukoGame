#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    /*
     *	UI界面的初始化配置，混用UI Designer和代码控制
     * 	包括排版、选单以及部分控件的配置
     */
    ui->setupUi(this);
    frame = new QFrame(this);
    QHBoxLayout *hLayout = new QHBoxLayout;
    SetupBlocks();
    PaintLine();
    SetupMenu();
    KeyboardMapping();
    hLayout->addWidget(frame);
    hLayout->addWidget(ui->Dial);
    setLayout(hLayout);
    this->setFixedSize(frame->width()+250,frame->height());
    this->setWindowTitle("数独俱乐部入会测试");
    ui->levelBox->addItem("青铜");
    ui->levelBox->addItem("白银");
    ui->levelBox->addItem("黄金");
    ui->levelBox->addItem("白金");
    ui->levelBox->addItem("钻石");
    ui->levelBox->addItem("数独大师");
    /*
     *	游戏信息的初始化以及指针的实例化
     */
    timer = new QTimer(this);
    timer->setInterval(1000);
    curMin = 0; curSec = 0;
    rcFlag = false; numFlag = false;
    curBlock.setX(-1);//有效坐标必非负，以此避免非法操作
    curBlock.setY(-1);
    processFlag = true;
    curMap.Clear();
    gameMode = 1;
    startFlag = false;
    sol = new Solver;
    infobox = new InfoBox;
    /*
     *	信号与槽的连接区
     */
    connect(timer, SIGNAL(timeout()), this, SLOT(UpdateTime()));
    connect(this, SIGNAL(Check()), this, SLOT(CheckResult()));
    SetGame();
}

/**
 * @brief MainWindow::SetupMenu
 * (选单操作设置区)
 * 这是一个虚函数，以利子类重写重新定制需要的操作选单
 * 放置在选单区的操作具备以下特点：
 * 1. 在游戏进程里不会时常使用，例如模式切换
 * 2. 特意放置在不起眼处的一键解题功能
 */
void MainWindow::SetupMenu()
{
    operaMenu=new QMenu(tr("操作选单(&O)"),this);
    ui->menuBar->addMenu(operaMenu);

    playAction = new QAction(tr("玩耍模式"), this);
    connect(playAction,SIGNAL(triggered()),this,SLOT(PlayMode()));
    operaMenu->addAction(playAction);

    runAction=new QAction(tr("解题模式"),this);
    connect(runAction,SIGNAL(triggered()),this,SLOT(SolveMode()));
    operaMenu->addAction(runAction);

    solveAction = new QAction(tr("VIP一键搞定"), this);
    solveAction->setShortcut(tr("Ctrl+r"));
    connect(solveAction,SIGNAL(triggered()),this,SLOT(SolveGame()));
    operaMenu->addAction(solveAction);

    vainAction = new QAction(tr("VIP虚荣模式"));
    vainAction->setShortcut(tr("Ctrl+v"));
    connect(vainAction,SIGNAL(triggered()),this,SLOT(VainGame()));
    operaMenu->addAction(vainAction);

    quitAction=new QAction(tr("退出"),this);
    quitAction->setShortcut(tr("ctrl+q"));
    quitAction->setStatusTip("退出程序");
    connect(quitAction,SIGNAL(triggered()),this,SLOT(close()));
    operaMenu->addAction(quitAction);

    helpMenu=new QMenu(tr("帮助(&H)"),this);
    ui->menuBar->addMenu(helpMenu);
    aboutAction=new QAction(tr("关于"),this);
    aboutAction->setShortcut(tr("Ctrl+I"));
    aboutAction->setStatusTip("关于数独计算器");
    connect(aboutAction,SIGNAL(triggered()),this,SLOT(about()));
    helpMenu->addAction(aboutAction);
}

/**
 * @brief MainWindow::SolveMode
 * @brief MainWindow::PlayMode
 * @brief MainWindow::VainMode
 * 这三个函数主要通过gameMode的判定来决定当前是何种游戏模式
 * 以进行不同的操作
 */
void MainWindow::SolveMode()
{
    TimerRestart();
    timer->stop();
    gameMode = 0;
    this->ClearMap();
    ui->startButton->setText("求解");
}

void MainWindow::PlayMode()
{
    TimerRestart();
    gameMode = 1;
    this->SetGame();
    ui->startButton->setText("开始");
}

void MainWindow::VainGame()
{
    bool flag = false;
    if(gameMode)
    {
        SolveGame();
        for(int i = 0; i < 9; i++)
        {
            if(flag)
                break;
            for(int j = 0; j < 9; j++)
            {
                if(!curMap.Original(i,j))
                {
                    if(flag)
                        break;
                    block[i][j]->clearBlock();
                    flag = true;
                }
            }
        }
    }
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

/**
 * @brief MainWindow::SetupBlocks
 * 这是九宫格设置函数，
 * 主要考量到Ui Designer在处理大量重复控件时的效率不及代码，
 * block[][]是自定义数据结构Block的二维数组
 */
void MainWindow::SetupBlocks()
{
    for(int i = 0; i < 9; i ++)
    {
        for(int j = 0; j < 9; j ++)
        {
            block[i][j] = new Block(frame);
            block[i][j]->SetPos(i,j);
            block[i][j]->move(j*50, i*50);
            //通过this的信号与槽的转接连接每个方格和全局的操作
            connect(block[i][j], SIGNAL(Chosen(int,int)), this, SLOT(UpdateCurBlock(int,int)));
            connect(this, SIGNAL(BlockChosen(int,int,int,char)), block[i][j], SLOT(Highlight(int,int,int,char)));
        }
    }
    int width = 50*9 + 1;
    frame->setMinimumSize(width, width);
}

/**
 * @brief MainWindow::KeyboardMapping
 * 键盘映射函数，通过QSignalMapper建立按键区与数字的对应关系
 */
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

/**
 * @brief MainWindow::KeyPressed
 * @param num
 * 连接个别方格和全局操作的中介槽函数
 * 每在有状态更新的时候发射信号
 * (当pause的时候不发射信号，通过processFlag判断）
 */
void MainWindow::KeyPressed(int num)
{
    int && _x = curBlock.x(), && _y = curBlock.y();
    if(processFlag)
    {
        if(_x >= 0 && _y >= 0 && !curMap.Original(_x, _y)) {
            block[_x][_y]->AddValue(num);
        }
        else if(curMap.BlankMap()) {
            block[_x][_y]->AddValue(num);
        }
        PushStep(_x, _y, num, "click");
        emit Check();
        emit BlockChosen(_x, _y, num, HighlightType());
    }
}

/**
 * @brief MainWindow::Undo
 * @brief MainWindow::Redo
 * Undo 的操作是后进先出，于是使用Stack储存每个操作
 * Pop出来的丢到redoArr里面（同样是个Stack）
 * 当有新操作时清空RedoArr，如此便可实现若干步的还原与撤销
 * 通过约定的字符串与坐标定义各个操作
 */
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

void MainWindow::PushStep(int &x, int &y, int num, QString qstr)
{
    Step *tmpStep = new Step;
    tmpStep->SetInstruct(qstr);
    tmpStep->SetPos(x, y);
    tmpStep->SetValue(num);
    undoArr.push(tmpStep);
    redoArr.clear();
}

void MainWindow::UpdateCurBlock(int _x, int _y)
{
    curBlock.setX(_x);
    curBlock.setY(_y);
    if(processFlag)
    {
        emit BlockChosen(_x, _y,block[_x][_y]->num(),HighlightType());
        timer->start(1000);
    }
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

/**
 * @brief MainWindow::CheckResult
 * 胜利状态检查函数
 */
void MainWindow::CheckResult()
{
    SudukoMap curM = CurrentState();
    if(sol->Check(curM))
    {
        this->infobox->setWindowTitle("欢迎您加入数独大师俱乐部");
        this->infobox->SetText("太厉害了！您已经赢得这场游戏，获得加入数独大师俱乐部的资格，扫码输入验证码233即可入会！");
        this->infobox->show();
        timer->stop();
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
            verticalLine[i]->move(0,block[i*3-1][0]->pos().y()+block[i*3-1][0]->height() + 19);//+ui->menuBar->height()-2);
        }
        else
        {
            horizontalLine[i]->move(0,0);
            verticalLine[i]->move(0,0);
        }
    }
}

/**
 * @brief MainWindow::SetGame
 * 往求解器里面丢参数得到不同难易度的地图并设置
 */
void MainWindow::SetGame()
{
    startFlag = true;
    int level = ui->levelBox->currentIndex() + 1;
    ClearMap();
    curMap = sol->GenerateMap(level);
    FillMap(curMap);
}

/**
 * @brief MainWindow::CurrentMap
 * @brief MainWindow::CurrentState
 * Map 是返回当前的地图设置, State是返回包括用户填写数字的全部状态
 */
SudukoMap MainWindow::CurrentMap()
{
    return curMap;
}

SudukoMap MainWindow::CurrentState()
{
    SudukoMap tmp;
    for(int i = 0; i < 9; i ++)
        for(int j = 0; j < 9; j++)
        {
            tmp.SetData(i, j, block[i][j]->num());
            tmp.SetOriginal(i, j, true);
        }
    return tmp;
}

void MainWindow::ClearMap()
{
    curBlock.setX(-1);
    curBlock.setY(-1);
    for(int i = 0; i < 9; i ++)
    {
        for(int j = 0; j < 9; j ++)
        {
            block[i][j]->clearBlock();
            block[i][j]->ChangeColor("background");
            block[i][j]->SetFontType(block[i][j]->FontPolicy());
            block[i][j]->SetEna(true);
            block[i][j]->AddValue(-1);
            block[i][j]->marked = false;
        }
    }
}

void MainWindow::FillMap(SudukoMap tmpMap)
{
    ClearMap();
    for(int i = 0; i < 9; i ++)
    {
        for(int j = 0; j < 9; j++)
        {
            if(!tmpMap.Original(i,j)){
                block[i][j]->SetEna(true);
                block[i][j]->ChangeColor("background");
            }
            else
            {
                block[i][j]->SetEna(false);
                block[i][j]->ChangeColor("map");
            }
            block[i][j]->AddValue(tmpMap.Data(i,j));
        }
    }
}

void MainWindow::TimerRestart()
{
    QTimer *tmp = timer;
    timer = new QTimer(this);
    timer->setInterval(1000);
    timer->start(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(UpdateTime()));
    disconnect(tmp, SIGNAL(timeout()), this, SLOT(UpdateTime()));
    delete tmp;
    curSec =0; curMin = 0;
    ui->minute->setText("00");
    ui->second->setText("00");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_restartButton_clicked()
{
    if(startFlag)
    {
        TimerRestart();
        processFlag = true;
        FillMap(curMap);
    }
}

void MainWindow::on_clearButton_clicked()
{
    int &&x = curBlock.x(), &&y = curBlock.y();
    if(x >= 0 && y >= 0 && !curMap.Original(x, y) && processFlag)
    {
        QString tmpStr = block[x][y]->Content();
        block[x][y]->clearBlock();
        PushStep(x, y, -2, tmpStr);//-2 as a signal.
        emit BlockChosen(x, y, -1, HighlightType());
    }
}

void MainWindow::on_startButton_clicked()
{
    TimerRestart();
    processFlag = true;
    SetGame();
}

void MainWindow::on_Pause_clicked()
{
    if(startFlag)
    {
        timer->stop();
        processFlag = false;
    }
}

void MainWindow::on_Resume_clicked()
{
    if(startFlag)
    {
        timer->start(1000);
        processFlag = true;
    }
}


void MainWindow::on_markButton_clicked()
{
    if(processFlag)
    {
        int && _x = curBlock.x(), && _y = curBlock.y(), && _n = block[_x][_y]->num();
        bool flag = block[_x][_y]->marked;
        block[_x][_y]->marked = !flag;
        int tmp = block[_x][_y]->num();
        emit BlockChosen(_x, _y, tmp, HighlightType());
        PushStep(_x, _y, _n, "mark");
    }
}

void MainWindow::on_checkRC_clicked(bool checked)
{
    rcFlag = checked;
    int && x = curBlock.x(), && y = curBlock.y();
    if(x >= 0 && y >= 0)
    {
        int tmp = block[curBlock.x()][curBlock.y()]->num();
        if(processFlag)
            emit BlockChosen(curBlock.x(), curBlock.y(), tmp, HighlightType());
    }
}

void MainWindow::on_checkNum_clicked(bool checked)
{
    numFlag = checked;
    int && x = curBlock.x(), && y = curBlock.y();
    if(x >= 0 && y >= 0)
    {
        int tmp = block[curBlock.x()][curBlock.y()]->num();
        if(processFlag)
            emit BlockChosen(curBlock.x(), curBlock.y(), tmp, HighlightType());
    }
}

void MainWindow::on_undoButton_clicked()
{
    if(!undoArr.empty() && processFlag)
        Undo();
}

void MainWindow::on_redoButton_clicked()
{
    if(!redoArr.empty() && processFlag)
       Redo();
}

/**
 * @brief MainWindow::SolveGame
 * 通过求解器的求解功能返回答案地图并设置
 */
void MainWindow::SolveGame()
{
    SudukoMap tmpMap;
    if(gameMode)
    {
        if(!curMap.BlankMap())
        {
            tmpMap = sol->Solve(curMap);
            FillMap(tmpMap);
        }
    }
    else
    {
        SudukoMap ans, tmp = CurrentState();
        ans = sol->Solve(tmp);
        FillMap(tmpMap);
    }
    this->infobox->setWindowTitle("已使用VIP功能");
    this->infobox->SetText("您好，您方才试用的是高级会员的专属功能。扫码输入666即可获得往后测试功能的优先试用权！");
    this->infobox->show();
    timer->stop();
}
