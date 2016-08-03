#ifndef INTENSITYMAP_H
#define INTENSITYMAP_H

#include <QDialog>
#include "qcustomplot.h"
#include "surfacegraph.h"


namespace Ui {
class intensitymap;
}

class intensitymap : public QDialog
{
    Q_OBJECT

public:
    explicit intensitymap(QSurfaceDataProxy* series, QList <int> ScanData, QWidget *parent = 0);
    ~intensitymap();
    void setupIntensityMap(QCustomPlot *customPlot );

public slots:
    void setCutOff(int);

private slots:

    void on_rangeMaxSlider_valueChanged(int value);

    void on_save_pb_clicked();

private:
    QGraphicsScene *scene;
    QPixmap PixMap;
    QSurfaceDataProxy* dataSeries;
    QCPColorMap *colorMap;
    Ui::intensitymap *ui;
};

#endif // INTENSITYMAP_H
