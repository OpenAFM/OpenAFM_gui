#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QtGlobal>
#include <QMainWindow>
#include <QObject>
#include <QtSerialPort/QtSerialPort>
#include "qcustomplot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QByteArray bytesReceived;



public slots:

    void putChar(QByteArray data);
    void putChar(char data);
    void phone_CommandRouter(QByteArray buffer, quint16 bytes_received);
    void setStatusbarText(const QString& text);
    void sendEcho();
    void toggleLED();
    void putData_debug(const QByteArray &data);


signals:
    void sendDataFrame(QByteArray buffer, quint16 size);
    void dataReceived(QByteArray data);
    void plotDataReceived(QByteArray data);

private:
    QTimer dataTimer;
    int previouskey=0;

    Ui::MainWindow *ui;
    void fillPortsInfo();
    QSerialPort *serial;
    QSerialPortInfo *info;
    void getChipSignature();
    void command_default(QByteArray buffer, quint16 bytes_received);
    void response_echo_data(QByteArray buffer, quint16 bytes_received);
    void command_error();
    void setupStreaming(QCustomPlot *customPlot);

private slots:
    void sendData();
    void openSerialPort();
    void closeSerialPort();
    void writeData(const QByteArray &data);

    void displayIncoming(QByteArray data,quint16 no_of_bytes);

    void readData();
    void sendData(QByteArray data);
    void handleError(QSerialPort::SerialPortError error);
    void print_hex_value(QByteArray data);
    void on_pushButton_clicked();
    void on_pushButton_Send_clicked();
    void on_scan_setup1_textChanged();
    void on_pushButton_2_pressed();
    void on_lineEdit_Data_returnPressed();
    void realtimeDataSlot(QByteArray);

    void on_calibration_PB_toggled(bool checked);
};

#endif // MAINWINDOW_H