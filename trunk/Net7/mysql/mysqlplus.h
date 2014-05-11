///////////////////////////////////////////////////////////////////////////////////////
//// mysqlplus.h
///////////////////////////////////////////////////////////////////////////////////////

#ifndef __MYSQLPLUS_H__
#define __MYSQLPLUS_H__

#include "mysql.h"
#include "../Mutex.h"

#define _CRT_SECURE_NO_WARNINGS 1		// Disable Warning messages about new Secure Functions in VS2008

///////////////////////////////////////////////////////////////////////////////////////
//// opendbstruct - database connection handle

typedef struct opendbstruct
{
   struct opendbstruct *next;
   MYSQL mysql;
   bool busy;
} OPENDB;

///////////////////////////////////////////////////////////////////////////////////////
//// sql_connection_c

class sql_connection_c
{
public:
    sql_connection_c();
    sql_connection_c(char *database, char *host, char *user = 0, char *password = 0);
    ~sql_connection_c();

    bool connected();
    void connect(char *database, char *host, char *user = 0, char *password = 0);
    void disconnect();

    OPENDB *grabdb();
    void freedb(OPENDB *odb);

private:
    char *host;
    char *user;
    char *password;
    char *database;
    int	portn;
    OPENDB *opendbbase;

    Mutex m_Mutex;
};

///////////////////////////////////////////////////////////////////////////////////////
//// sql_query_c

class sql_result_c;

class sql_query_c
{
public:
    sql_query_c(sql_connection_c *sql_connection);
    ~sql_query_c();

    int execute(char *sql);
    bool run_query(char *sql);

    void store(sql_result_c *result);

    unsigned int Error();
    char * ErrorMsg();

    my_ulonglong n_rows();

private:
    void free_result();

private:
    MYSQL_RES *res;
    sql_connection_c *sql_connection;
    OPENDB *odb;
};

///////////////////////////////////////////////////////////////////////////////////////
//// sql_var_c

class sql_var_c
{
public:
    sql_var_c();
    sql_var_c(char *s);
    ~sql_var_c();

    operator short ();
    operator int ();
    operator long ();
    operator unsigned long ();
    operator double ();
    operator float ();
    operator const char * ();
    operator char * ();
    operator char ();

private:
    char *value;
};


///////////////////////////////////////////////////////////////////////////////////////
//// sql_field_c

class sql_field_c
{
public:
    sql_field_c();
    sql_field_c(MYSQL_FIELD *field);

    char *get_name();
    char *get_default_value();
    enum_field_types get_type();
    unsigned int get_max_length();

private:
    MYSQL_FIELD *field;
};

///////////////////////////////////////////////////////////////////////////////////////
//// sql_result_c
class sql_row_c;

class sql_result_c
{
public:
    sql_result_c();
    ~sql_result_c();

    void set_res(MYSQL_RES *res);

    my_ulonglong n_rows();
    void fetch_row(sql_row_c *row);

    unsigned int n_fields();
    sql_field_c fetch_field(unsigned int index);

    char * field(int index);		// return field name
	char * table(int index);		// return table name

private:
    MYSQL_RES *res;
};

///////////////////////////////////////////////////////////////////////////////////////
//// sql_row_c

class sql_row_c
{
public:
    sql_row_c();

    void init(MYSQL_ROW row, sql_result_c *result);

    // Null values are returned as emprty strings if allow = 0
    void allow_null(int allow = 1);

    sql_var_c operator [] (int idx);
    sql_var_c operator [] (char *name); //USE SPARINGLY

private:
    MYSQL_ROW row;
    sql_result_c *result;
    int __allow_null;
    int field_count;
};

///////////////////////////////////////////////////////////////////////////////////////
//// sql_query

class sql_query
{
public:
    sql_query();

    void AddData(char *Field, int Value);
    void AddData(char *Field, unsigned long Value);
    void AddData(char *Field, long Value);
    void AddData(char *Field, float Value);
    void AddData(char *Field, double Value);
    void AddData(char *Field, char Value);
    void AddData(char *Field, char * Value);
    void SetTable(char *Table);
    char * CreateQuery();
    void Clear();

private:
    void AddField(char *Field);

private:
    char m_Table[64];
    char m_Fields[2048];
    char m_Values[6144];
    char m_Buffer[8192];
};

#endif