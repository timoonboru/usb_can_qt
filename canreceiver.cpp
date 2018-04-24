#include "canreceiver.h"

#include <QDebug>
#include <common.h>
#include <QLabel>

CanReceiver::CanReceiver(int DevType , int DevIndex )
{
    m_DevIndex = DevIndex;
    m_DevType = DevType;
}

CanReceiver::~CanReceiver()
{
    qDebug() << "ThreadFromQThread::~ThreadFromQThread()";
}

void CanReceiver::run()
{

    int k=0;
    while(1)
    {
        int NumValue;
        int i;
        VCI_CAN_OBJ pCanObj[200];
        QString strbuf[200],str1;
        int num=0;
        QString str;
        DWORD ReceivedID;

        for(int kCanIndex=0;kCanIndex<2;kCanIndex++)
        {
            //调用动态链接看接收函数
            NumValue=VCI_Receive(m_DevType,m_DevIndex,kCanIndex,pCanObj,200,0);
            //接收信息列表显示
            k++;
            //qDebug()<<"NumValue = "<<NumValue ;
            for(num=0;num<NumValue;num++)
            {

                str="";
                unsigned char byteData[8];
                   // qDebug()<<(pCanObj[num].DataLen) ;
                for(i=0;i<(pCanObj[num].DataLen);i++)	//数据信息
                {

                    str1.setNum(pCanObj[num].Data[i],16);
                    str +=str1;

                    byteData[i] = (unsigned char)pCanObj[num].Data[i];

                }
                //qDebug()<<str;

                if( !((byteData[1]==31 && byteData[2]==255) || (byteData[0]==0 && byteData[1]==0)))
                {
                    ReceivedID = pCanObj[num].ID - 1024 + 1;

                    float dist = GetDistance(byteData);
                    float angle = GetAngle(byteData);
                    float velocity = GetVelocity(byteData);
                    //qDebug()<< "ID = " << ReceivedID <<"dist = " << dist << "angle = "<<angle << "velocity = " <<velocity;
                    emit message(ReceivedID, dist, angle, velocity , str);
                }
                 //接收信息列表显示完毕
            }
        }
        Sleep(10);
    }

}


