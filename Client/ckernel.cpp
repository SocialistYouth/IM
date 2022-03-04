#include "Ckernel.h"
#include "TcpClientMediator.h"
#include "qDebug"
#include "QMessageBox"

//设置协议映射关系
void CKernel::setProtocolMap()
{
    memset(m_NetProtocolMap,0,sizeof(m_NetProtocolMap));
    m_NetProtocolMap[_DEF_PACK_REGISTER_RS - _DEF_PROTOCOL_BASE] = &CKernel::slot_registerRs;
    m_NetProtocolMap[_DEF_PACK_LOGIN_RS - _DEF_PROTOCOL_BASE   ] = &CKernel::slot_loginRs;
}

CKernel::CKernel(QObject *parent) : QObject(parent)
{
    setProtocolMap();//构造函数先定义协议映射关系
    m_tcpClient = new TcpClientMediator();
    connect(m_tcpClient,SIGNAL(SIG_readyData(long,char*,int)),
            this,SLOT(slot_dealData(long,char*,int)));
    if(!m_tcpClient->OpenNet())
    {
        qDebug() << "连接服务器失败";
        delete m_tcpClient;
        exit(0);
    }
    m_loginWnd = new LoginDialog;
    connect(m_loginWnd,SIGNAL(SIG_LOGIN_RQ(QString,QString)),this,SLOT(slot_LOGIN_RQ(QString,QString)));
    connect(m_loginWnd,SIGNAL(SIG_REGISTER_RQ(QString,QString)),this,SLOT(slot_REGISTER_RQ(QString,QString)));
    connect(this,SIGNAL(SIG_REGISTER_ATTENTION()),m_loginWnd,SLOT(slot_REGISTER_ATTENTION()));
    connect(this,SIGNAL(SIG_LOGIN_ATTENTION()),m_loginWnd,SLOT(slot_LOGIN_ATTENTION()));
    m_loginWnd->show();
    m_mainWnd = new MyChatDialog;
    for( int i = 0 ; i< 5 ;++i)
    {
        userItem * item = new userItem;
        item->slot_setInfo( i+1 , QString("用户%1").arg(i+1)
                            ,1, i+1 );
        m_mainWnd->slot_addFriend( item );
    }
    for( int i = 5 ; i< 10 ;++i)
    {
        userItem * item = new userItem;
        item->slot_setInfo( i+1 , QString("用户%1").arg(i+1)
                            ,0, i+1 );
        m_mainWnd->slot_addFriend( item );
    }
    //m_mainWnd->show();
}

void CKernel::DestroyedInstance()//退出，回收m_mainWnd,m_tcpClient
{
    if(m_loginWnd)
    {
        m_loginWnd->hide();
        delete m_loginWnd;
        m_loginWnd = NULL;
    }
    if(m_mainWnd)
    {
        m_mainWnd->hide();
        delete m_mainWnd;
        m_mainWnd = NULL;
    }
    if(m_tcpClient)
    {
        m_tcpClient->CloseNet();
        delete m_tcpClient;
        m_tcpClient = NULL;
    }
    exit(0);
}

void CKernel::slot_dealData(long lSendIP, char* buf, int nlen)
{
    qDebug()<<__func__;
    //服务器发送的数据都会走到这里
    int type = *(int*)buf;
    //每一个协议头都对应一个处理函数，是映射关系 -->数组
    if(type >= _DEF_PROTOCOL_BASE && type <= _DEF_PROTOCOL_COUNT + _DEF_PROTOCOL_BASE)
    {//在协议数组范围内
        PFUN pf = m_NetProtocolMap[type-_DEF_PROTOCOL_BASE];
        if( pf )
        {//pf是成员函数指针
            (this->*pf)(buf,nlen);
        }
    }
    delete[] buf;
}


//注册回复(Net)
void CKernel::slot_registerRs(char* buf, int nlen)
{
    STRU_REGISTER_RS *rs = (STRU_REGISTER_RS*)buf;
    switch (rs->result) {
    case user_is_exist:
        Q_EMIT SIG_REGISTER_ATTENTION();
        break;
    case register_success:
        break;
    }
}

//登录回复(Net)
void CKernel::slot_loginRs(char* buf, int nlen)
{
    STRU_LOGIN_RS *rs = (STRU_LOGIN_RS*)buf;
    switch (rs->result) {
    case user_is_exist:
        Q_EMIT SIG_LOGIN_ATTENTION();
        break;
    case password_error:
        Q_EMIT SIG_LOGIN_ATTENTION();
        break;
    case login_success:
    {
        m_loginWnd->hide();
        m_mainWnd->showNormal();//show(), showNomal()
    }
        break;
    }
}

void CKernel::slot_LOGIN_RQ(QString username,QString password)
{
    //内容取出来封装成帧发送出去
    std::string username_str = username.toStdString();
    std::string password_str = password.toStdString();

    STRU_LOGIN_RQ rq;
    //strcpy(rq.username,username_str.c_str());
    strcpy(rq.password,username_str.c_str());
    //添加对于utf8到gb2312的转换
    Utf8ToGB2312(rq.username,sizeof(rq.username),username);
    m_tcpClient->SendData(0,(char*)&rq,sizeof(rq));
}

void CKernel::slot_REGISTER_RQ(QString username,QString password)
{
    //内容取出来封装成帧发送出去
    std::string username_str = username.toStdString();
    std::string password_str = password.toStdString();
    STRU_REGISTER_RQ rq;
    //strcpy(rq.username,username_str.c_str());
    Utf8ToGB2312(rq.username,sizeof(rq.username),username);
    strcpy(rq.password,username_str.c_str());
    m_tcpClient->SendData(0,(char*)&rq,sizeof(rq));
}

#include<QTextCodec>

// QString -> char* gb2312
void CKernel::Utf8ToGB2312( char* gbbuf , int nlen ,QString& utf8)
{
    //转码的对象
    QTextCodec * gb2312code = QTextCodec::codecForName( "gb2312");
    //QByteArray char 类型数组的封装类 里面有很多关于转码 和 写IO的操作
    QByteArray ba = gb2312code->fromUnicode( utf8 );// Unicode -> 转码对象的字符集

    strcpy_s ( gbbuf , nlen , ba.data() );
}

// char* gb2312 --> QString utf8
QString CKernel::GB2312ToUtf8( char* gbbuf )
{
    //转码的对象
    QTextCodec * gb2312code = QTextCodec::codecForName( "gb2312");
    //QByteArray char 类型数组的封装类 里面有很多关于转码 和 写IO的操作
    return gb2312code->toUnicode( gbbuf );// 转码对象的字符集 -> Unicode
}
