
#ifndef SURFACEGRAPH_H
#define SURFACEGRAPH_H

#include "mainwindow.h"

#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QHeightMapSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtWidgets/QSlider>
#include <QtSerialPort/QSerialPort>


using namespace QtDataVisualization;

class SurfaceGraph : public QObject
{
    Q_OBJECT
public:
    explicit SurfaceGraph(Q3DSurface *surface, QSerialPort *serial, QWidget *main);
    ~SurfaceGraph();


    bool size_set=false;
    void enableAFMModel(bool enable);

    void toggleModeNone() { m_graph->setSelectionMode(QAbstract3DGraph::SelectionNone); }
    void toggleModeItem() { m_graph->setSelectionMode(QAbstract3DGraph::SelectionItem); }
    void toggleModeSliceRow() { m_graph->setSelectionMode(QAbstract3DGraph::SelectionItemAndRow
                                                          | QAbstract3DGraph::SelectionSlice); }
    void toggleModeSliceColumn() { m_graph->setSelectionMode(QAbstract3DGraph::SelectionItemAndColumn
                                                             | QAbstract3DGraph::SelectionSlice); }

    void setBlackToYellowGradient();
    void setGreenToRedGradient();

    void adjustCameraZ(int angle);
    void adjustCameraY(int angle);

    void setAxisCameraSliderZ(QSlider *slider) { m_axisCameraSliderZ = slider; }
    void setAxisCameraSliderY(QSlider *slider) { m_axisCameraSliderY = slider; }

    void adjustXMin(int min);
    void adjustXMax(int max);
    void adjustZMin(int min);
    void adjustZMax(int max);
    void setAxisScaling(int size);

    int sampleCountX = 50;
    int sampleCountZ = 50;
    float sampleMin = -8.0f;
    float sampleMax = 8.0f;


public Q_SLOTS:
    void sendReady();
    void sendGo();
    void changeTheme(int theme);
    void fillAFMProxy(QList<QByteArray> data);

private:
    Q3DSurface *m_graph;
    QSurface3DSeries *AFM_Series;
    QSurfaceDataProxy *AFM_Proxy;


    QSlider *m_axisCameraSliderZ;
    QSlider *m_axisCameraSliderY;

    float m_rangeMinX;
    float m_rangeMinZ;
    float m_stepX;
    float m_stepZ;
    int m_heightMapWidth;
    int m_heightMapHeight;

    QSerialPort* data_serial;

    void setAxisXRange(float min, float max);
    void setAxisZRange(float min, float max);


signals:
    void sendSerial(QByteArray data);
    void scanFinished();

};

#endif // SURFACEGRAPH_H
