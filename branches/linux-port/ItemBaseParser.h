// ItemBaseParser.h

#ifndef _ITEM_BASE_PARSER_H_INCLUDED_
#define _ITEM_BASE_PARSER_H_INCLUDED_

#include "ItemBase.h"

#ifdef USE_MYSQL_ITEMS
#include "mysql/mysqlplus.h"
#endif

class ItemBaseParser
{
public:
    ItemBaseParser()    {}
    ~ItemBaseParser()   {}

public:
    bool LoadItemBase(ItemBase **);

#ifdef USE_MYSQL_ITEMS
private:
	sql_result_c * SqlQuery(sql_connection_c *connection, char * QueryString);
#endif
};


#endif

