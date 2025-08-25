/********************************************************************************
** Form generated from reading UI file 'logindialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINDIALOG_H
#define UI_LOGINDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *labelUser;
    QLineEdit *editUsername;
    QLabel *labelPass;
    QWidget *passwordComposite;
    QHBoxLayout *horizontalLayoutPass;
    QLineEdit *editPassword;
    QToolButton *btnTogglePassword;
    QHBoxLayout *buttonLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnLogin;
    QPushButton *btnCancel;
    QHBoxLayout *bottomActions;
    QSpacerItem *horizontalSpacer2;
    QPushButton *btnRegister;
    QPushButton *btnForgot;

    void setupUi(QDialog *LoginDialog)
    {
        if (LoginDialog->objectName().isEmpty())
            LoginDialog->setObjectName("LoginDialog");
        LoginDialog->resize(360, 200);
        verticalLayout = new QVBoxLayout(LoginDialog);
        verticalLayout->setObjectName("verticalLayout");
        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        labelUser = new QLabel(LoginDialog);
        labelUser->setObjectName("labelUser");

        formLayout->setWidget(0, QFormLayout::LabelRole, labelUser);

        editUsername = new QLineEdit(LoginDialog);
        editUsername->setObjectName("editUsername");

        formLayout->setWidget(0, QFormLayout::FieldRole, editUsername);

        labelPass = new QLabel(LoginDialog);
        labelPass->setObjectName("labelPass");

        formLayout->setWidget(1, QFormLayout::LabelRole, labelPass);

        passwordComposite = new QWidget(LoginDialog);
        passwordComposite->setObjectName("passwordComposite");
        horizontalLayoutPass = new QHBoxLayout(passwordComposite);
        horizontalLayoutPass->setObjectName("horizontalLayoutPass");
        editPassword = new QLineEdit(passwordComposite);
        editPassword->setObjectName("editPassword");
        editPassword->setEchoMode(QLineEdit::Password);

        horizontalLayoutPass->addWidget(editPassword);

        btnTogglePassword = new QToolButton(passwordComposite);
        btnTogglePassword->setObjectName("btnTogglePassword");

        horizontalLayoutPass->addWidget(btnTogglePassword);


        formLayout->setWidget(1, QFormLayout::FieldRole, passwordComposite);


        verticalLayout->addLayout(formLayout);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName("buttonLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonLayout->addItem(horizontalSpacer);

        btnLogin = new QPushButton(LoginDialog);
        btnLogin->setObjectName("btnLogin");

        buttonLayout->addWidget(btnLogin);

        btnCancel = new QPushButton(LoginDialog);
        btnCancel->setObjectName("btnCancel");

        buttonLayout->addWidget(btnCancel);


        verticalLayout->addLayout(buttonLayout);

        bottomActions = new QHBoxLayout();
        bottomActions->setObjectName("bottomActions");
        horizontalSpacer2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        bottomActions->addItem(horizontalSpacer2);

        btnRegister = new QPushButton(LoginDialog);
        btnRegister->setObjectName("btnRegister");

        bottomActions->addWidget(btnRegister);

        btnForgot = new QPushButton(LoginDialog);
        btnForgot->setObjectName("btnForgot");

        bottomActions->addWidget(btnForgot);


        verticalLayout->addLayout(bottomActions);


        retranslateUi(LoginDialog);

        QMetaObject::connectSlotsByName(LoginDialog);
    } // setupUi

    void retranslateUi(QDialog *LoginDialog)
    {
        LoginDialog->setWindowTitle(QCoreApplication::translate("LoginDialog", "\347\231\273\345\275\225", nullptr));
        labelUser->setText(QCoreApplication::translate("LoginDialog", "\347\224\250\346\210\267\345\220\215\357\274\232", nullptr));
        editUsername->setPlaceholderText(QCoreApplication::translate("LoginDialog", "\350\257\267\350\276\223\345\205\245\347\224\250\346\210\267\345\220\215", nullptr));
        labelPass->setText(QCoreApplication::translate("LoginDialog", "\345\257\206\347\240\201\357\274\232", nullptr));
        editPassword->setPlaceholderText(QCoreApplication::translate("LoginDialog", "\350\257\267\350\276\223\345\205\245\345\257\206\347\240\201", nullptr));
        btnTogglePassword->setText(QCoreApplication::translate("LoginDialog", "\346\230\276\347\244\272", nullptr));
        btnLogin->setText(QCoreApplication::translate("LoginDialog", "\347\231\273\345\275\225", nullptr));
        btnCancel->setText(QCoreApplication::translate("LoginDialog", "\345\217\226\346\266\210", nullptr));
        btnRegister->setText(QCoreApplication::translate("LoginDialog", "\346\263\250\345\206\214", nullptr));
        btnForgot->setText(QCoreApplication::translate("LoginDialog", "\346\211\276\345\233\236\345\257\206\347\240\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginDialog: public Ui_LoginDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINDIALOG_H
