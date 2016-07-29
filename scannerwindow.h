#ifndef SCANNERWINDOW_H
#define SCANNERWINDOW_H

#include <QDialog>
#include <QtSerialPort/QSerialPort>
#include <QObject>
#include <QtCore/QtGlobal>
#include <QMainWindow>




class scannerwindow :public QObject
{
    Q_OBJECT

public:
    explicit scannerwindow(QMainWindow *parent = 0, QSerialPort *serial=0);
    ~scannerwindow();


public slots:
    void close();

};

#endif // SCANNERWINDOW_H
