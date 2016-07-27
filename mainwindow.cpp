#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tx_rx_protocol.h"
#include "def_commands.h"
#include "scanwindow.h"
#include "scannerwindow.h"
#include "scan_setup.h"



#include <QMessageBox>
#include <QDebug>

#include <QtSerialPort/QSerialPort>
#include <QSerialPortInfo>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusBar->showMessage("Qt to Arduino phone  example", 3000);
    serial = new QSerialPort(this);
    this->fillPortsInfo();
    TX_RX_qt* phone = TX_RX_qt::instance();
    phone->receive_frame_buffer.reserve(10000);

    QObject::connect(phone, SIGNAL(TX_TransmitByte(QByteArray)),
                     this, SLOT(putChar(QByteArray)));

    QObject::connect(phone, SIGNAL(TX_TransmitByte(char)),
                     this, SLOT(putChar(char)));

    QObject::connect(phone, SIGNAL(ValidFrameReceived(QByteArray, quint16)),
                     this, SLOT(phone_CommandRouter(QByteArray, quint16)));

    QObject::connect(phone, SIGNAL(ValidFrameReceived(QByteArray, quint16)),
                     this, SLOT(displayIncoming(QByteArray, quint16)));


    QObject::connect(this, SIGNAL(dataReceived(QByteArray)),
                     phone, SLOT(charReceiver(QByteArray)));

    QObject::connect(this, SIGNAL(dataReceived(QByteArray)),
                     this, SLOT(print_hex_value(QByteArray)));

    QObject::connect(this, SIGNAL(sendDataFrame(QByteArray, quint16)),
                     phone, SLOT(frameDecode(QByteArray, quint16)));

    QObject::connect(ui->pushButton_connect, SIGNAL(clicked()),
                     this, SLOT(openSerialPort()));

    QObject::connect(serial, SIGNAL(readyRead()),
                     this, SLOT(readData()));



    QObject::connect(ui->pushButton_echo, SIGNAL(clicked()),
                     this, SLOT(sendEcho()) );

    QObject::connect(ui->pushButton_LED, SIGNAL(clicked()),
                     this, SLOT(toggleLED()) );

    QObject::connect(this, SIGNAL(plotDataReceived(QByteArray)), this, SLOT(realtimeDataSlot(QByteArray)));


    setupStreaming(ui->customPlot);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::putChar(QByteArray data)
{
    ui->plainTextEdit_debug->appendPlainText(data.data());
    this->writeData(data);
}

void MainWindow::putChar(char data)
{
    QByteArray qdata(1, data);
    ui->plainTextEdit_debug->appendPlainText(qdata.data());
    this->writeData(qdata);
}

void MainWindow::phone_CommandRouter(QByteArray buffer, quint16 bytes_received)
{
    if(previousResponse==RESPONSE_READY && buffer!=READY){
        emit plotDataReceived(buffer);
    }
    if(buffer==READY){
        qDebug()<<"RDY";
       previousResponse=RESPONSE_READY;
    }
    if(buffer==GO){
        qDebug()<<"GO";
       previousResponse=RESPONSE_GO;
    }
    if(buffer==DONE){
        qDebug()<<"DONE";
       previousResponse=RESPONSE_DONE;
    }
}

void MainWindow::command_default(QByteArray buffer, quint16 bytes_received) {
    ui->statusBar->showMessage(tr("Received a valid packet/command"), 2000);
}

void MainWindow::command_error() {
    ui->statusBar->showMessage(tr("Error in  packet/command"), 2000);
}



void MainWindow::response_echo_data(QByteArray buffer, quint16 bytes_received) {
    QString bufferText(buffer);
    ui->statusBar->showMessage(bufferText, 2000);
}


void MainWindow::sendData() {
    QByteArray data;
    data.append(ui->lineEdit_Data->text());;
    ui->lineEdit_Data->clear();
    emit sendDataFrame(data, (quint16)data.length());
}

void MainWindow::fillPortsInfo()
{
    ui->serialPortDropdown->clear();
    static const QString blankString = QObject::tr("N/A");
    QString description;
    QString manufacturer;
    QString serialNumber;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        serialNumber = info.serialNumber();
        list << info.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << (!serialNumber.isEmpty() ? serialNumber : blankString)
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

        ui->serialPortDropdown->addItem(list.first(), list);
    }
}

void MainWindow::openSerialPort()
{
    serial->setPortName(ui->serialPortDropdown->currentText());
    serial->setBaudRate(ui->baudDropDown->currentText().toInt());

    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    if (serial->open(QIODevice::ReadWrite)) {
            ui->statusBar->showMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                                       .arg(ui->serialPortDropdown->currentText())
                                       .arg(ui->baudDropDown->currentText().toInt())
                                       .arg(QSerialPort::Data8)
                                       .arg(QSerialPort::NoParity)
                                       .arg(QSerialPort::OneStop)
                                       .arg(QSerialPort::NoFlowControl));
            ui->pushButton_connect->setText("Disconnect");
            const QSerialPortInfo info= QSerialPortInfo(*serial);
            ui->label_deviceSignature->setText(info.description());
            disconnect( ui->pushButton_connect, SIGNAL(clicked()),0, 0);
            QObject::connect(ui->pushButton_connect, SIGNAL(clicked()),
                             this, SLOT(closeSerialPort()));

    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());
        ui->statusBar->showMessage(tr("Open error"));
    }
}

void MainWindow::closeSerialPort()
{
    serial->close();
    ui->statusBar->showMessage(tr("Disconnected"));
    disconnect( ui->pushButton_connect, SIGNAL(clicked()),0, 0);
    QObject::connect(ui->pushButton_connect, SIGNAL(clicked()),
                     this, SLOT(openSerialPort()));
    ui->pushButton_connect->setText("Connect");
    ui->label_deviceSignature->setText(" ");
}

void MainWindow::writeData(const QByteArray &data)
{
    serial->write(data);
}

void MainWindow::putData_debug(const QByteArray &data)
{
    ui->plainTextEdit_debug->appendPlainText(data.data());

}

void MainWindow::displayIncoming(QByteArray data,quint16 no_of_bytes){

    ui->plainTextEdit_input->appendPlainText(data);

}

void MainWindow::readData()
{

    QByteArray data = serial->readAll();
    data.replace('', "");
    data.replace('\n', "");
    data.replace('\r', "");
    data.simplified();
   // ui->plainTextEdit_input->appendPlainText(data.data());

    int dSize = data.size();
    if (dSize > 0) {

        emit dataReceived(data);}
}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}

void MainWindow::sendData(QByteArray data) {
    quint16 length = data.length();
    emit sendDataFrame(data, length);
}

void MainWindow::setStatusbarText(const QString& text) {
    ui->statusBar->showMessage(text);
}

void MainWindow::print_hex_value(QByteArray data) {
    QByteArray hex_console;
    QByteArray character;
    for (int i = 0; i < data.size(); ++i) {
        character.clear();
        character.append(data.at(i));
        //character = data.at(i);
        hex_console.append("0x");
        hex_console.append(character.toHex().toUpper());
        hex_console.append("\n");
    }
    ui->plainTextEdit_input_hex->appendPlainText(QString(hex_console));
}

/* Command sending functions */

void MainWindow::sendEcho() {
    QByteArray data;
    data.append((quint8)COMMAND_ECHO_DATA);
    data.append("ABCD");
    emit sendDataFrame(data, (quint16) data.length());
}

void MainWindow::toggleLED() {
    QByteArray data;
    data.append((quint8)COMMAND_TOGGLE_LED);
    emit sendDataFrame(data, 1);
}

void MainWindow::on_pushButton_clicked()
{
     scannerwindow* Scanner= new scannerwindow(this, serial);
     Scanner->setModal(true);
     Scanner->exec();


}

void MainWindow::on_pushButton_Send_clicked()
{
    sendData();
}

void MainWindow::on_pushButton_2_pressed()
{
    fillPortsInfo();
}

void MainWindow::on_lineEdit_Data_returnPressed()
{
    sendData();
}

void MainWindow::on_scan_setup1_textChanged()
{

}

void MainWindow::setupStreaming(QCustomPlot *customPlot)
{

  customPlot->addGraph(); // blue line

  customPlot->graph(0)->setPen(QPen(Qt::blue));
  customPlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
  customPlot->graph(0)->setAntialiasedFill(false);

  customPlot->addGraph(); // blue dot

  customPlot->graph(1)->setPen(QPen(Qt::blue));
  customPlot->graph(1)->setLineStyle(QCPGraph::lsNone);
  customPlot->graph(1)->setScatterStyle(QCPScatterStyle::ssDisc);


  customPlot->setInteractions(QCP::iRangeDrag);
  customPlot->setInteractions(QCP::iRangeZoom);


  customPlot->xAxis->setTickLabels(false);
  customPlot->yAxis->setTickLabels(false);


  customPlot->axisRect()->setRangeDrag(Qt::Vertical);
  customPlot->axisRect()->setRangeZoom(Qt::Vertical);

  customPlot->axisRect()->setupFullAxesBox(true);




}

void MainWindow::realtimeDataSlot(QByteArray data)
{

    if(ui->calibration_PB->isChecked()){
    int value0 = data[0];

    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

    // add data to lines:
    ui->customPlot->graph(0)->addData(key, value0);
    // set data of dots:
    ui->customPlot->graph(1)->clearData();
    ui->customPlot->graph(1)->addData(key, value0);
    // remove data of lines that's outside visible range:
    ui->customPlot->graph(0)->removeDataBefore(key-15);
    // rescale value (vertical) axis to fit the current data:


    ui->customPlot->xAxis->setRange(key+1, 16, Qt::AlignRight);
    ui->customPlot->graph(0)->rescaleValueAxis();
    ui->customPlot->replot();

    putChar("RDY;");

}

}

void MainWindow::on_calibration_PB_toggled(bool checked)
{
    if(checked){
    ui->calibration_PB->setText("Done");
    sendData("STREAM;");
    sendData("RDY;");
    }
    else{
     ui->calibration_PB->setText("Calibrate");
     sendData("DONE;");
    }
}