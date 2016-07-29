#include "surfacegraph.h"
#include "def_commands.h"
#include <QtDataVisualization/QValue3DAxis>
#include <QtDataVisualization/Q3DTheme>
#include <QtGui/QImage>
#include <QtCore/qmath.h>


using namespace QtDataVisualization;


SurfaceGraph::SurfaceGraph(Q3DSurface *surface, QSerialPort *serial, QWidget *parent)
    :QObject(parent)
{

    m_graph=surface;
    m_graph->setAxisX(new QValue3DAxis);
    m_graph->setAxisY(new QValue3DAxis);
    m_graph->setAxisZ(new QValue3DAxis);


    AFM_Proxy=new QSurfaceDataProxy();
    AFM_Series = new QSurface3DSeries(AFM_Proxy);
    AFM_Series->setMeshSmooth(true);
    AFM_Proxy->resetArray(NULL);

    m_graph->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetFront);

    data_serial=serial;       
}


SurfaceGraph::~SurfaceGraph()
{
}

void SurfaceGraph::sendGo(){
    this->data_serial->write(response::GO);

}

void SurfaceGraph::sendReady(){
    this->data_serial->write(response::READY);
}

void SurfaceGraph::sendDone(){
    this->data_serial->write(response::DONE);
}

void SurfaceGraph::fillAFMProxy(QList <QByteArray> data)
{
        float stepX = 1;
        if(!size_set){
            int line_size=data.size()/2;
            sampleCountX=line_size;
            setAxisScaling(line_size);
            size_set=true;
        }
        if(sampleCountX==data.size()/2){
        QSurfaceDataRow *newRow = new QSurfaceDataRow(sampleCountX);
        qDebug()<<data;
        int index = 0;
        for (int j = 0; j<sampleCountX; j++) {
            float x = (j * stepX);
            float y = (data.front().toInt()+data.back().toInt())/2;
            data.pop_back();
            data.pop_front();
            float z = AFM_Proxy->rowCount();
            (*newRow)[index].setPosition(QVector3D(x, y, z+1));
            index++;

        }
        m_graph->axisZ()->setRange(0,AFM_Proxy->rowCount()+2);

        AFM_Proxy->insertRow(AFM_Proxy->rowCount(),newRow);

        sendReady();
}


}



void SurfaceGraph::enableAFMModel()
{


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


}

void SurfaceGraph::adjustCameraZ(int angle)
{

    this->m_graph->scene()->activeCamera()->setXRotation(angle/2);
}



void SurfaceGraph::adjustCameraY(int angle)
{
    this->m_graph->scene()->activeCamera()->setYRotation(angle/2);

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
void SurfaceGraph::setAxisScaling(int sizeX){

    m_graph->axisX()->setRange(0,sizeX);

}

void SurfaceGraph::dataHandler(QByteArray data){

        data.replace(";","");
        QList <QByteArray> splitData=data.split(',');
        qDebug()<<"last element"<<splitData.back();
        fillAFMProxy(splitData);
}
