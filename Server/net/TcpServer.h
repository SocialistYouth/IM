#pragma once

#include"INet.h"
#include<iostream>
#include<list>
#include<map>

using namespace std;

class TcpServer : public INet
{
public:
	TcpServer(INetMediator * pMediator);
	~TcpServer(void);

	//��ʼ������
	bool InitNet(); 
	//�ر�����
	void UnInitNet(); 
	//���� : ͬʱ����tcp udp 
	bool SendData( long lSendIP , char* buf , int nlen );
	
protected:
	//�̺߳��� : ��������
	static unsigned int __stdcall RecvThread( void * lpvoid); // ��� ÿһ���ͻ�����һ��
	//�̺߳��� :���տͻ�������
	static unsigned int __stdcall AcceptThread( void * lpvoid); // 1�� 
	//����
	virtual void RecvData();

	SOCKET m_sock; // ���׽���
	//SOCKET sockWaiter; // �û��Ϳͻ��˶Ի����׽��� --> һ�� 
	map<unsigned int , SOCKET> m_mapThreadIdToSocket;
	list<HANDLE> m_hThreadHandleLst; // �����߳�

	bool m_isStop;
};

