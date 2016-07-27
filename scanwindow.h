#ifndef SCANWINDOW_H
#define SCANWINDOW_H

#include <QMainWindow>

namespace Ui {
class ScanWindow;
}

class ScanWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ScanWindow(QWidget *parent = 0);
    ~ScanWindow();

private:
    Ui::ScanWindow *ui;
};

#endif // SCANWINDOW_H
