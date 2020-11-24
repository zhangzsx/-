#include "widget.h"
#include "mainform.h"
#include <QApplication>
//#include <QTextCodec>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
    MainForm Main;
//    Main.resize(250,700);
    Main.show();
    a.connect(&a,SIGNAL(lastWindowClosed()),&a,SLOT(quit()));
    return a.exec();
}
