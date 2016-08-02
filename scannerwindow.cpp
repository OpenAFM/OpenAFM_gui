#include "scannerwindow.h"
#include "ui_scannerwindow.h"
#include "tx_rx_protocol.h"
#include "surfacegraph.h"




#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPainter>
#include <QtGui/QScreen>
#include <QObject>
#include <QDebug>
#include <QImage>


scannerwindow::scannerwindow(QMainWindow *parent, QSerialPort *serial):
    QObject(parent)
{

    widget = new QWidget();
    widget->setAttribute(Qt::WA_DeleteOnClose);
    graph = new Q3DSurface();
    QWidget* container = QWidget::createWindowContainer(graph, widget);
    container->setAttribute(Qt::WA_AcceptTouchEvents);

    if (!graph->hasContext()) {
        QMessageBox msgBox;
        msgBox.setText("Couldn't initialize the OpenGL context.");
        msgBox.exec();
    }

    QSize screenSize = graph->screen()->size();
    container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.6));
    container->setMaximumSize(screenSize);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);


    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    QVBoxLayout *vLayout = new QVBoxLayout();
    hLayout->addWidget(container, 1);
    hLayout->addLayout(vLayout);
    vLayout->setAlignment(Qt::AlignTop);

    widget->setWindowTitle(QStringLiteral("AFM Scan"));

    QGroupBox *modelGroupBox = new QGroupBox(QStringLiteral("Model"));




    AFM_Scan_3D_RB = new QPushButton(widget);
    AFM_Scan_3D_RB->setText(QStringLiteral("Start Scan"));
    AFM_Scan_3D_RB->setCheckable(true);
    AFM_Scan_3D_RB->setChecked(false);


    BitmapView = new QPushButton(widget);
    BitmapView->setText(QStringLiteral("Show Bitmap"));

    QVBoxLayout *modelVBox = new QVBoxLayout;
    modelVBox->addWidget(AFM_Scan_3D_RB);
    modelVBox->addWidget(BitmapView);

    modelGroupBox->setLayout(modelVBox);

    QGroupBox *selectionGroupBox = new QGroupBox(QStringLiteral("Selection Mode"));

    QRadioButton *modeNoneRB = new QRadioButton(widget);
    modeNoneRB->setText(QStringLiteral("No selection"));
    modeNoneRB->setChecked(false);

    QRadioButton *modeItemRB = new QRadioButton(widget);
    modeItemRB->setText(QStringLiteral("Item"));
    modeItemRB->setChecked(false);

    QRadioButton *modeSliceRowRB = new QRadioButton(widget);
    modeSliceRowRB->setText(QStringLiteral("Row Slice"));
    modeSliceRowRB->setChecked(false);

    QRadioButton *modeSliceColumnRB = new QRadioButton(widget);
    modeSliceColumnRB->setText(QStringLiteral("Column Slice"));
    modeSliceColumnRB->setChecked(false);

    QVBoxLayout *selectionVBox = new QVBoxLayout;
    selectionVBox->addWidget(modeNoneRB);
    selectionVBox->addWidget(modeItemRB);
    selectionVBox->addWidget(modeSliceRowRB);
    selectionVBox->addWidget(modeSliceColumnRB);
    selectionGroupBox->setLayout(selectionVBox);

    QSlider *axisCameraSliderZ = new QSlider(Qt::Horizontal, widget);
    axisCameraSliderZ->setMinimum(0);
    axisCameraSliderZ->setMaximum(179);
    axisCameraSliderZ->setTickInterval(1);
    axisCameraSliderZ->setEnabled(true);

    QSlider *axisCameraSliderY = new QSlider(Qt::Horizontal, widget);
    axisCameraSliderY->setMinimum(0);
    axisCameraSliderY->setMaximum(180);
    axisCameraSliderY->setTickInterval(0);
    axisCameraSliderY->setEnabled(true);


    QComboBox *themeList = new QComboBox(widget);
    themeList->addItem(QStringLiteral("Qt"));
    themeList->addItem(QStringLiteral("Primary Colors"));
    themeList->addItem(QStringLiteral("Digia"));
    themeList->addItem(QStringLiteral("Stone Moss"));
    themeList->addItem(QStringLiteral("Army Blue"));
    themeList->addItem(QStringLiteral("Retro"));
    themeList->addItem(QStringLiteral("Ebony"));
    themeList->addItem(QStringLiteral("Isabelle"));

    QGroupBox *colorGroupBox = new QGroupBox(QStringLiteral("Custom gradient"));

    QLinearGradient grBtoY(0, 0, 1, 100);
    grBtoY.setColorAt(1.0, Qt::black);
    grBtoY.setColorAt(0.67, Qt::blue);
    grBtoY.setColorAt(0.33, Qt::red);
    grBtoY.setColorAt(0.0, Qt::yellow);
    QPixmap pm(24, 100);
    QPainter pmp(&pm);
    pmp.setBrush(QBrush(grBtoY));
    pmp.setPen(Qt::NoPen);
    pmp.drawRect(0, 0, 24, 100);
    QPushButton *gradientBtoYPB = new QPushButton(widget);
    gradientBtoYPB->setIcon(QIcon(pm));
    gradientBtoYPB->setIconSize(QSize(24, 100));

    QLinearGradient grGtoR(0, 0, 1, 100);
    grGtoR.setColorAt(1.0, Qt::darkGreen);
    grGtoR.setColorAt(0.5, Qt::yellow);
    grGtoR.setColorAt(0.2, Qt::red);
    grGtoR.setColorAt(0.0, Qt::darkRed);
    pmp.setBrush(QBrush(grGtoR));
    pmp.drawRect(0, 0, 24, 100);
    QPushButton *gradientGtoRPB = new QPushButton(widget);
    gradientGtoRPB->setIcon(QIcon(pm));
    gradientGtoRPB->setIconSize(QSize(24, 100));

    QHBoxLayout *colorHBox = new QHBoxLayout;
    colorHBox->addWidget(gradientBtoYPB);
    colorHBox->addWidget(gradientGtoRPB);
    colorGroupBox->setLayout(colorHBox);

    vLayout->addWidget(modelGroupBox);
    vLayout->addWidget(selectionGroupBox);

    vLayout->addWidget(new QLabel(QStringLiteral("Camera Position")));
    vLayout->addWidget(axisCameraSliderZ);
    vLayout->addWidget(axisCameraSliderY);
    vLayout->addWidget(new QLabel(QStringLiteral("Theme")));
    vLayout->addWidget(themeList);
    vLayout->addWidget(colorGroupBox);

    widget->show();

    modifier= new SurfaceGraph(graph, serial, widget);



    QObject::connect(parent, SIGNAL(plotDataReceived(QByteArray)),
                     modifier, SLOT(dataHandler(QByteArray)));

    QObject::connect(widget,SIGNAL(destroyed()),modifier, SLOT(sendDone()));

    QObject::connect(this, SIGNAL (AFMDone()),
                     modifier, SLOT (sendDone()));

    QObject::connect(widget,SIGNAL(destroyed()),this, SLOT(close()));

    QObject::connect(modifier, SIGNAL (scanFinished()),
                     this, SLOT(endScan()));


    QObject::connect(BitmapView, SIGNAL (clicked()),
                     this, SLOT(getBitmapScreen()));

    QObject::connect(AFM_Scan_3D_RB, SIGNAL (toggled(bool)),
                     this, SLOT(AFMButtonHandler(bool)));

    QObject::connect(this, SIGNAL (AFMStart()),
                     modifier, SLOT (enableAFMModel()));

    QObject::connect(this, SIGNAL (AFMStart()),
                     modifier, SLOT (sendGo()));

    QObject::connect(this, SIGNAL (AFMStart()),
                     modifier, SLOT(sendReady()));

    QObject::connect(modeNoneRB, &QRadioButton::toggled,
                     modifier, &SurfaceGraph::toggleModeNone);
    QObject::connect(modeItemRB,  &QRadioButton::toggled,
                     modifier, &SurfaceGraph::toggleModeItem);
    QObject::connect(modeSliceRowRB,  &QRadioButton::toggled,
                     modifier, &SurfaceGraph::toggleModeSliceRow);
    QObject::connect(modeSliceColumnRB,  &QRadioButton::toggled,
                     modifier, &SurfaceGraph::toggleModeSliceColumn);


    QObject::connect(themeList, SIGNAL(currentIndexChanged(int)),
                     modifier, SLOT(changeTheme(int)));

    QObject::connect(gradientBtoYPB, &QPushButton::pressed,
                     modifier, &SurfaceGraph::setBlackToYellowGradient);
    QObject::connect(gradientGtoRPB, &QPushButton::pressed,
                     modifier, &SurfaceGraph::setGreenToRedGradient);

    QObject::connect(axisCameraSliderZ, &QSlider::valueChanged,
                     modifier, &SurfaceGraph::adjustCameraZ);
    QObject::connect(axisCameraSliderY, &QSlider::valueChanged,
                     modifier, &SurfaceGraph::adjustCameraY);







    AFM_Scan_3D_RB->setChecked(false);
    modeItemRB->setChecked(true);
    BitmapView->setEnabled(false);

    themeList->setCurrentIndex(2);
}




scannerwindow::~scannerwindow()
{
    qDebug()<<"closed";
}

void scannerwindow::close(){
    delete this;
}

void scannerwindow::getBitmapScreen(){
    intensitymap* BitmapScreen= new intensitymap(modifier->AFM_Proxy, widget);
    BitmapScreen->setModal(false);
    BitmapScreen->exec();
}

void scannerwindow::endScan(){
    AFM_Scan_3D_RB->setEnabled(false);
    AFM_Scan_3D_RB->setText("Done");
    BitmapView->setEnabled(true);



}

void scannerwindow::AFMButtonHandler(bool checked){
    if(!checked){
        emit AFMDone();
        AFM_Scan_3D_RB->setText("Stopped");
        AFM_Scan_3D_RB->setStyleSheet("color: #000000;");

        BitmapView->setEnabled(true);

    }
    else{
        emit AFMStart();
        AFM_Scan_3D_RB->setText("End Scan");
        AFM_Scan_3D_RB->setStyleSheet("color: #ffffff;");
        BitmapView->setEnabled(false);

    }
}



