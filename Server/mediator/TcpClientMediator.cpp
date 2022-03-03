#include "TcpClientMediator.h"



#include"TcpClient.h"

TcpClientMediator::TcpClientMediator()
{
	//new �������
	m_pNet = new TcpClient(this);
}
TcpClientMediator::~TcpClientMediator()  //ʹ��ʱ, ����ָ��ָ������, ʹ��������
{
	if( m_pNet )
	{
		delete m_pNet;
		m_pNet = NULL;
	}
}
//��ʼ������
bool TcpClientMediator::OpenNet()
{
	if( !m_pNet->InitNet() )
		return false;

	return true;
} 
//�ر�����
void TcpClientMediator::CloseNet()
{
	m_pNet->UnInitNet();
} 
//���� : ͬʱ����tcp udp 
bool TcpClientMediator::SendData( long lSendIP , char* buf , int nlen )
{
	if( !m_pNet->SendData(lSendIP , buf , nlen ) )
		return false;

	return true;
}
#include<iostream>
//���� 
void TcpClientMediator::DealData( long lSendIP , char* buf , int nlen )
{
	//���⼸������ ���źŵ���ʽ���ͳ�ȥ  -- IKernel�� �Ĳۺ������� -- ע��ռ����
	std::cout<< lSendIP << ":" << buf <<std::endl;
}