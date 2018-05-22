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

#ifndef KBE_ENTITY_TABLE_MYSQL_H
#define KBE_ENTITY_TABLE_MYSQL_H
#include "db_interface_mysql.h"
#include "common.h"
#include "common/common.h"
#include "common/singleton.h"
#include "helper/debug_helper.h"
#include "db_interface/entity_table.h"

namespace KBEngine { 

class EntityTableMysql;

#define MYSQL_ENGINE_TYPE "InnoDB"

/*
	ά��entity�����ݿ��еı�
*/
class EntityTableMysql : public EntityTable
{
public:
	EntityTableMysql();
	virtual ~EntityTableMysql();
	
	/**
		��ʼ��
	*/
	virtual bool initialize(std::string name);

	/**
		ͬ��entity�����ݿ���
	*/
	virtual bool syncToDB(DBInterface* dbi);

	/**
		ͬ��������
	*/
	virtual bool syncIndexToDB(DBInterface* dbi);

	/** 
		����һ����item
	*/
	virtual EntityTableItem* createItem(std::string type);

	DBID writeTable(DBInterface* dbi, DBID dbid, int8 shouldAutoLoad, MemoryStream* s);

	/**
		�����ݿ�ɾ��entity
	*/
	bool removeEntity(DBInterface* dbi, DBID dbid);

	/**
		��ȡ���е����ݷŵ�����
	*/
	virtual bool queryTable(DBInterface* dbi, DBID dbid, MemoryStream* s);

	/**
		�����Ƿ��Զ�����
	*/
	virtual void entityShouldAutoLoad(DBInterface* dbi, DBID dbid, bool shouldAutoLoad) {};

	/**
		��ѯ�Զ����ص�ʵ��
	*/
	virtual void queryAutoLoadEntities(DBInterface* dbi,
		ENTITY_ID start, ENTITY_ID end, std::vector<DBID>& outs) {};

	/**
		��ȡĳ�������е����ݷŵ�����
	*/
	void addToStream(MemoryStream* s, DBContext& context, DBID resultDBID);

	/**
		��ȡ��Ҫ�洢�ı����� �ֶ�����ת��Ϊsql�洢ʱ���ַ���ֵ
	*/
	virtual void getWriteSqlItem(DBInterface* dbi, MemoryStream* s, DBContext& context);
	virtual void getReadSqlItem(DBContext& context);

	void init_db_item_name();

protected:
	
};


}

#ifdef CODE_INLINE
#include "entity_table_mysql.inl"
#endif
#endif // KBE_ENTITY_TABLE_MYSQL_H
