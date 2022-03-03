#include"UdpNet.h"

#include"INetMediator.h"



UdpNet::UdpNet( INetMediator * pMediator ):m_sock( INVALID_SOCKET ),m_isStop(false)
{
	m_pMediator = pMediator;
}
UdpNet::~UdpNet()  //ʹ��ʱ, ����ָ��ָ������, ʹ��������
{
	UnInitNet();
}
//��ʼ������	//���ؿ� �����׽��� ��
bool UdpNet::InitNet()
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
	m_sock = socket( AF_INET , SOCK_DGRAM , IPPROTO_UDP ); // ipv4 udp
	if ( m_sock == INVALID_SOCKET) {
		WSACleanup();
        return false;
	}

	//�������޹㲥Ȩ��
	BOOL bval = TRUE;
	setsockopt( m_sock, SOL_SOCKET/*��һ��*/ , /*ʲô����*/ SO_BROADCAST , 
		/*���õ�ֵ ���ú�ȡ��*/(const char*)&bval , sizeof(bval) );

	//3.�ҵ�-- ��ip��ַ 
	sockaddr_in addr;
	addr.sin_family = AF_INET ;
	addr.sin_addr.S_un.S_addr = INADDR_ANY /*inet_addr("192.168.31.115")*/ ;  //����������
	addr.sin_port = htons( _DEF_UDP_PORT );  //htons ת��Ϊ�����ֽ��� ��˴洢  43232

	if( bind( m_sock ,(const sockaddr* ) &addr , sizeof(addr) ) == SOCKET_ERROR )
	{
		UnInitNet();
        return false;
	}

	// ������ -- �����߳� CreateThread  WinAPI  strcpy  C/C++ RunTime �⺯�� �����ڴ��
	m_hThreadHandle = (HANDLE)_beginthreadex(  NULL, 0 ,&RecvThread ,this , 0 , NULL );     //( CreateThread �����ڴ�� )
	//_endthreadex(); -- �����ڴ��      //( ExitThread �������ڴ�� ) --�ڴ�й¶

	return true;
}

unsigned int __stdcall UdpNet::RecvThread( void * lpvoid)
{
	UdpNet* pthis = (UdpNet*) lpvoid;
	pthis->RecvData();
	
	return 0;
}

//�ر�����
void UdpNet::UnInitNet()
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
//���� : ͬʱ����tcp udp 
bool UdpNet::SendData( long lSendIP , char* buf , int nlen )
{
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons( _DEF_UDP_PORT);
	addr.sin_addr.S_un.S_addr = lSendIP;

	if( sendto( m_sock , buf , nlen , 0 , (const sockaddr * )&addr ,sizeof(addr) ) <= 0 ) // Ŀ��
		return false;
	
	return true;
}
//����

void UdpNet::RecvData()
{
	char buf[ _DEF_BUFFER ] = "";
	SOCKADDR_IN addrClient;
	int nSize = sizeof(addrClient);
	m_isStop = false;
	while( !m_isStop )
	{
		int nRes = recvfrom( m_sock , buf , sizeof(buf) , 0 , (sockaddr*)&addrClient , &nSize );
		
		if( nRes > 0 )
		{    //���� --�н鴦��
			char * packbuf = new char[nRes];
			if( m_pMediator ){
				m_pMediator->DealData( addrClient.sin_addr.S_un.S_addr , packbuf , nRes );
				//   Ҫ��������ռ� delete[] packbuf;
			}
		}	
	}
}