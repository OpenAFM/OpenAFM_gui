#ifndef AFMDEVICE_H
#define AFMDEVICE_H

#include <QtCore/QtGlobal>
#include <QObject>
#include <QtSerialPort/QtSerialPort>

class afmdevice : public QObject
{
    Q_OBJECT

private:
    // serial port
    QSerialPort *serialPort;
    QSerialPortInfo *serialPortInfo;

    // COMs configuration
    QString portName = "";
    int baudRate = 0;
    QSerialPort::DataBits dataBits = QSerialPort::Data8;
    QSerialPort::Parity parity = QSerialPort::NoParity;
    QSerialPort::StopBits stopBits = QSerialPort::OneStop;
    QSerialPort::FlowControl flowControl = QSerialPort::NoFlowControl;

    int sendData(QByteArray string);

public:

    /*
     * Connect the serial port using the internal configuration.
     * portName - the name of the serial port (e.g. "COM1")
     * baudRate - the serial baud rate
     * Return 0 if successfull, 1 otherwise.
     * If successfull, serialPortInfo is populated.
     */
    int connect(QString portName, int baudRate);

    /*
     * Disconnect the serial port.
     * Return 0 if successfull.
     */
    int disconnect();
};

#endif // AFMDEVICE_H
