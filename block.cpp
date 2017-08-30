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
    //this->changeColor(Qt::yellow);
    this->setValue(3);
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

void Block::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {
        this->changeColor(Qt::yellow);
        this->setValue(1);
    }
}

int Block::data()
{
    return da;
}

void Block::clearBlock()
{
    blockNum->setText("");
    da=-1;
}

void Block::setValue(int a)
{
    if(a<1)
    {
        blockNum->setText("");
        da=-1;
    }
    else
    {
        blockNum->setText(QString::number(a));
        da=a;
    }

}

void Block::setEna(bool ok)
{
    blockNum->setEnabled(ok);
}


void Block::dataChange(const QString &data)
{
    if(data.isEmpty())
        da=-1;
    else
        da=data.toInt();
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
