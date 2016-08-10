#include "tx_rx_protocol.h"
#include "def_commands.h"
#include "afmdevice.h"
#include <QtCore/QtGlobal>
#include <QObject>
#include <QtSerialPort/QtSerialPort>

int afmdevice::connect(QString portName, int baudRate)
{
    serialPort->setPortName(portName);
    serialPort->setBaudRate(baudRate);

    serialPort->setDataBits(afmdevice::dataBits);
    serialPort->setParity(afmdevice::parity);
    serialPort->setStopBits(afmdevice::stopBits);
    serialPort->setFlowControl(afmdevice::flowControl);
    if (serialPort->open(QIODevice::ReadWrite)) {
        serialPortInfo = &QSerialPortInfo(*serialPort);
        return 0;
    }
    return 1;
}

int afmdevice::disconnect()
{
    serialPort->close();
    return 0;
}
