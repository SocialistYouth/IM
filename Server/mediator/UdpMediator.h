#pragma once

#include"INetMediator.h"

class UdpMediator: public  INetMediator
{
public:
	UdpMediator();
	~UdpMediator(); //ʹ��ʱ, ����ָ��ָ������, ʹ��������
	//��ʼ������
	bool OpenNet(); 
	//�ر�����
	void CloseNet(); 
	//���� : ͬʱ����tcp udp 
	bool SendData( long lSendIP , char* buf , int nlen );
	//����
	void DealData( long lSendIP , char* buf , int nlen );
};