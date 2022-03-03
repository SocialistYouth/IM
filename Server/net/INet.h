#pragma once

#include<winsock2.h>

//���ض�̬��������
#pragma comment(lib, "ws2_32.lib")
#include<ws2tcpip.h>
#include<process.h>

#include"packdef.h"


class INetMediator;
class INet
{
public:
	INet(){}
	virtual ~INet(){} //ʹ��ʱ, ����ָ��ָ������, ʹ��������
	//��ʼ������
	virtual bool InitNet() = 0 ; 
	//�ر�����
	virtual void UnInitNet() = 0 ; 
	//���� : ͬʱ����tcp udp 
	virtual bool SendData( long lSendIP , char* buf , int nlen ) = 0;
	
protected:
	//����
	virtual void RecvData() = 0 ;
	INetMediator * m_pMediator; //�н� , ���ڽ��������������⴫��
};