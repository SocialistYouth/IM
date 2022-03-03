#include "TcpClientMediator.h"



#include"TcpClient.h"

TcpClientMediator::TcpClientMediator()
{
	//new 网络对象
	m_pNet = new TcpClient(this);
}
TcpClientMediator::~TcpClientMediator()  //使用时, 父类指针指向子类, 使用虚析构
{
	if( m_pNet )
	{
		delete m_pNet;
		m_pNet = NULL;
	}
}
//初始化网络
bool TcpClientMediator::OpenNet()
{
	if( !m_pNet->InitNet() )
		return false;

	return true;
} 
//关闭网络
void TcpClientMediator::CloseNet()
{
	m_pNet->UnInitNet();
} 
//发送 : 同时兼容tcp udp 
bool TcpClientMediator::SendData( long lSendIP , char* buf , int nlen )
{
	if( !m_pNet->SendData(lSendIP , buf , nlen ) )
		return false;

	return true;
}
#include<iostream>
//处理 
void TcpClientMediator::DealData( long lSendIP , char* buf , int nlen )
{
	//将这几个参数 以信号的形式发送出去  -- IKernel类 的槽函数处理 -- 注意空间回收
	std::cout<< lSendIP << ":" << buf <<std::endl;
}