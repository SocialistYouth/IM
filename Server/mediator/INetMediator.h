#pragma once


class INet;
class INetMediator
{
public:
	INetMediator(){}
	virtual ~INetMediator(){} //使用时, 父类指针指向子类, 使用虚析构
	//初始化网络
	virtual bool OpenNet() = 0 ; 
	//关闭网络
	virtual void CloseNet() = 0 ; 
	//发送 : 同时兼容tcp udp 
	virtual bool SendData( long lSendIP , char* buf , int nlen ) = 0;
	//接收
	virtual void DealData( long lSendIP , char* buf , int nlen ) = 0 ;

protected:
	INet * m_pNet; //网络
};