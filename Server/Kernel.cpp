#include "Kernel.h"

#include"TcpServerMediator.h"
#include<iostream>

#define NetPackFunMap( a )  m_NetProtocolMap[ a - _DEF_PROTOCOL_BASE ]
void CKernel::setProtocolMap()
{
	memset( m_NetProtocolMap  , 0 , sizeof(m_NetProtocolMap) );

	NetPackFunMap( _DEF_PACK_LOGIN_RQ ) = &CKernel::dealLoginRq;
	NetPackFunMap( _DEF_PACK_REGISTER_RQ ) = &CKernel::dealRegisterRq;
}


CKernel * CKernel::pKernel = NULL;
CKernel::CKernel(void)
{
	pKernel = this;
	setProtocolMap();
}


CKernel::~CKernel(void)
{
	closeServer();
}

bool CKernel::startServer()
{
	//ip user pass db port
	if( !m_sql.ConnectMySql( "127.0.0.1", "root" , "becauseofyou0926" , "mychat" ) )
		return false;

	m_pServer = new TcpServerMediator; 

	if(  !m_pServer -> OpenNet() )
		return false;

	return true;
}

void CKernel::closeServer()
{
	//������Դ�Ļ��� todo

	m_sql.DisConnect();
	
	if(  m_pServer )
		m_pServer->CloseNet();
}


void CKernel::DealData( long lSendIP , char* buf , int nlen )
{
	std::cout<< lSendIP << ":" << buf <<std::endl;
	//SendData( lSendIP , buf , nlen );

	//Э��ӳ���ϵ 
	int type = *(int*) buf; // �����ĸ��ֽ�ȡ Э��ͷ

	if( type >= _DEF_PROTOCOL_BASE && type < _DEF_PROTOCOL_BASE + _DEF_PROTOCOL_COUNT)
	{
		PFUN pf = NetPackFunMap(type);
		if( pf )
		{
			(this->*pf)( lSendIP , buf , nlen );
		}
	}
	delete[] buf;
} 


//�����¼����
void CKernel::dealLoginRq( long lSendIP , char* buf , int nlen )
{
	//��� ��ȡ tel password
	STRU_LOGIN_RQ * rq = (STRU_LOGIN_RQ *) buf;

	//����tel �� ��û������� 
	list<string> lstRes;
	char sqlbuf[1024] = "";
	sprintf( sqlbuf, "select id , password from t_user where username = '%s';" , rq->username);
	if( !m_sql.SelectMySql(sqlbuf , 2 , lstRes ) )
	{
		cout << "select error:"<<sqlbuf <<endl;
	}
	STRU_LOGIN_RS rs;
	if( lstRes.size() == 0 )
	{//û������� ����ǲ��޴���
		rs.result = user_not_exist;
	}else {//�������  ��ȡ id password
		int nID = atoi( lstRes.front().c_str() );
		lstRes.pop_front();

		string strPass = lstRes.front().c_str();
		lstRes.pop_front();

		//�� ���� �Ƿ�һ�� 
		if( strcmp( strPass.c_str() , rq->password ) != 0 )
		{//��һ�� ������� 
			rs.result = password_error;
		}else
		{//һ�� ��¼�ɹ� ����id �Լ����
			rs.result = login_success;
			rs.userid = nID;
		}		
	}
	m_pServer ->SendData( lSendIP , (char*)&rs , sizeof(rs) );
}
//����ע������
void CKernel::dealRegisterRq( long lSendIP , char* buf , int nlen )
{
	//���
	STRU_REGISTER_RQ * rq = ( STRU_REGISTER_RQ * )buf;

	//�õ� tel ���� name
	//��� tel����  �� ��û������� û�п���ע��  
	list<string> resLst;
	char sqlbuf[1024] ="";
	sprintf(  sqlbuf , "select username from t_user where username = '%s';" , rq->username );
	if( !m_sql.SelectMySql( sqlbuf , 1 , resLst ) )
	{
		cout << "SelectMySql error: "<<sqlbuf <<endl;
		return;
	}
	STRU_REGISTER_RS rs;
	
	if( resLst.size() > 0  )
	{ //�������
		rs.result = user_is_exist ;
		cout<<rs.type<<endl;
	}else
	{//����ע��
		//дĬ����Ϣ -- ( ��������й��ڴ��ļ� , �����ȴ�������˵��ļ��� )
		rs.result = register_success ;
		sprintf(  sqlbuf , "insert into t_user(username ,password ,icon , feeling) values('%s' ,'%s', 1 ,'�Ƚ���,ʲôҲûд')" 
			, rq->username , rq->password );
		if( !m_sql.UpdateMySql( sqlbuf ) )
		{
			cout << "UpdateMySql error: "<< sqlbuf <<endl;
		}
	}
	//���ؽ�� 
	cout<<rs.type<<endl;
	m_pServer->SendData( lSendIP , (char*)&rs , sizeof(rs));

}