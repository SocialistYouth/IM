#include "TcpServer.h"


#include"INetMediator.h"


TcpServer::TcpServer( INetMediator * pMediator ):m_sock( INVALID_SOCKET ),m_isStop(false)
{
	m_pMediator = pMediator;
}

TcpServer::~TcpServer()  //ʹ��ʱ, ����ָ��ָ������, ʹ��������
{
	UnInitNet();
}

//��ʼ������	//���ؿ� �����׽��� ��
bool TcpServer::InitNet()
{
	//1.����С���� ը�� --���ؿ�
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

	//2.����-- �����׽��� �������������ͨ����Ҫ�Ľӿ� �����������ͨѶ�ķ�ʽ(tcp udp)
	m_sock = socket( AF_INET , SOCK_STREAM , IPPROTO_TCP ); // ipv4 udp
	if ( m_sock == INVALID_SOCKET) {
		WSACleanup();
        return false;
	}

	//3.bind
	sockaddr_in addr;
	addr.sin_family = AF_INET ;
	addr.sin_addr.S_un.S_addr = INADDR_ANY /*inet_addr("192.168.31.115")*/ ;  //����������
	addr.sin_port = htons( _DEF_TCP_PORT );  //htons ת��Ϊ�����ֽ��� ��˴洢  43232

	if( bind( m_sock ,(const sockaddr* ) &addr , sizeof(addr) ) == SOCKET_ERROR )
	{
		UnInitNet();
        return false;
	}

	//4. listen
	if( listen( m_sock , 10 ) ==  SOCKET_ERROR )
	{
		UnInitNet();
        return false;
	}

	//5. �������������߳� -- �ȴ��ͻ�������

	// ������ -- �����߳� CreateThread  WinAPI  strcpy  C/C++ RunTime �⺯�� �����ڴ��
	HANDLE hThreadHandle = (HANDLE)_beginthreadex(  NULL, 0 ,&AcceptThread ,this , 0 , NULL );     //( CreateThread �����ڴ�� )
	//_endthreadex(); -- �����ڴ��      //( ExitThread �������ڴ�� ) --�ڴ�й¶
	if( hThreadHandle )
	{
		m_hThreadHandleLst .push_back( hThreadHandle );
	}

	return true;
}

unsigned int __stdcall TcpServer::RecvThread( void * lpvoid)
{
	TcpServer* pthis = (TcpServer*) lpvoid;
	pthis->RecvData();
	
	return 0;
}

unsigned int __stdcall TcpServer::AcceptThread( void * lpvoid)
{
	//����IOģ�� �������� ֮��, ÿһ���ͻ�����Ҫһ���߳�
	TcpServer* pthis = (TcpServer*) lpvoid;
	sockaddr_in addrClient;
	int nSize = sizeof(addrClient);
	while( !pthis->m_isStop )
	{
		SOCKET sockWaiter = accept( pthis-> m_sock , (sockaddr*)&addrClient , &nSize );
		cout << "IP:"<< inet_ntoa(addrClient.sin_addr) <<" connect" <<endl;

		//ÿһ���ͻ��˶���Ӧһ���׽���
		//ÿһ���ͻ��� ���Ӧ һ���߳� һ���׽��� --> �̺߳��׽���Ҳ�Ƕ�Ӧ��
		//�����ڴ����̵߳�ʱ�� ���߳�id ���׽��� ����һ�� �γ�ӳ�� m_mapThreadIdToSocket
		unsigned int threadID = 0;
		HANDLE handle = (HANDLE)_beginthreadex( NULL , 0 , RecvThread ,pthis , 0 , &threadID);
		//m_mapThreadIdToSocket
		pthis->m_hThreadHandleLst.push_back( handle );

		pthis->m_mapThreadIdToSocket[threadID] = sockWaiter;
	}
	return 0;
}

//�ر�����
void TcpServer::UnInitNet()
{
	m_isStop = true ; // �����˳��߳�ѭ�� 

	for( auto ite = m_hThreadHandleLst.begin() ;ite != m_hThreadHandleLst.end() ; ++ite  )
	{
		if( *ite )
		{
			if( WaitForSingleObject(*ite , 100 ) == WAIT_TIMEOUT )
			{
				TerminateThread( *ite , -1 );
			}
			CloseHandle(*ite);
			*ite = NULL;
		}
	}
	
	if( m_sock && m_sock != INVALID_SOCKET )
		closesocket(m_sock);

	//ÿһ���ͻ����׽��ֵĻ��� 
	for( auto ite = m_mapThreadIdToSocket.begin() ; ite !=m_mapThreadIdToSocket.end(); ++ite )
	{
		if( ite->second && ite->second != INVALID_SOCKET )
			closesocket(ite->second);
	}

	WSACleanup();
} 


//���� : ͬʱ����tcp udp 
bool TcpServer::SendData( long lSendIP , char* buf , int nlen )
{
	if( !buf|| nlen <= 0 ) return false;

	//��ֹճ��  ����: �ȷ�����С �ٷ����ݰ�
	// m_sock  <--> lSendIP
	send( lSendIP , (char*)&nlen , sizeof(int) , 0  );

	if( send( lSendIP , buf , nlen , 0  ) <= 0 )
		return false;
	
	return true;
}
//����
void TcpServer::RecvData()
{
	//ȡ���׽��� �߳�id --> �׽��� 
	//��ȡ�̵߳�id  GetCurrentThreadId();
	Sleep(100);
	SOCKET sockWaiter = m_mapThreadIdToSocket[GetCurrentThreadId()];

	if( !sockWaiter || sockWaiter == INVALID_SOCKET  ) return;
	int nPackSize = 0; // �洢����С --> δ��Ҫ��ȡ���ֽ���
	int nRes = 0;
	while( !m_isStop )
	{
		//�����Ƚ��հ���С �ڽ������ݰ�
		nRes = recv( sockWaiter , (char*)&nPackSize , sizeof(int) , 0   ); 
		std::cout<<nPackSize<<std::endl;
		//�ӽ��ջ�������������С
		if( nRes <= 0 )
		{
			break;
		}
		int offset = 0; // ��buf ��ʼλ��ƫ�ƶ���
		char * buf = new char[nPackSize];
		while( nPackSize )
		{
			nRes = recv( sockWaiter , buf + offset , nPackSize  , 0   );
			if( nRes > 0 )
			{
				nPackSize -= nRes;
				offset += nRes;
			}
		}
		this->m_pMediator->DealData( sockWaiter , buf , offset  );  //��Ҫ������������� ���� �ռ�
	}
}