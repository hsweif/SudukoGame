#include "block.h"

Block::Block(QWidget *parent)
    :QWidget(parent)
{
    //nubEidt=new QLineEdit(this);
    nubEidt=new QTextBrowser(this);
    nubEidt->setAlignment(Qt::AlignCenter);
    QFont font;
    font.setPixelSize(30);
    nubEidt->setFont(font);
    nubEidt->setFixedSize(50,50);
    /*
    //限制住格子内只能输入1-9
    QRegExp regExp("[1-9]{1}");
    nubEidt->setValidator(new QRegExpValidator(regExp,this));
    */
    connect(nubEidt,SIGNAL(textChanged(QString)),this,SLOT(dataChange(QString)));
    clearBlock();
}

int Block::data()
{
    return da;
}

void Block::clearBlock()
{
    nubEidt->setText("");
    da=-1;
}

void Block::setValue(int a)
{
    if(a<1)
    {
        nubEidt->setText("");
        da=-1;
    }
    else
    {
        nubEidt->setText(QString::number(a));
        da=a;
    }

}

void Block::setEna(bool ok)
{
    nubEidt->setEnabled(ok);
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
    pale.setColor(QPalette::Text,color);
    nubEidt->setPalette(pale);
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
