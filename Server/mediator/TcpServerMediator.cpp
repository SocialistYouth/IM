#include "TcpServerMediator.h"
#include"TcpServer.h"
#include "../Kernel.h"

TcpServerMediator::TcpServerMediator()
{
	//new �������
	m_pNet = new TcpServer(this);
}
TcpServerMediator::~TcpServerMediator()  //ʹ��ʱ, ����ָ��ָ������, ʹ��������
{
	if( m_pNet )
	{
		delete m_pNet;
		m_pNet = NULL;
	}
}
//��ʼ������
bool TcpServerMediator::OpenNet()
{
	if( !m_pNet->InitNet() )
		return false;

	return true;
} 
//�ر�����
void TcpServerMediator::CloseNet()
{
	m_pNet->UnInitNet();
} 
//���� : ͬʱ����tcp udp 
bool TcpServerMediator::SendData( long lSendIP , char* buf , int nlen )
{
	if( !m_pNet->SendData(lSendIP , buf , nlen ) )
		return false;

	return true;
}

//���� 
void TcpServerMediator::DealData( long lSendIP , char* buf , int nlen )
{
	//���⼸������ ���źŵ���ʽ���ͳ�ȥ  -- IKernel�� �Ĳۺ������� -- ע��ռ����

	//std::cout<< lSendIP << ":" << buf <<std::endl;
	//SendData( lSendIP , buf , nlen );

	CKernel::pKernel->DealData( lSendIP , buf , nlen );
}