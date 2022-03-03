#include"UdpMediator.h"

#include"UdpNet.h"

UdpMediator::UdpMediator()
{
	//new �������
	m_pNet = new UdpNet(this);
}
UdpMediator::~UdpMediator()  //ʹ��ʱ, ����ָ��ָ������, ʹ��������
{
	if( m_pNet )
	{
		delete m_pNet;
		m_pNet = NULL;
	}
}
//��ʼ������
bool UdpMediator::OpenNet()
{
	if( !m_pNet->InitNet() )
		return false;

	return true;
} 
//�ر�����
void UdpMediator::CloseNet()
{
	m_pNet->UnInitNet();
} 
//���� : ͬʱ����tcp udp 
bool UdpMediator::SendData( long lSendIP , char* buf , int nlen )
{
	if( !m_pNet->SendData(lSendIP , buf , nlen ) )
		return false;

	return true;
}
//���� 
void UdpMediator::DealData( long lSendIP , char* buf , int nlen )
{
	//���⼸������ ���źŵ���ʽ���ͳ�ȥ  -- IKernel�� �Ĳۺ������� -- ע��ռ����
}