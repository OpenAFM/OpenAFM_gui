#include "surfacegraph.h"

#include <QtDataVisualization/QValue3DAxis>
#include <QtDataVisualization/Q3DTheme>
#include <QtGui/QImage>
#include <QtCore/qmath.h>


using namespace QtDataVisualization;


SurfaceGraph::SurfaceGraph(Q3DSurface *surface, QSerialPort *serial, QWidget *main)
    : m_graph(surface)
{

    m_graph->setAxisX(new QValue3DAxis);
    m_graph->setAxisY(new QValue3DAxis);
    m_graph->setAxisZ(new QValue3DAxis);


    AFM_Proxy=new QSurfaceDataProxy();
    AFM_Series = new QSurface3DSeries(AFM_Proxy);
    AFM_Proxy->resetArray(NULL);

    m_graph->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetFront);

    data_serial=serial;
    sampleCountX = 250;
    sampleCountZ = 250;
    sampleMin = 0;
    sampleMax = 8;
}

SurfaceGraph::~SurfaceGraph()
{
    delete m_graph;
}

void SurfaceGraph::sendGo(){
    QByteArray data;
    data.append("GO;");
    this->data_serial->write(data);
    qDebug()<<"Sent Signal from surface";

    emit sendSerial(data);
}

void SurfaceGraph::sendReady(){
    this->data_serial->write("RDY;");
}

void SurfaceGraph::fillAFMProxy(QList <QByteArray> data)
{

        bool lastline=false;
        QTime time;
        float stepX = 1;

        if(!size_set){
            int line_size=data.size()/2;
            sampleCountX=line_size;
            setAxisScaling(line_size);
            size_set=true;
        }
        if(sampleCountX==data.size()/2){
        QSurfaceDataRow *newRow = new QSurfaceDataRow(sampleCountX);
        qDebug()<<"Adding Row"<<sampleCountX<<QTime::currentTime();
        int index = 0;
        for (int j = 0; j<sampleCountX; j++) {
            float x = (j * stepX);
            float y = (data[j].toInt()+data[sampleCountX-1-j].toInt())/2;
            float z = AFM_Proxy->rowCount();
            (*newRow)[index].setPosition(QVector3D(x, y, z+1));
            index++;

        }
        m_graph->axisZ()->setRange(0,AFM_Proxy->rowCount()+2);

        AFM_Proxy->insertRow(AFM_Proxy->rowCount(),newRow);

        sendReady();
}

}



void SurfaceGraph::enableAFMModel(bool enable)
{
    if (enable) {
        AFM_Series->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
        AFM_Series->setFlatShadingEnabled(true);

        m_graph->axisX()->setLabelFormat("%.2f");
        m_graph->axisZ()->setLabelFormat("%.2f");
        m_graph->axisX()->setAutoAdjustRange(true);
        m_graph->axisY()->setAutoAdjustRange(true);
        m_graph->axisZ()->setAutoAdjustRange(true);
        m_graph->axisX()->setLabelAutoRotation(30);
        m_graph->axisY()->setLabelAutoRotation(90);
        m_graph->axisZ()->setLabelAutoRotation(30);

        m_graph->addSeries(AFM_Series);

        // Reset range sliders for Sqrt&Sin
        m_rangeMinX = sampleMin;
        m_rangeMinZ = sampleMin;
        m_stepX = (sampleMax - sampleMin) / float(sampleCountX - 1);
        m_stepZ = (sampleMax - sampleMin) / float(sampleCountZ - 1);
        m_axisMinSliderX->setMaximum(sampleCountX - 2);
        m_axisMinSliderX->setValue(0);
        m_axisMaxSliderX->setMaximum(sampleCountX - 1);
        m_axisMaxSliderX->setValue(sampleCountX - 1);
        m_axisMinSliderZ->setMaximum(sampleCountZ - 2);
        m_axisMinSliderZ->setValue(0);
        m_axisMaxSliderZ->setMaximum(sampleCountZ - 1);
        m_axisMaxSliderZ->setValue(sampleCountZ - 1);
    }
}

void SurfaceGraph::adjustCameraZ(int angle)
{

    this->m_graph->scene()->activeCamera()->setXRotation(angle/2);
}




void SurfaceGraph::adjustCameraY(int angle)
{
    this->m_graph->scene()->activeCamera()->setYRotation(angle/2);

}
void SurfaceGraph::adjustXMin(int min)
{
    float minX = m_stepX * float(min) + m_rangeMinX;

    int max = m_axisMaxSliderX->value();
    if (min >= max) {
        max = min + 1;
        m_axisMaxSliderX->setValue(max);
    }
    float maxX = m_stepX * max + m_rangeMinX;

    setAxisXRange(minX, maxX);
}

void SurfaceGraph::adjustXMax(int max)
{
    float maxX = m_stepX * float(max) + m_rangeMinX;

    int min = m_axisMinSliderX->value();
    if (max <= min) {
        min = max - 1;
        m_axisMinSliderX->setValue(min);
    }
    float minX = m_stepX * min + m_rangeMinX;

    setAxisXRange(minX, maxX);
}

void SurfaceGraph::adjustZMin(int min)
{
    float minZ = m_stepZ * float(min) + m_rangeMinZ;

    int max = m_axisMaxSliderZ->value();
    if (min >= max) {
        max = min + 1;
        m_axisMaxSliderZ->setValue(max);
    }
    float maxZ = m_stepZ * max + m_rangeMinZ;

    setAxisZRange(minZ, maxZ);
}

void SurfaceGraph::adjustZMax(int max)
{
    float maxX = m_stepZ * float(max) + m_rangeMinZ;

    int min = m_axisMinSliderZ->value();
    if (max <= min) {
        min = max - 1;
        m_axisMinSliderZ->setValue(min);
    }
    float minX = m_stepZ * min + m_rangeMinZ;

    setAxisZRange(minX, maxX);
}

void SurfaceGraph::setAxisXRange(float min, float max)
{
    //m_graph->axisX()->setRange(min, max);
}

void SurfaceGraph::setAxisZRange(float min, float max)
{
   // m_graph->axisZ()->setRange(min, max);
}

void SurfaceGraph::changeTheme(int theme)
{
    m_graph->activeTheme()->setType(Q3DTheme::Theme(theme));
}

void SurfaceGraph::setBlackToYellowGradient()
{
    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::black);
    gr.setColorAt(0.33, Qt::blue);
    gr.setColorAt(0.67, Qt::red);
    gr.setColorAt(1.0, Qt::yellow);

    m_graph->seriesList().at(0)->setBaseGradient(gr);
    m_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
}

void SurfaceGraph::setGreenToRedGradient()
{
    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::darkGreen);
    gr.setColorAt(0.5, Qt::yellow);
    gr.setColorAt(0.8, Qt::red);
    gr.setColorAt(1.0, Qt::darkRed);

    m_graph->seriesList().at(0)->setBaseGradient(gr);
    m_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
}
void SurfaceGraph::setAxisScaling(int size){
    sampleMin = 0;
    sampleMax = size;

    m_graph->axisX()->setRange(0,size);

    m_rangeMinX = 0;
    m_rangeMinZ = 0;
    m_stepX = (size - sampleMin) / float(sampleCountX - 1);
    m_stepZ = (size - sampleMin) / float(sampleCountZ - 1);
    m_axisMinSliderX->setMaximum(size - 1);
    m_axisMinSliderX->setValue(0);
    m_axisMaxSliderX->setMaximum(size - 1);
    m_axisMaxSliderX->setValue(size - 1);

    m_axisMinSliderZ->setMaximum(size - 2);
    m_axisMinSliderZ->setValue(0);
    m_axisMaxSliderZ->setMaximum(size - 1);
    m_axisMaxSliderZ->setValue(size - 1);
}
