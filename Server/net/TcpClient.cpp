#include "TcpClient.h"


#include"INetMediator.h"



TcpClient::TcpClient( INetMediator * pMediator ):m_sock( INVALID_SOCKET ),m_isStop(false)
{
	m_pMediator = pMediator;
}

TcpClient::~TcpClient()  //ʹ��ʱ, ����ָ��ָ������, ʹ��������
{
	UnInitNet();
}
//��ʼ������	//���ؿ� �����׽��� ��
bool TcpClient::InitNet()
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

	//3. ���ӷ�����
	sockaddr_in addr;
	addr.sin_family = AF_INET ;
	addr.sin_addr.S_un.S_addr = inet_addr( _DEF_SERVER_IP );  /*inet_addr("192.168.31.115")*/ ;  //����������
	addr.sin_port = htons( _DEF_TCP_PORT );  //htons ת��Ϊ�����ֽ��� ��˴洢  43232

	
	if( connect( m_sock ,(const sockaddr* ) &addr , sizeof(addr) ) == SOCKET_ERROR )
	{
		UnInitNet();
        return false;
	}

	// ������ -- �����߳� CreateThread  WinAPI  strcpy  C/C++ RunTime �⺯�� �����ڴ��
	m_hThreadHandle = (HANDLE)_beginthreadex(  NULL, 0 ,&RecvThread ,this , 0 , NULL );     //( CreateThread �����ڴ�� )
	//_endthreadex(); -- �����ڴ��      //( ExitThread �������ڴ�� ) --�ڴ�й¶

	return true;
}

unsigned int __stdcall TcpClient::RecvThread( void * lpvoid)
{
	TcpClient* pthis = (TcpClient*) lpvoid;
	pthis->RecvData();
	
	return 0;
}

//�ر�����
void TcpClient::UnInitNet()
{
	m_isStop = true ; // �����˳��߳�ѭ�� 
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
//���� : ͬʱ����tcp udp 
bool TcpClient::SendData( long lSendIP , char* buf , int nlen )
{
	std::cout<<nlen<<std::endl;
	if( !buf|| nlen <= 0 ) return false;

	//��ֹճ��  ����: �ȷ�����С �ٷ����ݰ�
	// m_sock  <--> lSendIP
	send( m_sock , (char*)&nlen , sizeof(int) , 0  );
	
	if( send( m_sock , buf , nlen , 0  ) <= 0 )
		return false;
	
	return true;
}
//����

void TcpClient::RecvData()
{
	int nPackSize = 0; // �洢����С
	int nRes = 0;
	while( !m_isStop )
	{
		//�����Ƚ��հ���С �ڽ������ݰ�
		nRes = recv( m_sock , (char*)&nPackSize , sizeof(int) , 0   ); 
		//�ӽ��ջ�������������С

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
		this->m_pMediator->DealData( m_sock , buf , offset);  //��Ҫ������������� ���� �ռ�
	}
}