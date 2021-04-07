#include "manualpumpcontrol.h"
#include "ui_manualpumpcontrol.h"
#include <QTimer>
#include <QActionGroup>

#include "serialib.h"
#include "SMC100C.h"

SMC100C SMC;

static bool ConnectionFlag;

static bool UseTargetTime;

static double InfusionRate;

static double InfusionTime;

static double InfusionVolume;


manualpumpcontrol::manualpumpcontrol(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::manualpumpcontrol)
{
    ui->setupUi(this);
}

manualpumpcontrol::~manualpumpcontrol()
{
    delete ui;
}

void manualpumpcontrol::on_ConnectButton_clicked()
{

    if (SMC.serial.openDevice("COM3",9600) == 1)
    {
        ui->TerminalOut->append("Serial Port Connected");

        ui->ConnectionIndicator->setStyleSheet("background:rgb(0, 255, 0); border: 1px solid black;");
        ui->ConnectionIndicator->setText("Connected");

        ConnectionFlag = true;
        Sleep(50);
        QString Command = ".";
        const char* CommandToSend = Command.toLatin1().data();
        SMC.serial.writeString(CommandToSend);
        Sleep(50);
        QString Command2 = "cmd.\r\n";
        const char* Command2ToSend = Command2.toLatin1().data();
        SMC.serial.writeString(CommandToSend);
        Sleep(50);
        QString Command3 = "ver.\r\n";
        const char* Command3ToSend = Command3.toLatin1().data();
        SMC.serial.writeString(CommandToSend);
    }
    else
    {
        ui->TerminalOut->append("Serial Port Connection Failed");

        ui->ConnectionIndicator->setStyleSheet("background:rgb(255, 0, 0); border: 1px solid black;");
        ui->ConnectionIndicator->setText("Disconnected");

        ConnectionFlag = false;
    }
}



 /*
void manualPumpControl::setInfusionRate(double InfusionRate)
{

}
*/


void manualpumpcontrol::on_GetInfuseRate_clicked()
{
    InfusionRate = ui->NewInfuseRateParam->value();
    QString ExpDarkRatioString = "Set Infusion Rate to: " + QString::number(InfusionRate);
    ui->TerminalOut->append(ExpDarkRatioString);

    QString Command = "irate.";
    const char* CommandToSend = Command.toLatin1().data();
    SMC.serial.writeString(CommandToSend);
    QString ReadVal = SerialRead();

    ui->NewInfuseRateParam->setValue(ReadVal.toDouble());
    ui->TerminalOut->append("Current Infusion Rate: " + ReadVal);


}

void manualpumpcontrol::on_SetInfuseRate_clicked()
{
    InfusionRate = ui->NewInfuseRateParam->value();
    QString ExpDarkRatioString = "Set Infusion Rate to: " + QString::number(InfusionRate);
    ui->TerminalOut->append(ExpDarkRatioString);

    QString Command = "irate " + QString::number(InfusionRate) + " ul/s.\r\n";
    const char* CommandToSend = Command.toLatin1().data();
    SMC.serial.writeString(CommandToSend);
    /*
    QString Command2 = "cmd.\r\n";
    const char* Command2ToSend = Command2.toLatin1().data();
    SMC.serial.writeString(CommandToSend);

    QString Command3 = "ver.\r\n";
    const char* Command3ToSend = Command3.toLatin1().data();
    SMC.serial.writeString(CommandToSend);
    */



}

char* manualpumpcontrol::SerialRead()
{
    char* receivedString;
    char finalChar;
    unsigned int maxNbBytes = 13;
    int ReadStatus;
    ReadStatus = SMC.serial.readString(receivedString,finalChar,maxNbBytes,250);
/*
    char ReadChar;
    if (ReadStatus > 0)
    {
        return receivedString;
    }
    else if(ReadStatus == 0)
    {
        char* errString = "Timeout Reached";
        return  receivedString;
    }
    else if(ReadStatus == -1)
    {
        char* errString = "Error Setting Timeout";
        return  errString;
    }
    else if(ReadStatus == -2)
    {
        char* errString = "Error while reading byte";
        return  receivedString;
    }
    else if(ReadStatus == -3)
    {
        char* errString = "Max N bytes reached";
        return  receivedString;
    }
    */
   return receivedString;
}

void manualpumpcontrol::on_StartInfuse_clicked()
{
    QString InfuseString = "Start infusion at rate = " + QString::number(InfusionRate) + "ul/s";
    ui->TerminalOut->append(InfuseString);

    QString Command = "irun.\r\n";
    const char* CommandToSend = Command.toLatin1().data();
    SMC.serial.writeString(CommandToSend);
}

void manualpumpcontrol::on_StopInfuse_clicked()
{
    QString StopString = "Stop infusion";
    ui->TerminalOut->append(StopString);

    QString Command = "stop.\r\n";
    const char* CommandToSend = Command.toLatin1().data();
    SMC.serial.writeString(CommandToSend);

    QString Command2 = "cmd.\r\n";
    const char* Command2ToSend = Command2.toLatin1().data();
    SMC.serial.writeString(CommandToSend);

    QString Command3 = "ver.\r\n";
    const char* Command3ToSend = Command3.toLatin1().data();
    SMC.serial.writeString(CommandToSend);
}

void manualpumpcontrol::on_StartTargetInfuse_clicked()
{
    on_StartInfuse_clicked();

    if (UseTargetTime)
        QTimer::singleShot(InfusionTime*1000, Qt::PreciseTimer, this, SLOT(on_StopInfuse_clicked()));

    else
    {
        double InferredTargetTime = (InfusionVolume*1000) / InfusionRate; // [s] = [mL*1000] / [ul/s]
        QTimer::singleShot(InferredTargetTime*1000, Qt::PreciseTimer, this, SLOT(on_StopInfuse_clicked()));
    }
}

void manualpumpcontrol::on_SetTargetTime_clicked()
{
    InfusionTime = (ui->NewTargetTime->value());
    QString InfusionTimeString = QString::number(InfusionTime);
    ui->TerminalOut->append("Set Target Infusion time to " + InfusionTimeString + "s");
    ui->CurrentTargetTime->setText(InfusionTimeString);
}

void manualpumpcontrol::on_SetTargetVolume_clicked()
{
    InfusionVolume = (ui->NewTargetVolume->value());
    QString InfusionVolumeString = QString::number(InfusionVolume);
    ui->TerminalOut->append("Set Target Infusion Volume to " + InfusionVolumeString + "mL");
    ui->CurrentTargetVolume->setText(InfusionVolumeString);
}

void manualpumpcontrol::on_UseTime_clicked()
{
    UseTargetTime = true;
}

void manualpumpcontrol::on_UseVolume_clicked()
{
    UseTargetTime = false;
}
