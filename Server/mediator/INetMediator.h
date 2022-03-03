#pragma once


class INet;
class INetMediator
{
public:
	INetMediator(){}
	virtual ~INetMediator(){} //ʹ��ʱ, ����ָ��ָ������, ʹ��������
	//��ʼ������
	virtual bool OpenNet() = 0 ; 
	//�ر�����
	virtual void CloseNet() = 0 ; 
	//���� : ͬʱ����tcp udp 
	virtual bool SendData( long lSendIP , char* buf , int nlen ) = 0;
	//����
	virtual void DealData( long lSendIP , char* buf , int nlen ) = 0 ;

protected:
	INet * m_pNet; //����
};