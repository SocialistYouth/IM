#pragma once

#include"INetMediator.h"

class TcpClientMediator :
	public INetMediator
{
public:
	TcpClientMediator(void);
	~TcpClientMediator(void);

	//��ʼ������
	bool OpenNet(); 
	//�ر�����
	void CloseNet(); 
	//���� : ͬʱ����tcp udp 
	bool SendData( long lSendIP , char* buf , int nlen );
	//����
	void DealData( long lSendIP , char* buf , int nlen );
};

