#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class LoginDialog; }
QT_END_NAMESPACE

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    bool isAuthenticated() const;

private slots:
    void onLoginClicked();
    void onCancelClicked();
    void onTogglePasswordClicked();
    void onRegisterClicked();
    void onForgotClicked();

private:
    bool m_authenticated;
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H


