#include<iostream>

using namespace std;

//#include"TcpClientMediator.h"
//#include"TcpServerMediator.h"


#include"Kernel.h"

#include<windows.h>



int main()
{
	/*INetMediator * m_server = new TcpServerMediator;
	if( !m_server->OpenNet() )
	{
		cout<<"����������ʧ��"<<endl;
	}

	INetMediator * m_client = new TcpClientMediator;
	if( !m_client->OpenNet() )
	{
		cout<<"�ͻ�������ʧ��"<<endl;
	}
	m_client->SendData(  0 , "hello server" 
		, strlen("hello server" )+1 );
	while(1)
	{
		Sleep(10);
	}*/
	CKernel kernel;
	if( kernel.startServer() )
	{
		while( 1 )
		{
			cout<< "server running"<<endl;
			Sleep(5000);
		}
	}else
	{
		cout<< "��������ʧ��"<<endl;
	}
	
	system("pause");
	return 0;
}