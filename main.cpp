#include "mainwindow.h"
#include "surfacegraph.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::addLibraryPath("@executable_path/../");
    QApplication a(argc, argv);
    MainWindow w;

    QFile f(":qdarkstyle/style.qss");
    if (!f.exists())
    {
        printf("Unable to set stylesheet, file not found\n");
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }

    a.setApplicationName("OpenAFM 0.7");
    a.setApplicationDisplayName("OpenAFM 0.7");
    a.setApplicationVersion("0.7");
    w.show();
    return a.exec();
}
