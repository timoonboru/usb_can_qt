#ifndef COMMON_H
#define COMMON_H

//一位十六进制转换为十进制
int HexChar(char c)
{
    if((c>='0') && (c<='9'))
        return c-0x30;
    else if((c>='A') && (c<='F'))
        return c-'A'+10;
    else if((c>='a') && (c<='f'))
        return c-'a'+10;
    else
        return 0x10;
}
//两位十六进制数转换为十进制
int Str2Hex(QString str)
{
    int len = str.length();
    if(len == 2)
    {
        int a= HexChar((unsigned char)str.at(0).toLatin1());
        int b =HexChar((unsigned char)str.at(1).toLatin1());
        if(a==16 || b==16 )
        {

            return 256;
        }
        else
        {
            return a*16+b;

        }

    }
    else
    {
        return 256;
    }
}

/************************************************/
/*************函数功能：获取距离信息*************/
/*输入值:CanByteData, 得到的CAN线上8byte的信息  */
/*返回值:dis_f,得到对应的距离信息,单位:m        */
/************************************************/
float GetDistance(unsigned char *CanByteData)
{
    int dis;
    float dis_f;
    dis = CanByteData[1] & 0x1F;
    dis <<= 8;
    dis += CanByteData[2];
    dis_f = (float)(dis * 0.05);
    return dis_f;
}

/**************************************************/
/**************函数功能:获取速度信息************* */
/*输入值:CanByteData, 得到的CAN上的8byte的信息    */
/*返回值:velocity,得到对应的速度信息,单位:m/s     */
/**************************************************/
float GetVelocity(unsigned char *CanByteData)
{
    int velocity;
    float velocity_f;
    velocity = CanByteData[3];
    velocity <<= 8;
    velocity += CanByteData[4];
    if((CanByteData[3] & 0x80) == 0)
    {
        velocity_f = (float)(velocity * 0.1);
    }
    else
    {
        velocity_f = (float)((velocity - 65536) * 0.1);
    }
    return velocity_f;
}

/**************************************************/
/**************函数功能:获取角度信息************* */
/*输入值:CanByteData, 得到的CAN线上8byte的信息    */
/*返回值:angle,得到对应的速度信息,单位:deg        */
/**************************************************/
float GetAngle(unsigned char *CanByteData)
{
    int angle;
    float angle_f;
    if((CanByteData[1] & 0x80) == 0)
    {
        angle = (CanByteData[1] >> 5) & 0x07;
        angle <<= 8;
        angle += CanByteData[5];
        angle_f = (float)(angle * 0.1);
    }
    else
    {
        angle = (CanByteData[1] >> 5) & 0x07;
        angle <<= 8;
        angle += CanByteData[5];
        angle_f = (float)((angle - 2048) * 0.1);
    }
    return angle_f;
}

#endif // COMMON_H
