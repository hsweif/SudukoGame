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
    blockNum->setStyleSheet("QTextBrowser{"
                            "font: 24pt American Typewriter;"
                            "text-align: center;"
                            "background-color: rgb(240, 248, 255);}");

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
    this->SetFontType(FontPolicy());
    /*if(content.size() > 2) {
        SetFontType(1);
    }
    else {
        SetFontType(2);
    }*/
    //blockNum->setFont(font);
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
        this->changeColor("mark");
    }
    else if((_type == 'r' || _type == 'b')
          && (p.x() == _x || p.y() == _y))
    {
        this->changeColor("highlight");
    }
    else if((_type == 'b' || _type == 'n')
            && number == _num && number != -1)
    {
        this->changeColor("highlight");
    }
    else if(p.x() == _x && p.y() == _y)
    {
        this->changeColor("highlight");
    }
    else if(!enaFlag)
    {
        this->changeColor("map");
    }
    else{
        this->changeColor("background");
    }
    this->SetFontType(FontPolicy());
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
        /*if(content.size() > 2) {
            //font.setPixelSize(18);
            qDebug() << "SetFontType2";
            this->SetFontType(1);
        }
        else {
            //font.setPixelSize(30);
            qDebug() << "SetFontType1";
            this->SetFontType(2);
        }*/
        this->SetFontType(FontPolicy());
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

void Block::SetFontType(int sz)
{
     if(sz == 0)
     {
        qDebug() << "hi";
        if(curColor == "background")
        {
            blockNum->setStyleSheet("QTextBrowser{"
                                "font: 12pt American Typewriter;"
                                "background-color: rgb(240, 248, 255);}");

        }
        else if(curColor == "mark")
        {
            blockNum->setStyleSheet("QTextBrowser{"
                                "font: 12pt American Typewriter;"
                                "background-color: rgb(135, 206, 235);}");

        }
        else if(curColor == "highlight")
        {
            blockNum->setStyleSheet("QTextBrowser{"
                                "font: 12pt American Typewriter;"
                                "background-color: rgb(151, 255, 255);}");

        }
        else if(curColor == "map")
        {
            blockNum->setStyleSheet("QTextBrowser{"
                                "font: 12pt American Typewriter;"
                                "background-color: rgb(211, 211, 211);}");

        }
    }
    else if(sz == 1)
    {
        qDebug() << "hi";
        if(curColor == "background")
        {
            blockNum->setStyleSheet("QTextBrowser{"
                                "font: 18pt American Typewriter;"
                                "background-color: rgb(240, 248, 255);}");

        }
        else if(curColor == "mark")
        {
            blockNum->setStyleSheet("QTextBrowser{"
                                "font: 18pt American Typewriter;"
                                "background-color: rgb(135, 206, 235);}");

        }
        else if(curColor == "highlight")
        {
            blockNum->setStyleSheet("QTextBrowser{"
                                "font: 18pt American Typewriter;"
                                "background-color: rgb(151, 255, 255);}");

        }
        else if(curColor == "map")
        {
            blockNum->setStyleSheet("QTextBrowser{"
                                "font: 18pt American Typewriter;"
                                "background-color: rgb(211, 211, 211);}");

        }
    }
    else if(sz == 2)
    {
        if(curColor == "background")
        {
            blockNum->setStyleSheet("QTextBrowser{"
                                "font: 30pt American Typewriter;"
                                "background-color: rgb(240, 248, 255);}");

        }
        else if(curColor == "mark")
        {
            blockNum->setStyleSheet("QTextBrowser{"
                                "font: 30pt American Typewriter;"
                                "background-color: rgb(135, 206, 235);}");

        }
        else if(curColor == "highlight")
        {
            blockNum->setStyleSheet("QTextBrowser{"
                                "font: 30pt American Typewriter;"
                                "background-color: rgb(151, 255, 255);}");

        }
        else if(curColor == "map")
        {
            blockNum->setStyleSheet("QTextBrowser{"
                                "font: 30pt American Typewriter;"
                                "background-color: rgb(211, 211, 211);}");

        }
    }

}

int Block::FontPolicy()
{
    if(content.size() > 6)
        return 0;
    else if(content.size() > 2)
        return 1;
    else
        return 2;
}

void Block::dataChange(const QString &data)
{
    if(data.isEmpty())
        number=-1;
    else
        number=data.toInt();
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

void Block::changeColor(QString qstr)
{
    curColor = qstr;
    /*if(qstr == "background")
    {
        blockNum->setStyleSheet("QTextBrowser{"
                            "font: 24pt American Typewriter;"
                            "background-color: rgb(240, 248, 255);}");
    }
    else if(qstr == "mark")
    {
        blockNum->setStyleSheet("QTextBrowser{"
                            "font: 24pt American Typewriter;"
                            "background-color: rgb(135, 206, 235);}");
    }
    else if(qstr == "highlight")
    {
        blockNum->setStyleSheet("QTextBrowser{"
                            "font: 24pt American Typewriter;"
                            "background-color: rgb(151, 255, 255);}");
    }
    else if(qstr == "map")
    {
        blockNum->setStyleSheet("QTextBrowser{"
                            "font: 24pt American Typewriter;"
                            "background-color: rgb(211, 211, 211);}");
    }*/
}
