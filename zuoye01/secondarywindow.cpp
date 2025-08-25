#include "secondarywindow.h"
#include "ui_secondarywindow.h"

SecondaryWindow::SecondaryWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SecondaryWindow)
{
    ui->setupUi(this);
}

SecondaryWindow::~SecondaryWindow()
{
    delete ui;
}


