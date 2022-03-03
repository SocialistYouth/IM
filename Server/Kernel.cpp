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
	//其他资源的回收 todo

	m_sql.DisConnect();
	
	if(  m_pServer )
		m_pServer->CloseNet();
}


void CKernel::DealData( long lSendIP , char* buf , int nlen )
{
	std::cout<< lSendIP << ":" << buf <<std::endl;
	//SendData( lSendIP , buf , nlen );

	//协议映射关系 
	int type = *(int*) buf; // 按照四个字节取 协议头

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


//处理登录请求
void CKernel::dealLoginRq( long lSendIP , char* buf , int nlen )
{
	//拆包 获取 tel password
	STRU_LOGIN_RQ * rq = (STRU_LOGIN_RQ *) buf;

	//根据tel 查 有没有这个人 
	list<string> lstRes;
	char sqlbuf[1024] = "";
	sprintf( sqlbuf, "select id , password from t_user where username = '%s';" , rq->username);
	if( !m_sql.SelectMySql(sqlbuf , 2 , lstRes ) )
	{
		cout << "select error:"<<sqlbuf <<endl;
	}
	STRU_LOGIN_RS rs;
	if( lstRes.size() == 0 )
	{//没有这个人 结果是查无此人
		rs.result = user_not_exist;
	}else {//有这个人  获取 id password
		int nID = atoi( lstRes.front().c_str() );
		lstRes.pop_front();

		string strPass = lstRes.front().c_str();
		lstRes.pop_front();

		//看 密码 是否一致 
		if( strcmp( strPass.c_str() , rq->password ) != 0 )
		{//不一致 密码错误 
			rs.result = password_error;
		}else
		{//一致 登录成功 返回id 以及结果
			rs.result = login_success;
			rs.userid = nID;
		}		
	}
	m_pServer ->SendData( lSendIP , (char*)&rs , sizeof(rs) );
}
//处理注册请求
void CKernel::dealRegisterRq( long lSendIP , char* buf , int nlen )
{
	//拆包
	STRU_REGISTER_RQ * rq = ( STRU_REGISTER_RQ * )buf;

	//拿到 tel 密码 name
	//查表 tel条件  查 有没有这个人 没有可以注册  
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
	{ //有这个人
		rs.result = user_is_exist ;
		cout<<rs.type<<endl;
	}else
	{//可以注册
		//写默认信息 -- ( 如果后面有关于传文件 , 可以先创建这个人的文件夹 )
		rs.result = register_success ;
		sprintf(  sqlbuf , "insert into t_user(username ,password ,icon , feeling) values('%s' ,'%s', 1 ,'比较懒,什么也没写')" 
			, rq->username , rq->password );
		if( !m_sql.UpdateMySql( sqlbuf ) )
		{
			cout << "UpdateMySql error: "<< sqlbuf <<endl;
		}
	}
	//返回结果 
	cout<<rs.type<<endl;
	m_pServer->SendData( lSendIP , (char*)&rs , sizeof(rs));

}