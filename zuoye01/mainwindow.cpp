#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "secondarywindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->btnOpenSecond, &QPushButton::clicked, this, [this]{
        SecondaryWindow dlg(this);
        dlg.exec();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
