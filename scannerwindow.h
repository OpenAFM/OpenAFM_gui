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






class scannerwindow :public QObject
{
    Q_OBJECT

public:
    explicit scannerwindow(QMainWindow *parent = 0, QSerialPort *serial=0);
    ~scannerwindow();
    QWidget* widget;
    Q3DSurface *graph;
    SurfaceGraph *modifier;
public slots:
    void close();
    void getBitmapScreen();
    void AFMButtonHandler(bool);
    void endScan();

signals:
    void AFMStart();
    void AFMDone();
private:
    QPushButton *AFM_Scan_3D_RB;
    QPushButton *BitmapView;


};

#endif // SCANNERWINDOW_H
