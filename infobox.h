#ifndef INFOBOX_H
#define INFOBOX_H

#include <QDialog>

namespace Ui {
class InfoBox;
}

class InfoBox : public QDialog
{
    Q_OBJECT

public:
    explicit InfoBox(QWidget *parent = 0);
    ~InfoBox();
    void SetText(QString qstr);

private slots:
    void on_pushButton_clicked();

private:
    Ui::InfoBox *ui;
};

#endif // INFOBOX_H
