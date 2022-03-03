#pragma once

#include<winsock2.h>

//加载动态库的引入库
//#pragma comment(lib, "ws2_32.lib")
#include<ws2tcpip.h>
#include<process.h>

#include"packdef.h"


class INetMediator;
class INet
{
public:
	INet(){}
	virtual ~INet(){} //使用时, 父类指针指向子类, 使用虚析构
	//初始化网络
	virtual bool InitNet() = 0 ; 
	//关闭网络
	virtual void UnInitNet() = 0 ; 
	//发送 : 同时兼容tcp udp 
	virtual bool SendData( long lSendIP , char* buf , int nlen ) = 0;
	
protected:
	//接收
	virtual void RecvData() = 0 ;
	INetMediator * m_pMediator; //中介 , 用于将网络数据向类外传输
};
