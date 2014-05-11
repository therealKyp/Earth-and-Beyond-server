///////////////////////////////////////////////////////////////////////////////////////
//// mysqlplus.cpp
///////////////////////////////////////////////////////////////////////////////////////

#include "mysqlplus.h"
#include "net7.h"
#include <stdio.h>
#include <assert.h>

#define _CRT_SECURE_NO_WARNINGS 1		// Disable Warning messages about new Secure Functions in VS2008
#pragma warning(disable:4996)

///////////////////////////////////////////////////////////////////////////////////////
//// sql_connection_c

sql_connection_c::sql_connection_c()
{
    host = 0;
    user = 0;
    password = 0;
    database = 0;
    opendbbase = 0;
    portn = 3306;
}

sql_connection_c::sql_connection_c(char *__database, char *__host, char *__user, char *__password)
{
    host = 0;
    user = 0;
    password = 0;
    database = 0;
    opendbbase = 0;

    connect(__database, __host, __user, __password);
}

sql_connection_c::~sql_connection_c()
{
    if (host)
    {
        delete [] host;
    }

    if (user)
    {
        delete [] user;
    }

    if (password)
    {
        delete [] password;
    }

    if (database)
    {
        delete [] database;
    }

    disconnect();
}

///////////////////////////////////////////////////////////////////////////////////////

bool sql_connection_c::connected()
{
	OPENDB *odb = grabdb();

	if (!odb)
	{
		return false;
	}

	int ping_result = mysql_ping(&odb->mysql);

	if (ping_result)
	{
		printf("sql_connection_c::connected() - mysql_ping() failed\n");
	}

	freedb(odb);

	return ping_result ? false : true;
}

void sql_connection_c::connect(char *__database, char *__host, char *__user, char *__password)
{
    disconnect();

    if (!__database || !__host)
    {
        printf("sql_connection_c::connect - Parameter(s) are null\n");
        return;
    }

    // Assume the default port
    portn = 3306;

    char * host_buff = 0;
    char * host_tmp = 0;
    char * port_tmp = 0;

    if (host)
    {
        delete [] host;
    }

    if (database)
    {
        delete [] database;
    }

    if (user)
    {
        delete [] user;
        user = 0;
    }

    if (password)
    {
        delete [] password;
        password = 0;
    }

    // Seperate the host from the port
    host_buff = new char[strlen(__host) + 1];
    strcpy(host_buff, __host);

    host_tmp = strtok(host_buff, ":");
    port_tmp = strtok(NULL, "");

    if (!host_tmp)
    {
        printf("sql_connection_c::connect - Invalid host: %s\n",__host);
        return;
    }

    if (port_tmp)
    {
        portn = atoi(port_tmp);
    }

    host = new char[strlen(host_tmp) + 1];
    strcpy(host, host_tmp);

    if (host_buff)
    {
        delete [] host_buff;
    }


    database = new char[strlen(__database) + 1];
    strcpy(database, __database);

    if (__user)
    {
        user = new char[strlen(__user) + 1];
        strcpy(user, __user);
    }

    if (__password)
    {
        password = new char[strlen(__password) + 1];
        strcpy(password, __password);
    }

    // Open a connection
    freedb(grabdb());
}

void sql_connection_c::disconnect()
{
    m_Mutex.Lock();

    OPENDB *odb;
    for (odb = opendbbase; odb; odb = odb->next)
    {
        if (odb && &odb->mysql)
        {
            mysql_close(&odb->mysql);
        }
    }

    while (opendbbase)
    {
        odb = opendbbase;
        opendbbase = opendbbase->next;

        if (odb->busy)
        {
	       //fprintf(stderr,"destroying Database object before Connect object(s)\n");
        }

        delete odb;
    }

    m_Mutex.Unlock();
}

///////////////////////////////////////////////////////////////////////////////////////

OPENDB *sql_connection_c::grabdb()
{
    OPENDB * last_db = 0;
    OPENDB * open_db = 0;

    m_Mutex.Lock();

    // See if any databases are open and not being used
    for (OPENDB * db = opendbbase; db != 0; db = db->next)
    {
        last_db = db;

        if (db && !db->busy)
        {
            open_db = db;
            break;
        }
    }

    // If no open databases are found, create a new one
    if (open_db == 0)
    {
        open_db = new OPENDB;

        if (!mysql_init(&open_db->mysql))
        {
            printf("sql_connection_c::grabdb - mysql_init() failed\n");
            delete open_db;

            m_Mutex.Unlock();
            return 0;
        }

        //Set this db to auto-reconnect
        bool reconnect = true;
        if (mysql_options(&open_db->mysql, MYSQL_OPT_RECONNECT, &reconnect))
        {
            printf("sql_connection_c::grabdb - mysql_options() failed\n");
            delete open_db;

            m_Mutex.Unlock();
            return 0;
        }

        if (!mysql_real_connect(&open_db->mysql, host, user, password, database, portn, 0, 0))
        {
            printf("sql_connection_c::grabdb - mysql_connect() failed\n");
            delete open_db;

            m_Mutex.Unlock();
            return 0;
        }

        open_db->busy = true;
        open_db->next = 0;

        // Attach it to the list
        if (last_db == 0)
        {
            opendbbase = open_db;
        }
        else
        {
            last_db->next = open_db;
        }
    }
    else
    {
        open_db->busy = true;
    }

    m_Mutex.Unlock();
    return open_db;
}

void sql_connection_c::freedb(OPENDB *odb)
{
    if (odb)
    {
        m_Mutex.Lock();
        odb->busy = false;
        m_Mutex.Unlock();
    }
}

///////////////////////////////////////////////////////////////////////////////////////
//// sql_query_c

sql_query_c::sql_query_c(sql_connection_c *__sql_connection)
{
    sql_connection = __sql_connection;
    odb = sql_connection->grabdb();
    res = 0;
}

sql_query_c::~sql_query_c()
{
    if (res)
    {
        mysql_free_result(res);
    }

    if (odb)
    {
        sql_connection->freedb(odb);
    }
}

///////////////////////////////////////////////////////////////////////////////////////

int sql_query_c::execute(char *sql)
{
    free_result();

    if (odb && mysql_query(&odb->mysql, sql) == 0)
    {
        res = mysql_store_result(&odb->mysql);
    }       

    return (int)res;
} 

bool sql_query_c::run_query(char *sql)
{
    if (execute(sql) == 0 && Error() > 0)
    {
        LogMySQLMsg("Error executing query:\n\n%s\n\n", sql);
        LogMySQLMsg("Error #%d: %s\n\n", Error(), ErrorMsg());
        return false;
    }

    return true;
}

my_ulonglong sql_query_c::n_rows()
{
    return odb ? mysql_affected_rows(&odb->mysql) : 0;
}

void sql_query_c::store(sql_result_c *result)
{
    if (!res)
    {
        return;
    }

    result->set_res(res);

    res = 0;
}

void sql_query_c::free_result()
{
   if (res)
   {
	   mysql_free_result(res);
	   res = 0;
   }
}

unsigned int sql_query_c::Error()
{
    if (!odb)
    {
        printf("sql_query_c::Error - odb is NULL\n");
        return 1000;
    }

    return mysql_errno(&odb->mysql);
}

char * sql_query_c::ErrorMsg()
{
    if (!odb)
    {
        printf("sql_query_c::ErrorMsg - odb is NULL\n");
        return 0;
    }

    return (char *) mysql_error(&odb->mysql);
}

///////////////////////////////////////////////////////////////////////////////////////
//// sql_var_c

sql_var_c::sql_var_c()
{
   value = 0;
}

sql_var_c::sql_var_c(char *s)
{
   value = s;
}

sql_var_c::~sql_var_c()
{
}

///////////////////////////////////////////////////////////////////////////////////////

sql_var_c::operator short ()
{
    return value ? (short)atoi(value) : 0;
}

sql_var_c::operator int ()
{
    return value ? atoi(value) : 0;
}

sql_var_c::operator long ()
{
    return value ? (long)atoi(value) : 0;
}

sql_var_c::operator unsigned long ()
{
    return value ? (unsigned long)atoi(value) : 0;
}

sql_var_c::operator double ()
{
    return value ? atof(value) : 0;
}

sql_var_c::operator float ()
{
    return value ? (float)atof(value) : 0;
}

//This no longer returns an ascii character, but rather a number
sql_var_c::operator char ()
{
    return value ? (char)atoi(value) : 0;
}

sql_var_c::operator char * ()
{
   return value;
}

sql_var_c::operator const char * ()
{
   return (const char *)value;
}

///////////////////////////////////////////////////////////////////////////////////////
//// sql_row_c

sql_row_c::sql_row_c()
{
    row = 0;
    result = 0;
    field_count = 0;
    __allow_null = 1;
}

void sql_row_c::init(MYSQL_ROW __row, sql_result_c *__result)
{
    row = __row;
    result = __result;
    field_count = (int)result->n_fields();
    __allow_null = 1;
}

void sql_row_c::allow_null( int allow )
{
   __allow_null = allow;
}

///////////////////////////////////////////////////////////////////////////////////////

sql_var_c sql_row_c::operator [] (char * name)
{
	char compound[100];

    if (!result)
    {
        return sql_var_c();
    }

	for (int i=0; i<field_count; i++)
	{
		if (strcmp(result->field(i), name) == 0)
        {
	        return (*this)[i];
        }
		// see if this is a compound name
		sprintf(compound, "%s.%s", result->table(i), result->field(i));
		if (strcmp(compound, name) == 0)
        {
	        return (*this)[i];
        }
	}

    printf("Field `%s` does not exist in this table '%s'\n", name, result->table(0));
    return sql_var_c();
}

sql_var_c sql_row_c::operator [] (int idx)
{
    return sql_var_c(__allow_null ? row[idx] : (row[idx] ? row[idx] : ""));
}

///////////////////////////////////////////////////////////////////////////////////////
//// sql_field_c

sql_field_c::sql_field_c()
{
   field = 0;
}

sql_field_c::sql_field_c( MYSQL_FIELD *__field )
{
   field = __field;
}

///////////////////////////////////////////////////////////////////////////////////////

char *sql_field_c::get_name()
{
   return field->name;
}

char *sql_field_c::get_default_value()
{
   return field->def;
}

enum_field_types sql_field_c::get_type()
{
   return field->type;
}

unsigned int sql_field_c::get_max_length()
{
   return field->max_length;
}

///////////////////////////////////////////////////////////////////////////////////////
//// sql_result_c

sql_result_c::sql_result_c()
{
    res = 0;
}

sql_result_c::~sql_result_c()
{
    if (res)
    {
        mysql_free_result(res);
    }
}

///////////////////////////////////////////////////////////////////////////////////////

void sql_result_c::set_res(MYSQL_RES *__res)
{
    if (res)
    {
        mysql_free_result(res);
    }
    
    res = __res;
}

my_ulonglong sql_result_c::n_rows()
{
    return res ? mysql_num_rows(res) : 0;
}

void sql_result_c::fetch_row(sql_row_c *row)
{
    if (res && row)
    {
        row->init(mysql_fetch_row(res), this);
    }
}

///////////////////////////////////////////////////////////////////////////////////////

unsigned int sql_result_c::n_fields()
{
    return res ? mysql_num_fields(res) : 0;
}

sql_field_c sql_result_c::fetch_field(unsigned int index)
{
    return res ? sql_field_c(mysql_fetch_field_direct(res, index)) : sql_field_c();
}

char * sql_result_c::field(int index)
{
	return mysql_fetch_field_direct(res, index)->name;
}

char * sql_result_c::table(int index)
{
	return mysql_fetch_field_direct(res, index)->table;
}

/////////////////////////////////////////////////////////////////////////////////////////

sql_query::sql_query()
{
    Clear();
}

void sql_query::Clear()
{
	m_Table[0] = 0;
	m_Buffer[0] = 0;
	m_Fields[0] = 0;
	m_Values[0] = 0;
}

void sql_query::AddData(char * Field, int Value)
{
	sprintf(m_Values, "%s, '%d'", m_Values, Value);
	AddField(Field);
}

void sql_query::AddData(char * Field, unsigned long Value)
{
	sprintf(m_Values, "%s, '%d'", m_Values, Value);
	AddField(Field);
}

void sql_query::AddData(char * Field, long Value)
{
	sprintf(m_Values, "%s, '%d'", m_Values, Value);
	AddField(Field);
}

void sql_query::AddData(char * Field, float Value)
{
	sprintf(m_Values, "%s, '%f'", m_Values, Value);
	AddField(Field);
}

void sql_query::AddData(char * Field, double Value)
{
	sprintf(m_Values, "%s, '%f'", m_Values, Value);
	AddField(Field);
}

//The char is now interpreted as a numerical value
void sql_query::AddData(char * Field, char Value)
{
	sprintf(m_Values, "%s, '%d'", m_Values, (int)Value);
	AddField(Field);
}

void sql_query::AddData(char * Field, char * Value)
{
    char tmp_buff[1024];
    assert(strlen(Value) < sizeof(tmp_buff));
    mysql_escape_string(tmp_buff, Value, strlen(Value));

	sprintf(m_Values, "%s, '%s'", m_Values, tmp_buff);
	AddField(Field);
}

void sql_query::AddField(char * Field)
{
	sprintf(m_Fields, "%s, `%s`", m_Fields, Field);
    assert(strlen(m_Fields) < sizeof(m_Fields));
    assert(strlen(m_Values) < sizeof(m_Values));
}

void sql_query::SetTable(char * Table)
{
	assert(strlen(Table) < sizeof(m_Table));
	strcpy(m_Table, Table);
}

char * sql_query::CreateQuery()
{
    static char InsertSQL[] = "INSERT INTO `%s` (%s) VALUES (%s)";

	assert(strlen(InsertSQL) + strlen(m_Fields) + strlen(m_Values) + strlen(m_Table) < sizeof(m_Buffer));
	sprintf(m_Buffer, InsertSQL, m_Table, &m_Fields[1], &m_Values[1]);

	return m_Buffer;
}