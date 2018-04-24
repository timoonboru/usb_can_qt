#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QDebug>
//#include <common.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_nSendFrameFormat = 0;
    m_nSendFrameType = 0;
    //m_strSendData = _T("11 22 33 44 55 66 77 88");
    //m_strSendID = _T("00 00 00 88");
    m_radioIDFormat = 1;
    m_bCanRxEn = FALSE;
    m_nCanIndex = 0;
    m_nDevType = 1;

    canreceiver_thr = NULL;

    for(int i = 0; i < 40; i++ )
    {
        remember[i] = 0;
    }

    //connect(canreceiver_thr,&CanReceiver::message,this,&MainWindow::receiveMessage);
}

MainWindow::~MainWindow()
{
    if(canreceiver_thr != NULL)
    {
        canreceiver_thr->quit();
    }
    delete ui;
}

void MainWindow::receiveMessage(int id, float dist, float angle, float velocity, QString hex)
{
    //qDebug()<< "ID = " << id <<"dist = " << dist << "angle = "<<angle << "velocity = " <<velocity;

    QFont ft;
    ft.setPointSize(14);

    qlab_hex = this->findChild<QLabel *>("hex_lab");
    qlab_hex->setText(hex);
    qlab_hex->setFont(ft);

    QString id_str = QString::number(id);
    QString dx_lab = "d" + id_str + "_lab";
    QString ax_lab = "a" + id_str + "_lab";
    QString vx_lab = "v" + id_str + "_lab";

    qlab_d = this->findChild<QLabel *>(dx_lab);
    qlab_a = this->findChild<QLabel *>(ax_lab);
    qlab_v = this->findChild<QLabel *>(vx_lab);

    qlab_d->setText( QString("%1").arg(dist));
    qlab_a->setText( QString("%1").arg(angle));
    qlab_v->setText( QString("%1").arg(velocity));

    qlab_d->setFont(ft);
    qlab_a->setFont(ft);
    qlab_v->setFont(ft);

    remember[id-1] = 10;
    for(int i = 0; i < 40; i++)
    {
        remember[i]--;
        if(remember[i] == 1)
        {
            id_str = QString::number(i+1);

            dx_lab = "d" + id_str + "_lab";
            ax_lab = "a" + id_str + "_lab";
            vx_lab = "v" + id_str + "_lab";

            qlab_d = this->findChild<QLabel *>(dx_lab);
            qlab_a = this->findChild<QLabel *>(ax_lab);
            qlab_v = this->findChild<QLabel *>(vx_lab);

            qlab_d->setText( QString("%1").arg(0));
            qlab_a->setText( QString("%1").arg(0));
            qlab_v->setText( QString("%1").arg(0));

        }
    }
}



void MainWindow::on_btn_open_clicked()
{
    m_DevIndex=0;
    m_DevType= m_nDevType == 1 ?  VCI_USBCAN2 :  VCI_USBCAN1;
    DWORD Reserved=0;
    //打开设备
    if(VCI_OpenDevice(m_DevType,m_DevIndex,Reserved)!=1)
    {
        //MessageBox("open failed");
        QMessageBox::information(NULL, "OpenDevice", "open failed",
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }
    VCI_INIT_CONFIG InitInfo[1];
    InitInfo->Timing0=0x00;
    InitInfo->Timing1=0x1C;
    InitInfo->Filter=0;
    InitInfo->AccCode=0x80000008;
    InitInfo->AccMask=0xFFFFFFFF;
    InitInfo->Mode=0;
    //初始化通道0
    if(VCI_InitCAN(m_DevType,m_DevIndex, 0,InitInfo)!=1)	//can-0
    {
        //MessageBox("Init-CAN failed!");
        QMessageBox::information(NULL, "OpenDevice", "Init-CAN failed",
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }
    Sleep(100);
    //初始化通道0
    if(VCI_StartCAN(m_DevType,m_DevIndex, 0)!=1)	//can-0
    {
        //MessageBox("Start-CAN failed!");
        QMessageBox::information(NULL, "OpenDevice", "Start-CAN failed!",
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }
    //初始化通道1
    if(m_nDevType == 1)
    {
        if(VCI_InitCAN(m_DevType,m_DevIndex, 1,InitInfo)!=1)	//can-1
        {
            //MessageBox("Init-CAN failed!");
            QMessageBox::information(NULL, "OpenDevice", "Init-CAN failed!",
                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            return;
        }
    }
    Sleep(100);
    //初始化通道1
    if(VCI_StartCAN(m_DevType,m_DevIndex, 1)!=1)	//can-0
    {
        //MessageBox("Start-CAN failed!");
        QMessageBox::information(NULL, "OpenDevice", "Start-CAN failed!",
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }
                qDebug()<<m_DevType << m_DevIndex;
    //MessageBox("Open successfule!\n Start CAN OK!");
    QMessageBox::information(NULL, "OpenDevice", "Open successfule!\n Start CAN OK!",
                             QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
}


void MainWindow::on_btn_close_clicked()
{
    if(VCI_CloseDevice(m_DevType,m_DevIndex)!=1)
    {
        //MessageBox("Close failed！");
        QMessageBox::information(NULL, "CloseDevice", "Close failed！",
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;

    }

    //MessageBox("Close successful!");
    QMessageBox::information(NULL, "CloseDevice", "Close successful!",
                             QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

}

void MainWindow::on_btn_receive_clicked()
{
    if(canreceiver_thr == NULL)
    {
        canreceiver_thr = new CanReceiver(m_DevType,m_DevIndex);
    }

    connect(canreceiver_thr,SIGNAL(message(int, float, float, float, QString)),this,
            SLOT(receiveMessage(int, float, float, float, QString)));
    canreceiver_thr->start();

}
