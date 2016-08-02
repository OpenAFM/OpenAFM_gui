#include "intensitymap.h"
#include "ui_intensitymap.h"
intensitymap::intensitymap(QSurfaceDataProxy* Series,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::intensitymap)
{
    ui->setupUi(this);
    dataSeries=Series;
    setupIntensityMap(ui->intensityMap);
    ui->textBrowser->append("####AFM SCAN###\n");
    ui->textBrowser->append("Time: ");

    ui->textBrowser->append(QDateTime::currentDateTime().toString());
    ui->textBrowser->append("\n");


}

intensitymap::~intensitymap()
{
    delete ui;
}

void intensitymap::setupIntensityMap(QCustomPlot *customPlot)
{

  customPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
  customPlot->axisRect()->setupFullAxesBox(true);
  customPlot->xAxis->setLabel("x");
  customPlot->yAxis->setLabel("y");


  colorMap = new QCPColorMap(customPlot->xAxis, customPlot->yAxis);
  customPlot->addPlottable(colorMap);

  int nx = dataSeries->rowCount();
  int ny = dataSeries->columnCount();
  colorMap->data()->setSize(nx, ny);
  colorMap->data()->setRange(QCPRange(0, dataSeries->rowCount()), QCPRange(0, dataSeries->columnCount())); // and span the coordinate range -4..4 in both key (x) and value (y) dimensions
  int max=0;
  double x, y, z;

  for (int xIndex=0; xIndex<nx; ++xIndex)
  {
    for (int yIndex=0; yIndex<ny; ++yIndex)
    {
      colorMap->data()->cellToCoord(xIndex, yIndex, &x, &y);

      z = dataSeries->itemAt(xIndex,yIndex)->y();
      if (z>max){
          max=z;
      }
      colorMap->data()->setCell(xIndex, yIndex, z);
    }
  }


  QCPColorScale *colorScale = new QCPColorScale(customPlot);
  customPlot->plotLayout()->addElement(0, 1, colorScale);
  colorScale->setType(QCPAxis::atRight);
  colorMap->setColorScale(colorScale);
  colorScale->axis()->setLabel("Deflection");

  colorMap->setGradient(QCPColorGradient::gpPolar);

  colorMap->rescaleDataRange();

  ui->rangeMaxSlider->setMaximum(max);
  ui->rangeMaxSlider->setValue(max);

  QCPMarginGroup *marginGroup = new QCPMarginGroup(customPlot);
  customPlot->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
  colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);

  customPlot->rescaleAxes();
}
void intensitymap::setCutOff(int max){
    QCPRange range(0,max);
    colorMap->setDataRange(range);
    ui->intensityMap->replot();

}


void intensitymap::on_rangeMaxSlider_valueChanged(int value)
{
    setCutOff(value);
}

void intensitymap::on_save_pb_clicked()
{
    ui->intensityMap->saveBmp("AFM_demo_scan");
}
