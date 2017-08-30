#include "block.h"

Block::Block(QWidget *parent)
    :QWidget(parent)
{
    blockNum=new QTextBrowser(this);
    //blockNum = new QToolButton(this);
    //blockNum = new QLabel(this);
    //blockNum->setFrameStyle(QFrame::Panel);
    blockNum->setAlignment(Qt::AlignCenter);
    QFont font;
    font.setPixelSize(30);
    blockNum->setFont(font);
    blockNum->setFixedSize(50,50);
    blockNum->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    blockNum->installEventFilter(this);
    blockNum->setContextMenuPolicy(Qt::NoContextMenu);
    /*
    //限制住格子内只能输入1-9
    QRegExp regExp("[1-9]{1}");
    blockNum->setValidator(new QRegExpValidator(regExp,this));
    connect(blockNum,SIGNAL(textChanged(QString)),this,SLOT(dataChange(QString)));
    */
}

/*bool Block::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == blockNum)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if(mouseEvent->button() == Qt::LeftButton)
            {
                this->changeColor(Qt::yellow);
                return true;
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}*/

//Mac 上用左键选取会有种迷之bug。。。
void Block::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::RightButton) {
        emit Chosen(this->p.x(), this->p.y());
    }
}

void Block::Highlight(int _x, int _y)
{
    if(p.x() == _x && p.y() == _y) {
        this->changeColor(Qt::yellow);
    }
    else if(this->palette() != Qt::red){
        this->changeColor(Qt::white);
    }
}

int Block::num()const
{
    return number;
}

void Block::clearBlock()
{
    blockNum->setText("");
    number = -1;
}

void Block::setValue(int a)
{
    if(a<1)
    {
        blockNum->setText("");
        number =-1;
    }
    else
    {
        blockNum->setText(QString::number(a));
        number =a;
    }

}

void Block::setEna(bool ok)
{
    blockNum->setEnabled(ok);
}


void Block::dataChange(const QString &data)
{
    if(data.isEmpty())
        number=-1;
    else
        number=data.toInt();
}

void Block::changeColor(const QColor &color)
{
    QPalette pale;
    pale.setColor(QPalette::Base,color);
    blockNum->setPalette(pale);
}

QPoint Block::getPos()
{
    return p;
}

void Block::setPos(int x, int y)
{
    QPoint point;
    point.setX(x);
    point.setY(y);
    this->p=point;
}

void Block::setPos(const QPoint &p)
{
    this->p=p;
}
