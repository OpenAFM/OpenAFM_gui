#ifndef SCAN_SETUP_H
#define SCAN_SETUP_H

#include <QElapsedTimer>
#include <QObject>



class Scan_Setup : public QObject
{
    Q_OBJECT

public:
    explicit Scan_Setup(QWidget *parent = 0);
    ~Scan_Setup();

public slots:
    void setScanSize(int);
private:
    int scanlength;
    unsigned long elapsed;
    QElapsedTimer timer;
};

#endif // SCAN_SETUP_H
