#pragma once
#include"INetMediator.h"

class TcpServerMediator :
	public INetMediator
{
public:
	TcpServerMediator(void);
	~TcpServerMediator(void);

	//��ʼ������
	bool OpenNet(); 
	//�ر�����
	void CloseNet(); 
	//���� : ͬʱ����tcp udp 
	bool SendData( long lSendIP , char* buf , int nlen );
	//����
	void DealData( long lSendIP , char* buf , int nlen );
};

