#pragma once


#define _DEF_UDP_PORT  (1234)
#define _DEF_TCP_PORT  (12341)
#define _DEF_SERVER_IP ("10.130.86.107")

#define _DEF_BUFFER  ( 4096 )
#define _DEF_CONTENT_SIZE	(1024)
#define _MAX_SIZE	(40)

//�Զ���Э��   ��дЭ��ͷ ��дЭ��ṹ
//��¼ ע�� ��ȡ������Ϣ ��Ӻ��� ���� ���ļ� ��������
#define _DEF_PROTOCOL_BASE	(10000)
#define _DEF_PROTOCOL_COUNT	(100)

//ע��
#define _DEF_PACK_REGISTER_RQ	(_DEF_PROTOCOL_BASE + 0 )
#define _DEF_PACK_REGISTER_RS	(_DEF_PROTOCOL_BASE + 1 )
//��¼
#define _DEF_PACK_LOGIN_RQ	(_DEF_PROTOCOL_BASE + 2 )
#define _DEF_PACK_LOGIN_RS	(_DEF_PROTOCOL_BASE + 3 )
//������Ϣ
#define _DEF_PACK_FRIEND_INFO	(_DEF_PROTOCOL_BASE + 4 )
//��Ӻ���
#define _DEF_PACK_ADDFRIEND_RQ	(_DEF_PROTOCOL_BASE + 5 )
#define _DEF_PACK_ADDFRIEND_RS	(_DEF_PROTOCOL_BASE + 6 )
//����
#define _DEF_PACK_CHAT_RQ	(_DEF_PROTOCOL_BASE + 7 )
#define _DEF_PACK_CHAT_RS	(_DEF_PROTOCOL_BASE + 8 )
//����
#define _DEF_PACK_OFFLINE_RQ	(_DEF_PROTOCOL_BASE + 9 )

//���صĽ��
//ע������Ľ��
#define user_is_exist		(0)
#define register_success	(1)
//��¼����Ľ��
#define user_not_exist		(0)
#define password_error		(1)
#define login_success		(2)
//��Ӻ��ѵĽ��
#define no_this_user		(0)
#define user_refuse			(1)
#define user_offline		(2)
#define add_success			(3)

typedef int PackType;

//Э��ṹ
//ע��
typedef struct STRU_REGISTER_RQ
{
	STRU_REGISTER_RQ():type(_DEF_PACK_REGISTER_RQ)
	{
		memset( username  , 0, sizeof(username));
		memset( password , 0, sizeof(password) );
	}
	//��Ҫ�ֻ����� , ����, �ǳ�
	PackType type;
	char username[_MAX_SIZE];
	char password[_MAX_SIZE];

}STRU_REGISTER_RQ;

typedef struct STRU_REGISTER_RS
{
	//�ظ����
	STRU_REGISTER_RS(): type(_DEF_PACK_REGISTER_RS) , result(register_success)
	{
	}
	PackType type;
	int result;

}STRU_REGISTER_RS;

//��¼
typedef struct STRU_LOGIN_RQ
{
	//��¼��Ҫ: �ֻ��� ���� 
	STRU_LOGIN_RQ():type(_DEF_PACK_LOGIN_RQ)
	{
		memset( username , 0, sizeof(username) );
		memset( password , 0, sizeof(password) );
	}
	PackType type;
	char username[_MAX_SIZE];
	char password[_MAX_SIZE];

}STRU_LOGIN_RQ;

typedef struct STRU_LOGIN_RS
{
	// ��Ҫ ��� , �û���id
	STRU_LOGIN_RS(): type(_DEF_PACK_LOGIN_RS) , result(login_success),userid(0)
	{
	}
	PackType type;
	int result;
	int userid;

}STRU_LOGIN_RS;

//������Ϣ
typedef struct STRU_FRIEND_INFO
{
	STRU_FRIEND_INFO():type(_DEF_PACK_FRIEND_INFO) , userid(0) , iconid(0) , state(0) 
	{
		memset( name ,  0 , sizeof(name) );
		memset( feeling , 0 , sizeof(feeling));
	}
	//��Ҫ �û�id ͷ��id �ǳ� ǩ�� ״̬--�Ƿ�����
	PackType type;
	int userid;
	int iconid;
	int state;
	char name[_MAX_SIZE];
	char feeling[_MAX_SIZE];

}STRU_FRIEND_INFO;

//��Ӻ���
typedef struct STRU_ADD_FRIEND_RQ
{
	// ����û�1 ����û�2 Ϊ���� ��Ҫ �û�1 id �û�1 ���� ,�û�2������
	STRU_ADD_FRIEND_RQ():type(_DEF_PACK_ADDFRIEND_RQ) , userid(0)
	{
		memset( userName  , 0 , sizeof(userName));
		memset( friendName , 0 , sizeof(friendName));
	}
	PackType type;
	int userid;
	char userName[_MAX_SIZE];
	char friendName[_MAX_SIZE];

}STRU_ADD_FRIEND_RQ;

typedef struct STRU_ADD_FRIEND_RS
{
	STRU_ADD_FRIEND_RS():type(_DEF_PACK_ADDFRIEND_RS) , userid(0), friendid(0),result(add_success)
	{
		memset( friendName  , 0 , sizeof(friendName));
	}
	//����û�1 ����û�2 ��Ҫ �û�1id  �û�2 id �û�2������ ���
	PackType type;
	int userid; // �û�1
	int friendid; // �û�2
	int result;
	char friendName[_MAX_SIZE];
	
}STRU_ADD_FRIEND_RS;

//����
typedef struct STRU_CHAT_RQ
{
	STRU_CHAT_RQ():type(_DEF_PACK_CHAT_RQ) , userid(0) , friendid(0)
	{
		memset( content , 0 , _DEF_CONTENT_SIZE );
	}
	// ˭����˭ ������ת��  �û�1 id �û�2 id ��������
	PackType type;
	int userid;
	int friendid;
	char content[_DEF_CONTENT_SIZE];

}STRU_CHAT_RQ;

typedef struct STRU_CHAT_RS
{
	STRU_CHAT_RS():type(_DEF_PACK_CHAT_RS) , userid(0) , friendid(0),result(0)
	{
	}
	PackType type;
	int userid;
	int friendid; //���������ĸ��˲�����
	int result;

}STRU_CHAT_RS;

// ���ݿ�
/*
	��1 �û��� id->Ψһ��ʶ tel  password  name  iconid   feeling Ҳ����д�����ű� ��¼���û���Ϣ
	��:  id  tel password  name  iconid  feeling

	��2 ����  idA  idB
	�� idA  idB  
	����: ���ѹ�ϵ��˫���ϵ  ��ע�ǵ����ϵ  , ��Ӻ���ʱ , ��Ҫд������ �� A->B B->A �����ѯ
	(��Ӻ��� ����һ��Ȳ����)

//	��3 ������Ϣ
	
	�û����ݱ�t_user
	create table t_user ( id bigint unsigned  AUTO_INCREMENT primary key, tel varchar (260) , 
	password varchar(260)  , name varchar (260) , icon int , feeling varchar(260));

	������Ӳ������� 
	insert into t_user ( tel,password , name , icon , feeling  ) 
	values ('12345','test', '12345' ,1,'�Ƚ���,ʲôҲûд')

	���ѱ� t_friend
	create table t_friend ( idA bigint unsigned  , idB bigint unsigned);

*/








