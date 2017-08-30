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
}

//Mac 上用左键选取会有种迷之bug。。。
void Block::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::RightButton) {
        emit Chosen(this->p.x(), this->p.y());
    }
}

/*void Block::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton) {
        qDebug() << "Left";
        emit Chosen(this->p.x(), this->p.y());
    }
}*/

void Block::Highlight(int _x, int _y)
{
    if(p.x() == _x || p.y() == _y) {
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
    content.clear();
    blockNum->setText("");
    number = -1;
}

void Block::AddValue(int _num)
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
