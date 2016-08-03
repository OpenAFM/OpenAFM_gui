#ifndef SCANNERWINDOW_H
#define SCANNERWINDOW_H

#include <QDialog>

#include <QtSerialPort/QSerialPort>
#include <QObject>
#include <QtCore/QtGlobal>
#include <QMainWindow>
#include "qcustomplot.h"
#include "intensitymap.h"
#include "surfacegraph.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPainter>
#include <QtGui/QScreen>
#include <QObject>
#include <QDebug>
#include <QImage>






class scannerwindow :public QObject
{
    Q_OBJECT

public:
    explicit scannerwindow(QList<int>, QMainWindow *parent = 0, QSerialPort *serial=0,bool load=0, QTextStream* stream=nullptr);
    ~scannerwindow();
    QWidget* widget;
    Q3DSurface *graph;
    SurfaceGraph *modifier;
    QList<QByteArray> mock;

public slots:
    void close();
    void getBitmapScreen();
    void AFMButtonHandler(bool);
    void endScan();
    void saveImage();


signals:
    void AFMStart();
    void AFMDone();
private:
    QWidget* container;
    QList<int> parameters;
    QPushButton *AFM_Scan_3D_RB;
    QPushButton *BitmapView;
    QPushButton *SaveSurface;



};

#endif // SCANNERWINDOW_H
