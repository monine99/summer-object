#include "mainwindow.h"

#include <QApplication>
#include "logindialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LoginDialog login;
    if (login.exec() != QDialog::Accepted || !login.isAuthenticated()) {
        return 0; // 登录失败或取消，直接退出
    }

    MainWindow w;
    w.show();
    return a.exec();
}
