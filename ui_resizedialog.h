/********************************************************************************
** Form generated from reading UI file 'resizedialog.ui'
**
** Created: Mon Sep 26 14:16:39 2011
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RESIZEDIALOG_H
#define UI_RESIZEDIALOG_H

#include <QtCore/QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QDialog>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QSpacerItem>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_ResizeDialog
{
public:
    QVBoxLayout *vboxLayout;
    QLabel *mainLabel;
    QHBoxLayout *hboxLayout;
    QSpinBox *widthSpinBox;
    QLabel *xLabel;
    QSpinBox *heightSpinBox;
    QHBoxLayout *hboxLayout1;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *ResizeDialog)
    {
        if (ResizeDialog->objectName().isEmpty())
            ResizeDialog->setObjectName(QString::fromUtf8("ResizeDialog"));
        ResizeDialog->resize(190, 129);
        vboxLayout = new QVBoxLayout(ResizeDialog);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        mainLabel = new QLabel(ResizeDialog);
        mainLabel->setObjectName(QString::fromUtf8("mainLabel"));

        vboxLayout->addWidget(mainLabel);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        widthSpinBox = new QSpinBox(ResizeDialog);
        widthSpinBox->setObjectName(QString::fromUtf8("widthSpinBox"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widthSpinBox->sizePolicy().hasHeightForWidth());
        widthSpinBox->setSizePolicy(sizePolicy);
        widthSpinBox->setMaximum(10000);

        hboxLayout->addWidget(widthSpinBox);

        xLabel = new QLabel(ResizeDialog);
        xLabel->setObjectName(QString::fromUtf8("xLabel"));
        xLabel->setAlignment(Qt::AlignCenter);

        hboxLayout->addWidget(xLabel);

        heightSpinBox = new QSpinBox(ResizeDialog);
        heightSpinBox->setObjectName(QString::fromUtf8("heightSpinBox"));
        sizePolicy.setHeightForWidth(heightSpinBox->sizePolicy().hasHeightForWidth());
        heightSpinBox->setSizePolicy(sizePolicy);
        heightSpinBox->setMaximum(10000);

        hboxLayout->addWidget(heightSpinBox);


        vboxLayout->addLayout(hboxLayout);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        spacerItem = new QSpacerItem(0, 21, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(spacerItem);

        okButton = new QPushButton(ResizeDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setDefault(true);

        hboxLayout1->addWidget(okButton);

        cancelButton = new QPushButton(ResizeDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        hboxLayout1->addWidget(cancelButton);


        vboxLayout->addLayout(hboxLayout1);


        retranslateUi(ResizeDialog);
        QObject::connect(okButton, SIGNAL(clicked()), ResizeDialog, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), ResizeDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ResizeDialog);
    } // setupUi

    void retranslateUi(QDialog *ResizeDialog)
    {
        ResizeDialog->setWindowTitle(QApplication::translate("ResizeDialog", "Image Pro", 0));
        mainLabel->setText(QApplication::translate("ResizeDialog", "Enter new size:", 0));
        xLabel->setText(QApplication::translate("ResizeDialog", "x", 0));
        okButton->setText(QApplication::translate("ResizeDialog", "OK", 0));
        cancelButton->setText(QApplication::translate("ResizeDialog", "Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class ResizeDialog: public Ui_ResizeDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RESIZEDIALOG_H
