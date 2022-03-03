#include "TcpClient.h"


#include"INetMediator.h"



TcpClient::TcpClient( INetMediator * pMediator ):m_sock( INVALID_SOCKET ),m_isStop(false)
{
	m_pMediator = pMediator;
}

TcpClient::~TcpClient()  //使用时, 父类指针指向子类, 使用虚析构
{
	UnInitNet();
}
//初始化网络	//加载库 创建套接字 绑定
bool TcpClient::InitNet()
{
	//1.做个小买卖 炸串 --加载库
	WORD wVersionRequested;
    WSADATA wsaData;
    int err;

/* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        /* Tell the user that we could not find a usable */
        /* Winsock DLL.                                  */
        return false;
    }

/* Confirm that the WinSock DLL supports 2.2.*/
/* Note that if the DLL supports versions greater    */
/* than 2.2 in addition to 2.2, it will still return */
/* 2.2 in wVersion since that is the version we      */
/* requested.                                        */

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
        /* Tell the user that we could not find a usable */
        /* WinSock DLL.                                  */
        WSACleanup();
        return false;
    }

	//2.雇人-- 创建套接字 进程与外界网络通信需要的接口 决定了与外界通讯的方式(tcp udp)
	m_sock = socket( AF_INET , SOCK_STREAM , IPPROTO_TCP ); // ipv4 udp
	if ( m_sock == INVALID_SOCKET) {
		WSACleanup();
        return false;
	}

	//3. 连接服务器
	sockaddr_in addr;
	addr.sin_family = AF_INET ;
	addr.sin_addr.S_un.S_addr = inet_addr( _DEF_SERVER_IP );  /*inet_addr("192.168.31.115")*/ ;  //绑定任意网卡
	addr.sin_port = htons( _DEF_TCP_PORT );  //htons 转换为网络字节序 大端存储  43232

	
	if( connect( m_sock ,(const sockaddr* ) &addr , sizeof(addr) ) == SOCKET_ERROR )
	{
		UnInitNet();
        return false;
	}

	// 收数据 -- 创建线程 CreateThread  WinAPI  strcpy  C/C++ RunTime 库函数 创建内存块
	m_hThreadHandle = (HANDLE)_beginthreadex(  NULL, 0 ,&RecvThread ,this , 0 , NULL );     //( CreateThread 创建内存块 )
	//_endthreadex(); -- 回收内存块      //( ExitThread 不回收内存块 ) --内存泄露

	return true;
}

unsigned int __stdcall TcpClient::RecvThread( void * lpvoid)
{
	TcpClient* pthis = (TcpClient*) lpvoid;
	pthis->RecvData();
	
	return 0;
}

//关闭网络
void TcpClient::UnInitNet()
{
	m_isStop = true ; // 尝试退出线程循环 
	if( m_hThreadHandle )
	{
		if( WaitForSingleObject(m_hThreadHandle , 100 ) == WAIT_TIMEOUT )
		{
			TerminateThread( m_hThreadHandle , -1 );
		}
		CloseHandle(m_hThreadHandle);
		m_hThreadHandle = NULL;
	}
	if( m_sock && m_sock != INVALID_SOCKET )
		closesocket(m_sock);

	WSACleanup();
} 
#include<iostream>
//发送 : 同时兼容tcp udp 
bool TcpClient::SendData( long lSendIP , char* buf , int nlen )
{
	std::cout<<nlen<<std::endl;
	if( !buf|| nlen <= 0 ) return false;

	//防止粘包  策略: 先发包大小 再发数据包
	// m_sock  <--> lSendIP
	send( m_sock , (char*)&nlen , sizeof(int) , 0  );
	
	if( send( m_sock , buf , nlen , 0  ) <= 0 )
		return false;
	
	return true;
}
//接收

void TcpClient::RecvData()
{
	int nPackSize = 0; // 存储包大小
	int nRes = 0;
	while( !m_isStop )
	{
		//接收先接收包大小 在接受数据包
		nRes = recv( m_sock , (char*)&nPackSize , sizeof(int) , 0   ); 
		//从接收缓冲区拷贝包大小

		if( nRes <= 0 )
		{
			//continue;
			break;
		}
		int offset = 0;
		char * buf = new char[nPackSize];
		while( nPackSize )
		{
			nRes = recv( m_sock , buf + offset , nPackSize  , 0   );
			if( nRes > 0 )
			{
				nPackSize -= nRes;
				offset += nRes;
			}
		}
		this->m_pMediator->DealData( m_sock , buf , offset);  //需要在这个函数回收 堆区 空间
	}
}