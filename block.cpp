#include "block.h"

Block::Block(QWidget *parent)
    :QWidget(parent)
{
    blockNum=new QTextBrowser(this);
    blockNum->setAlignment(Qt::AlignCenter);
    font.setPixelSize(30);
    blockNum->setFont(font);
    blockNum->setFixedSize(50,50);
    blockNum->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    blockNum->installEventFilter(this);
    blockNum->setContextMenuPolicy(Qt::NoContextMenu);
    /*blockNum->setStyleSheet("QTextBrowser{"
                            "font: 24pt American Typewriter;"
                            "border-width: 1px;"
                            "text-align: center;"
                            "border-color: rgb(135, 206, 250);"
                            "background-color: rgb(240, 248, 255);}");*/

    marked = false;
    //this->setMouseTracking(true);
    number = -1;
    enaFlag = true;
}

//Mac 上用左键选取会有种迷之bug。。。
void Block::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::RightButton) {
        emit Chosen(this->p.x(), this->p.y());
    }
}

QString Block::Content()const
{
    return content;
}

bool Block::Enable()const
{
    return enaFlag;
}
void Block::RemoveTail()
{
    int tmpSz = content.size();
    content.remove(tmpSz-1, 1);
    if(content.size() > 2) {
        font.setPixelSize(18);
    }
    else {
        font.setPixelSize(30);
    }
    blockNum->setFont(font);
    blockNum->setText(content);
}

void Block::SetContent(QString qstr)
{
    content = qstr;
    blockNum->setText(content);
}

/*void Block::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton) {
        qDebug() << "Left";
        emit Chosen(this->p.x(), this->p.y());
    }
}*/

void Block::Highlight(int _x, int _y, int _num, char _type)
{
    if(marked) {
        this->changeColor(Qt::green);
    }
    else if((_type == 'r' || _type == 'b')
          && (p.x() == _x || p.y() == _y))
    {
        this->changeColor(Qt::yellow);
    }
    else if((_type == 'b' || _type == 'n')
            && number == _num && number != -1)
    {
        this->changeColor(Qt::yellow);
    }
    else if(p.x() == _x && p.y() == _y)
    {
        this->changeColor(Qt::yellow);
    }
    else if(!enaFlag)
    {
        this->changeColor(Qt::gray);
    }
    else{
        this->changeColor(Qt::white);
    }
}

int Block::num()const
{
    return number;
}

void Block::clearBlock()
{
    content.clear();
    //qDebug() << "block size: " << content.size();
    blockNum->setText("");
    number = -1;
}

void Block::AddValue(int _num)
{
    if(content.size() == 0) {
        number = _num;
    }
    if(_num > 0)
    {
        content.append(QString::number(_num));
        if(content.size() > 2) {
            font.setPixelSize(18);
        }
        else {
            font.setPixelSize(30);
        }
        blockNum->setFont(font);
        blockNum->setText(content);
    }
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

void Block::SetEna(bool ok)
{
    enaFlag = ok;
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
