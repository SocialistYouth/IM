#include"UdpMediator.h"

#include"UdpNet.h"

UdpMediator::UdpMediator()
{
	//new 网络对象
	m_pNet = new UdpNet(this);
}
UdpMediator::~UdpMediator()  //使用时, 父类指针指向子类, 使用虚析构
{
	if( m_pNet )
	{
		delete m_pNet;
		m_pNet = NULL;
	}
}
//初始化网络
bool UdpMediator::OpenNet()
{
	if( !m_pNet->InitNet() )
		return false;

	return true;
} 
//关闭网络
void UdpMediator::CloseNet()
{
	m_pNet->UnInitNet();
} 
//发送 : 同时兼容tcp udp 
bool UdpMediator::SendData( long lSendIP , char* buf , int nlen )
{
	if( !m_pNet->SendData(lSendIP , buf , nlen ) )
		return false;

	return true;
}
//处理 
void UdpMediator::DealData( long lSendIP , char* buf , int nlen )
{
	//将这几个参数 以信号的形式发送出去  -- IKernel类 的槽函数处理 -- 注意空间回收
}