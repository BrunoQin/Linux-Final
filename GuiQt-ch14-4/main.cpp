#include <QtWidgets/QApplication>
#include <QTextCodec>
#include "imagewindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName ("System");
    //QTextCodec::setCodecForCStrings (codec);
    QTextCodec::setCodecForLocale (codec);
    //QTextCodec::setCodecForTr (codec);

    ImageWindow w;
    w.show();

    return a.exec();
}
