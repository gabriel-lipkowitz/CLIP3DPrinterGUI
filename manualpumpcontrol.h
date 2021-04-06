#ifndef MANUALPUMPCONTROL_H
#define MANUALPUMPCONTROL_H

#include <QWidget>

namespace Ui {
class manualpumpcontrol;
}

class manualpumpcontrol : public QWidget
{
    Q_OBJECT

public:
    explicit manualpumpcontrol(QWidget *parent = nullptr);
    ~manualpumpcontrol();

private slots:
    void on_ConnectButton_clicked();

    void on_GetInfuseRate_clicked();

    void on_SetInfuseRate_clicked();



    void on_StartInfuse_clicked();

    void on_StopInfuse_clicked();

    void on_StartTargetInfuse_clicked();

    void on_SetTargetTime_clicked();

    void on_SetTargetVolume_clicked();

    void on_UseTime_clicked();

    void on_UseVolume_clicked();

private:
    Ui::manualpumpcontrol *ui;
    char* SerialRead();
};

#endif // MANUALPUMPCONTROL_H
