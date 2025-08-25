#include "logindialog.h"
#include "ui_logindialog.h"

#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , m_authenticated(false)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    // 占位提示
    ui->editUsername->setPlaceholderText(tr("请输入用户名"));
    ui->editPassword->setPlaceholderText(tr("请输入密码"));

    // 信号绑定
    connect(ui->btnLogin, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
    connect(ui->btnCancel, &QPushButton::clicked, this, &LoginDialog::onCancelClicked);
    connect(ui->btnTogglePassword, &QToolButton::clicked, this, &LoginDialog::onTogglePasswordClicked);
    connect(ui->btnRegister, &QPushButton::clicked, this, &LoginDialog::onRegisterClicked);
    connect(ui->btnForgot, &QPushButton::clicked, this, &LoginDialog::onForgotClicked);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

bool LoginDialog::isAuthenticated() const
{
    return m_authenticated;
}

void LoginDialog::onLoginClicked()
{
    const QString username = ui->editUsername->text().trimmed();
    const QString password = ui->editPassword->text();

    // 简单校验：固定账号密码，可改为查数据库/接口
    const QString validUser = QStringLiteral("admin");
    const QString validPass = QStringLiteral("123456");

    if (username == validUser && password == validPass) {
        m_authenticated = true;
        accept();
    } else {
        m_authenticated = false;
        QMessageBox::warning(this, tr("登录失败"), tr("用户名或密码错误"));
    }
}

void LoginDialog::onCancelClicked()
{
    m_authenticated = false;
    reject();
}

void LoginDialog::onTogglePasswordClicked()
{
    if (ui->editPassword->echoMode() == QLineEdit::Password) {
        ui->editPassword->setEchoMode(QLineEdit::Normal);
        ui->btnTogglePassword->setText(tr("隐藏"));
    } else {
        ui->editPassword->setEchoMode(QLineEdit::Password);
        ui->btnTogglePassword->setText(tr("显示"));
    }
}

void LoginDialog::onRegisterClicked()
{
    QMessageBox::information(this, tr("注册"), tr("这里实现注册流程"));
}

void LoginDialog::onForgotClicked()
{
    QMessageBox::information(this, tr("找回密码"), tr("这里实现找回密码流程"));
}


