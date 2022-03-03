#pragma once

#include"INet.h"

class UdpNet:public INet
{
public:
	UdpNet( INetMediator * pMediator );
	~UdpNet(); //ʹ��ʱ, ����ָ��ָ������, ʹ��������
	//��ʼ������
	bool InitNet(); 
	//�ر�����
	void UnInitNet(); 
	//���� : ͬʱ����tcp udp 
	bool SendData( long lSendIP , char* buf , int nlen );
	
protected:
	static unsigned int __stdcall RecvThread( void * lpvoid);

	//����
	virtual void RecvData();
	SOCKET m_sock;
	HANDLE m_hThreadHandle; // ���ƽ����߳�
	bool m_isStop;
};