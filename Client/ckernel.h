#ifndef CKERNEL_H
#define CKERNEL_H

#include <QObject>
#include "dialog.h"
#include "logindialog.h"
#include "INetMediator.h"
#include "packdef.h"



class CKernel : public QObject
{
    Q_OBJECT
public:
    explicit CKernel(QObject *parent = 0);
    //函数指针
    typedef void (CKernel::*PFUN)(char* buf, int nlen);
signals:
    void SIG_REGISTER_ATTENTION();
    void SIG_LOGIN_ATTENTION();
public slots:
    void DestroyedInstance();
    void slot_dealData(long lSendIP, char* buf, int nlen);
    void setProtocolMap();
    //注册回复
    void slot_registerRs(char* buf, int nlen);
    //登录回复
    void slot_loginRs(char* buf, int nlen);

    void slot_LOGIN_RQ(QString username,QString password);
    void slot_REGISTER_RQ(QString username,QString password);
public:
    void Utf8ToGB2312( char* gbbuf , int nlen ,QString& utf8);
    QString GB2312ToUtf8( char* gbbuf );
private:
    Dialog * m_mainWnd;
    LoginDialog * m_loginWnd;
    INetMediator * m_tcpClient;

    //协议映射表（函数指针数组）协议头-->函数指针
    PFUN m_NetProtocolMap[_DEF_PROTOCOL_COUNT];
};

#endif // CKERNEL_H
