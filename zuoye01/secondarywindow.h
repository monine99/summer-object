#ifndef SECONDARYWINDOW_H
#define SECONDARYWINDOW_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class SecondaryWindow; }
QT_END_NAMESPACE

class SecondaryWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SecondaryWindow(QWidget *parent = nullptr);
    ~SecondaryWindow();

private:
    Ui::SecondaryWindow *ui;
};

#endif // SECONDARYWINDOW_H


