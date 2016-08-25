#ifndef AFMDEVICE_H
#define AFMDEVICE_H

#include <QtCore/QtGlobal>
#include <QtCore/QObject>
#include <QtSerialPort/QtSerialPort>
#include "tx_rx_protocol.h"

class afmdevice : public QObject
{
    Q_OBJECT

private:
    // serial port
    QSerialPort *serialPort;
    QSerialPortInfo *serialPortInfo;
    TX_RX_qt* phone;
    QMutex commsMutex;
    bool m_isConnected = false;

    // COMs configuration
    QString portName = "";
    int baudRate = 0;
    QSerialPort::DataBits dataBits = QSerialPort::Data8;
    QSerialPort::Parity parity = QSerialPort::NoParity;
    QSerialPort::StopBits stopBits = QSerialPort::OneStop;
    QSerialPort::FlowControl flowControl = QSerialPort::NoFlowControl;

    // handlers
    void handleError(QSerialPort::SerialPortError error);

public:

    explicit afmdevice(QObject *parent = 0);

    /*
     * Connect the serial port using the internal configuration.
     * portName - the name of the serial port (e.g. "COM1")
     * baudRate - the serial baud rate
     * Return 0 if successfull, 1 if failed, 2 if already connected.
     * If successfull, serialPortInfo is populated.
     */
    int connectPort(QString portName, int baudRate);

    /*
     * Disconnect the serial port.
     * Return 0 if successfull.
     */
    int disconnectPort();

    /*
     * Write a byte array to the device.
     * Returns the number of bytes written.
     * Locks commsMutex.
     */
    qint64 write(const QByteArray &data);

    /*
     * Write a string to the device.
     * Returns the number of bytes written.
     * Locks commsMutex.
     */
    qint64 write(const QString &data);

    /*
     * Reads all data from the serial port.
     * Returns an array of Bytes read from the port.
     * Locks commsMutex.
     */
    QByteArray readAllData();

    /*!
     * Read a line from the serial port (read everything up until the new line separator).
     * Blocks until a line is read, or the timeout period of \a timeOut milliseconds elapses.
     * \returns the line read.
     * Locks commsMutex;
     */
    QByteArray readLine(int timeOut);

    /*
     * Get a list of ports on the machine.
     * Returns the list.
     */
    QList<QSerialPortInfo> getSerialPorts();

    /*
     * Is the serial port connected?
     */
    bool isConnected() { return m_isConnected; }

    /*
     * Send the Ready command to the device.
     */
    void sendReady();

    /*
     * Send the Go signal to the device
     */
    void sendGo();
    void sendDone();

    /*
     * Sets up a scan.
     * Send the start signal followed by the stepSize, lineLength and sampleSize.
     */
    void setup(int stepSize, int lineLength, int sampleSize);


signals:
    void sendDataFrame(QByteArray buffer, quint16 size);
    void dataReceived(QByteArray data);
    void plotDataReceived(QByteArray data);
    void dataSent(QByteArray);
    void readReady();
    void plotData(QList<QByteArray>);
    void scan_starting();
};

#endif // AFMDEVICE_H
