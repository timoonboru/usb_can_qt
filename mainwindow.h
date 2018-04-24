#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ControlCAN.h"
#include "resource.h"
#include <QLabel>

#include "canreceiver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum { IDD = IDD_DEMOCAN_DIALOG };
    //CColoredListCtrl	m_list;
    int		m_nSendFrameFormat;
    int		m_nSendFrameType;
    //CString	m_strSendData;
    QString	m_strSendData;
    //CString	m_strSendID;
    QString	m_strSendID;
    int		m_radioIDFormat;
    //BOOL	m_bCanRxEn;
    bool	m_bCanRxEn;
    int		m_nCanIndex;
    int		m_nDevType;

    int DeviceIndex;
    int m_DevType;
    int m_DevIndex;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    QLabel *qlab_d;
    QLabel *qlab_a;
    QLabel *qlab_v;

    QLabel *qlab_delete;

    QLabel *qlab_hex;

    int remember[40];

private slots:

    void receiveMessage(int id, float dist, float angle, float velocity, QString hex);

    void on_btn_open_clicked();

    void on_btn_close_clicked();

    void on_btn_receive_clicked();

private:
    Ui::MainWindow *ui;

    CanReceiver *canreceiver_thr;

};

#endif // MAINWINDOW_H
