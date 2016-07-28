#ifndef SCANNERWINDOW_H
#define SCANNERWINDOW_H

#include <QDialog>
#include <QtSerialPort/QSerialPort>
#include <QObject>
#include <QtCore/QtGlobal>
#include <QMainWindow>



namespace Ui {
class scannerwindow;
}

class scannerwindow : public QDialog
{
    Q_OBJECT

public:
    explicit scannerwindow(QMainWindow *parent = 0, QSerialPort *serial=0);
    ~scannerwindow();

public slots:
    void dataHandler(QByteArray);


signals:
    void dataSent(QByteArray);
    void plotData(QList<QByteArray>);
    void scan_starting();


private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::scannerwindow *ui;
};

#endif // SCANNERWINDOW_H
