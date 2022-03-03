#include "TcpClient.h"
#include "INetMediator.h"

TcpClient::TcpClient(INetMediator * pMediator):m_sock(INVALID_SOCKET),m_isStop(false)
{
	m_pMediator = pMediator;
}


TcpClient::~TcpClient(void)
{
	UnInitNet();
}

//初始化网络
bool TcpClient::InitNet()
{
	//1.加载库
	WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        return false;
    }

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
        WSACleanup();
        return false;
    }
	//2.创建套接字
	m_sock = socket( AF_INET , SOCK_STREAM , IPPROTO_TCP ); // ipv4 udp
	if ( m_sock == INVALID_SOCKET) {
		WSACleanup();
        return false;
	}
	//3.连接服务器
	sockaddr_in addr;
	addr.sin_family = AF_INET ;
	addr.sin_addr.S_un.S_addr = inet_addr(_DEF_SERVER_IP); //inet_addr("192.168.31.115") 绑定任意网卡
	addr.sin_port = htons( _DEF_TCP_PORT );  //htons 转换为网络字节序 大端存储  43232

	if( connect( m_sock ,(const sockaddr* ) &addr , sizeof(addr) ) == SOCKET_ERROR )
	{
		UnInitNet();
        return false;
	}

	//收数据 -- 创建线程 CreateThread WinAPI 
	m_hThreadHandle = (HANDLE)_beginthreadex(NULL,0,&RecvThread,this,0,NULL); // 控制接收线程

	return true;
}

unsigned int __stdcall TcpClient::RecvThread( void * lpvoid)
{
	TcpClient* pthis = (TcpClient*)lpvoid;
	pthis->RecvData();
	return 0;
}

//关闭网络
void TcpClient::UnInitNet()
{
	m_isStop = true;//尝试退出线程循环
	//关闭线程
	if(m_hThreadHandle)
	{
		if(WaitForSingleObject(m_hThreadHandle,100) == WAIT_TIMEOUT)
		{
			TerminateThread(m_hThreadHandle,-1);
		}
		CloseHandle(m_hThreadHandle);
		m_hThreadHandle = NULL;
	}
	//关闭套接字
	if(m_sock&&m_sock != INVALID_SOCKET)
	{
		closesocket(m_sock);
	}
	//卸载库
	WSACleanup();
}
//发送 : 同时兼容tcp udp 
bool TcpClient::SendData( long lSendIP , char* buf , int nlen )
{
	//如果发送数据没有或小于等于0，return
	if(!buf||nlen<=0)return false;
	//防止粘包策略：先发包大小，再发数据包
	//m_sock <--> lSendIP
	send(m_sock,(char*)&nlen,sizeof(int),0);
	if(send(m_sock,buf,nlen,0) <= 0)
	{
		return false;
	}
	return true;
}


void TcpClient::RecvData()
{
	int nPackSize = 0;//存储包大小
	int nRes = 0;
	while(!m_isStop)
	{
		//接受先接受包大小，再接受数据包
		nRes = recv(m_sock,(char*)&nPackSize,sizeof(int),0);
		if(nRes <= 0)
		{
			break;
		}
		int offset = 0;
		char * buf = new char[nPackSize];
		while(nPackSize)
		{
			nRes = recv(m_sock,buf+offset,nPackSize,0);
			if(nRes > 0)
			{
				nPackSize -= nRes;
				offset += nRes;
			}
		}
		this->m_pMediator->DealData( m_sock , buf , offset  );
	}
}