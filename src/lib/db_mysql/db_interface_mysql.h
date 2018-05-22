/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2016 KBEngine.

KBEngine is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

KBEngine is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.
 
You should have received a copy of the GNU Lesser General Public License
along with KBEngine.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef KBE_DB_INTERFACE_MYSQL_H
#define KBE_DB_INTERFACE_MYSQL_H

#include "common.h"
#include "db_transaction.h"
#include "common/common.h"
#include "common/singleton.h"
#include "common/memorystream.h"
#include "helper/debug_helper.h"
#include "db_interface/db_interface.h"

#include "mysql/mysql.h"
#if KBE_PLATFORM == PLATFORM_WIN32
#ifdef _DEBUG
#pragma comment (lib, "libmysql_d.lib")
#pragma comment (lib, "mysqlclient_d.lib")
#else
#pragma comment (lib, "libmysql.lib")
#pragma comment (lib, "mysqlclient.lib")
#endif
#endif

namespace KBEngine { 

struct TABLE_FIELD
{
	std::string name;
	int32 length;
	uint64 maxlength;
	unsigned int flags;
	enum_field_types type;
};

/*
	���ݿ�ӿ�
*/
class DBInterfaceMysql : public DBInterface
{
public:
	DBInterfaceMysql(std::string characterSet, std::string collation);
	virtual ~DBInterfaceMysql();

	static bool initInterface(DBInterface* dbi);
	
	/**
		��ĳ�����ݿ����
	*/
	virtual bool attach(const char* databaseName = NULL);
	virtual bool detach();

	bool ping(){ 
		return mysql_ping(pMysql_) == 0; 
	}

	bool reattach();

	void inTransaction(bool value)
	{
		KBE_ASSERT(inTransaction_ != value);
		inTransaction_ = value;
	}

	bool hasLostConnection() const		{ return hasLostConnection_; }
	void hasLostConnection( bool v )	{ hasLostConnection_ = v; }

	/**
		��黷��
	*/
	virtual bool checkEnvironment();
	
	/**
		������ �Դ�������ݽ��о���
		����������ɹ�����ʧ��
	*/
	virtual bool checkErrors();

	virtual bool query(const char* strCommand, uint32 size, bool showExecInfo = true, MemoryStream * result = NULL);

	bool write_query_result(MemoryStream * result);

	/**
		��ȡ���ݿ����еı���
	*/
	virtual bool getTableNames( std::vector<std::string>& tableNames, const char * pattern);

	/**
		��ȡ���ݿ�ĳ�������е��ֶ�����
	*/
	virtual bool getTableItemNames(const char* tableName, std::vector<std::string>& itemNames);

	/** 
		�����ݿ�ɾ��entity���ֶ�
	*/
	virtual bool dropEntityTableItemFromDB(const char* tableName, const char* tableItemName);

	MYSQL* mysql(){ return pMysql_; }

	void throwError();

	my_ulonglong insertID()		{ return mysql_insert_id( pMysql_ ); }

	my_ulonglong affectedRows()	{ return mysql_affected_rows( pMysql_ ); }

	const char* info()			{ return mysql_info( pMysql_ ); }

	const char* getLastError()	
	{
		if(pMysql_ == NULL)
			return "pMysql is NULL";

		return mysql_error( pMysql_ ); 
	}

	unsigned int getLastErrorNum() { return mysql_errno( pMysql_ ); }

	typedef KBEUnordered_map<std::string, TABLE_FIELD> TABLE_FIELDS;
	void getFields(TABLE_FIELDS& outs, const char* tableName);

	/**
		��������ӿڵ�����
	*/
	virtual const char* c_str();

	/** 
		��ȡ����
	*/
	virtual const char* getstrerror();

	/** 
		��ȡ������
	*/
	virtual int getlasterror();

	/**
		������ݿⲻ�����򴴽�һ�����ݿ�
	*/
	virtual bool createDatabaseIfNotExist();
	
	/**
		����һ��entity�洢��
	*/
	virtual EntityTable* createEntityTable();

	/** 
		�����ݿ�ɾ��entity��
	*/
	virtual bool dropEntityTableFromDB(const char* tableName);

	/**
		��ס�ӿڲ���
	*/
	virtual bool lock();
	virtual bool unlock();

	/**
		�����쳣
	*/
	bool processException(std::exception & e);

	/**
		SQL�������С
	*/
	static size_t sql_max_allowed_packet(){ return sql_max_allowed_packet_; }

protected:
	MYSQL* pMysql_;

	bool hasLostConnection_;

	bool inTransaction_;

	DBTransaction lock_;

	std::string characterSet_;
	std::string collation_;

	static size_t sql_max_allowed_packet_;
};


}

#endif // KBE_DB_INTERFACE_MYSQL_H