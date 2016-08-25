#include "tx_rx_protocol.h"
#include "def_commands.h"
#include "afmdevice.h"
#include <QtCore/QtGlobal>
#include <QObject>
#include <QtSerialPort/QtSerialPort>
#include <QtCore/QStringBuilder>


afmdevice::afmdevice(QObject *parent = 0) : QObject(parent)
{
    serialPort = new QSerialPort(this);
    phone = TX_RX_qt::instance();
    phone->receive_frame_buffer.reserve(10000);

    // connect signals of private objects to public signals
    connect(serialPort, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(handleError(QSerialPort::SerialPortError)));
    connect(serialPort, SIGNAL(QSerialPort::readyRead), this, SLOT(readReady));
}


int afmdevice::connectPort(QString portName, int baudRate)
{
    if (m_isConnected) return 2;
    commsMutex.lock();
    serialPort->setPortName(portName);
    serialPort->setBaudRate(baudRate);
    serialPort->setDataBits(dataBits);
    serialPort->setParity(parity);
    serialPort->setStopBits(stopBits);
    serialPort->setFlowControl(flowControl);
    if (serialPort->open(QIODevice::ReadWrite)) {
        serialPortInfo = &QSerialPortInfo(*serialPort);
        m_isConnected = true;
    }
    commsMutex.unlock();
    return isConnected() ? 0 : 1;
}

int afmdevice::disconnectPort()
{
    commsMutex.lock();
    serialPort->close();
    m_isConnected = false;
    commsMutex.unlock();
    return 0;
}

qint64 afmdevice::write(const QByteArray &data)
{
    commsMutex.lock();
    qint64 numBytesWritten = serialPort->write(data);
    commsMutex.unlock();
    return numBytesWritten;
}

qint64 afmdevice::write(const QString &data)
{
    commsMutex.lock();
    const QByteArray &bData = data.toUtf8();
    qint64 numBytesWritten = serialPort->write(bData);
    commsMutex.unlock();
    return numBytesWritten;
}


void afmdevice::sendReady()
{
    commsMutex.lock();
    qDebug()<<"received sig";
    serialPort->write(response::READY);
    commsMutex.unlock();
}

void afmdevice::sendGo()
{
    commsMutex.lock();
    qDebug()<<"received sig";
    serialPort->write(response::GO);
    commsMutex.unlock();
}

void afmdevice::sendDone(){
    commsMutex.lock();
    qDebug()<<"received sig";
    serialPort->write(response::DONE);
    commsMutex.unlock();
}


void afmdevice::setup(int stepSize, int lineLength, int sampleSize)
{
    // make send array
    QString toSend =  response::sSETUP % QString::number(stepSize) %
                      response::sF_BOUNDARY % QString::number(lineLength) %
                      response::sF_BOUNDARY % QString::number(sampleSize) %
                      response::sF_BOUNDARY;
    write(toSend);
}


QByteArray afmdevice::readAllData()
{
    commsMutex.lock();
    QByteArray data = serialPort->readAll();
    commsMutex.unlock();
    //data.replace('', "");
    //data.replace('\n', "");
    //data.replace('\r', "");
    //data.simplified();
    int dSize = data.size();
    if (dSize > 0) {
        emit dataReceived(data);
    }
    return data;
}

QByteArray afmdevice::readLine(int timeOut)
{
	QByteArray response;

	// just wait until the phone emits frame received
	QEventLoop ev;
	QTimer::singleShot(timeOut, ev.wakeUp);
	connect(phone, SIGNAL(ValidFrameReceived), ev.wakeUp);

	// run Qt event loop until either the timer elapses, or a valid frame is received
	ev.exec();
}

QList<QSerialPortInfo> afmdevice::getSerialPorts()
{
    return QSerialPortInfo::availablePorts();
}
