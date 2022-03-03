#pragma once

#include"INetMediator.h"

class TcpClientMediator :
	public INetMediator
{
public:
	TcpClientMediator(void);
	~TcpClientMediator(void);

	//初始化网络
	bool OpenNet(); 
	//关闭网络
	void CloseNet(); 
	//发送 : 同时兼容tcp udp 
	bool SendData( long lSendIP , char* buf , int nlen );
	//接收
	void DealData( long lSendIP , char* buf , int nlen );
};

