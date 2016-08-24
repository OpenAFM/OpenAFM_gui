#include "mainwindow.h"
#include "surfacegraph.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::addLibraryPath("@executable_path/../");
    QApplication a(argc, argv);
    MainWindow w;

    a.setApplicationName("OpenAFM 0.7");
    a.setApplicationDisplayName("OpenAFM 0.7");
    a.setApplicationVersion("0.7");
    w.show();
    return a.exec();
}
