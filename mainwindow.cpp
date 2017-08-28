#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    frame = new QFrame(this);
}

void MainWindow::SetupBlocks()
{

}

MainWindow::~MainWindow()
{
    delete ui;
}
