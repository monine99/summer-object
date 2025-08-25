/********************************************************************************
** Form generated from reading UI file 'secondarywindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SECONDARYWINDOW_H
#define UI_SECONDARYWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SecondaryWindow
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;

    void setupUi(QDialog *SecondaryWindow)
    {
        if (SecondaryWindow->objectName().isEmpty())
            SecondaryWindow->setObjectName("SecondaryWindow");
        SecondaryWindow->resize(400, 300);
        verticalLayout = new QVBoxLayout(SecondaryWindow);
        verticalLayout->setObjectName("verticalLayout");
        label = new QLabel(SecondaryWindow);
        label->setObjectName("label");
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);


        retranslateUi(SecondaryWindow);

        QMetaObject::connectSlotsByName(SecondaryWindow);
    } // setupUi

    void retranslateUi(QDialog *SecondaryWindow)
    {
        SecondaryWindow->setWindowTitle(QCoreApplication::translate("SecondaryWindow", "\346\254\241\347\252\227\345\217\243", nullptr));
        label->setText(QCoreApplication::translate("SecondaryWindow", "\350\277\231\346\230\257\344\270\200\344\270\252\346\254\241\347\272\247\347\252\227\345\217\243", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SecondaryWindow: public Ui_SecondaryWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SECONDARYWINDOW_H
