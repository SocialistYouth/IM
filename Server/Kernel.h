#pragma once



#include"CMySql.h"
#include"INetMediator.h"


#include"packdef.h"

class CKernel;

typedef void (CKernel::*PFUN)(long lSendIP , char* buf , int nlen);

class CKernel
{
public:
	CKernel(void);
	~CKernel(void);

	bool startServer();
	void closeServer();
	void DealData( long lSendIP , char* buf , int nlen );

	static CKernel * pKernel ;

	//处理登录请求
	void dealLoginRq( long lSendIP , char* buf , int nlen );
	//处理注册请求
	void dealRegisterRq( long lSendIP , char* buf , int nlen );

private:
	void setProtocolMap();

	CMySql m_sql;
	INetMediator * m_pServer;

	//协议映射表
	PFUN m_NetProtocolMap[_DEF_PROTOCOL_COUNT];
};

