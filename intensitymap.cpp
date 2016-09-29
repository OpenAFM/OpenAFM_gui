#include "intensitymap.h"
#include "ui_intensitymap.h"

intensitymap::intensitymap(QSurfaceDataProxy* Series, QList <int> parameters,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::intensitymap)
{
    ui->setupUi(this);
    dataSeries=Series;
    setupIntensityMap(ui->intensityMap);

    //parameters={lineLength, stepSize, sampleSize};

    ui->textBrowser->append("####AFM SCAN SETTINGS###\n");

    ui->textBrowser->append("Time: ");
    ui->textBrowser->append(QDateTime::currentDateTime().toString());
    ui->textBrowser->append("\n");

    ui->textBrowser->append("X Length");
    ui->textBrowser->append(QString::number(parameters[0]));


    ui->textBrowser->append("Y Length");
    ui->textBrowser->append(QString::number(parameters[1]));

    ui->textBrowser->append("Step Size");
    ui->textBrowser->append(QString::number(parameters[2]));

    ui->textBrowser->append("Sample Size");
    ui->textBrowser->append(QString::number(parameters[3]));


    ui->textBrowser->append("X Offset");
    ui->textBrowser->append(QString::number(parameters[4]));


    ui->textBrowser->append("Y Offset");
    ui->textBrowser->append(QString::number(parameters[5]));


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
    colorMap->data()->setRange(QCPRange(0, dataSeries->rowCount()), QCPRange(0, dataSeries->columnCount()));
    int max=0;
    double x, y, z;

    for (int xIndex=0; xIndex<nx; ++xIndex)
    {
        for (int yIndex=0; yIndex<ny; ++yIndex)
        {
            //colorMap->data()->cellToCoord(xIndex, yIndex, &x, &y);

            z = dataSeries->itemAt(yIndex,xIndex)->y();
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
    customPlot->xAxis->setScaleRatio(customPlot->yAxis,1);
}

void intensitymap::on_save_pb_clicked()
{

    QString date=QDate::currentDate().toString();
    QString time=QTime::currentTime().toString();

    QString filepath = QFileDialog::getExistingDirectory(this, "Choose save path", "../../",QFileDialog::ShowDirsOnly);
    QFile file(filepath+"/AFM_Scan_config_"+QDate::currentDate().toString()+"_"+QTime::currentTime().toString());
    if(!filepath.isNull()){
        QDir dir(filepath+"/"+date+"AFM Scan");
           if (!dir.exists()) {
               dir.mkpath(".");
           }
     QFile file(dir.path()+"/AFM_Scan_config_"+time);

        if ( file.open(QIODevice::ReadWrite) )
        {
            QTextStream stream( &file );
            stream << ui->textBrowser->toPlainText() << endl;
            ui->intensityMap->saveBmp(dir.path()+"/AFM_Scan_image_"+time+".bmp");
        }
        else{QMessageBox::critical(this, tr("Error"), "File Can't Be Opened");}
    }
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

void intensitymap::on_colorPickDD_currentIndexChanged(int index)
{
   switch(index){
       case 0:
           colorMap->setGradient(QCPColorGradient::gpPolar);
            break;
       case 1:
            colorMap->setGradient(QCPColorGradient::gpThermal);
            break;
       case 2:
            colorMap->setGradient(QCPColorGradient::gpHot);
           break;
       case 3:
           colorMap->setGradient(QCPColorGradient::gpCold);
           break;
       case 4:
            colorMap->setGradient(QCPColorGradient::gpGrayscale);
           break;
    }
    ui->intensityMap->replot();

}

void intensitymap::on_invertRB_toggled(bool checked)
{
        colorMap->setGradient(colorMap->gradient().inverted());
        ui->intensityMap->replot();

}

intensitymap::~intensitymap()
{
    delete ui;
}

void intensitymap::on_interpolateRB_toggled(bool checked)
{
    colorMap->setInterpolate(checked);
    ui->intensityMap->replot();

}

void intensitymap::on_quit_pb_clicked()
{
    this->close();
}
