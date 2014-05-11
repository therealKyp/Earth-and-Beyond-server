///////////////////////////////////////////////////////////////////////////////////////
//// mysqlplus.h
///////////////////////////////////////////////////////////////////////////////////////

#ifndef __MYSQLPLUS_H__
#define __MYSQLPLUS_H__

//#define USE_OLD_FUNCTIONS

#include "mysql.h"
#include "vector.h"
#include <iostream>
#include <string>

///////////////////////////////////////////////////////////////////////////////////////
//// opendbstruct - database connection handle

typedef struct opendbstruct
{
   struct opendbstruct *next;    //// pointer to next member
   MYSQL mysql;                  //// MySQL connection handle
   short busy;                   //// connection busy flag
}OPENDB;


///////////////////////////////////////////////////////////////////////////////////////
//// sql_connection_c

class sql_connection_c
{
   public:
	   sql_connection_c();
   	sql_connection_c( char *database, char *host, char *user = 0, char *password = 0 );
      ~sql_connection_c();

      void connect( char *database, char *host, char *user = 0, char *password = 0 );

	   OPENDB *grabdb();
	   void freedb( OPENDB *odb );
	   short errcode();

	   unsigned int Error( OPENDB *odb );
	   char * ErrorMsg( OPENDB *odb );

   //protected:
      char *host;
      char *user;
      char *password;
      char *database;
	  int	portn;
      OPENDB *opendbbase;
      short errc;
};

///////////////////////////////////////////////////////////////////////////////////////
//// sql_query_c

class sql_result_c;
class sql_query_c
{
   public:
      sql_query_c();
      sql_query_c( sql_connection_c *sql_connection );
      sql_query_c( sql_connection_c *sql_connection, char *sql );
      ~sql_query_c();

	  void Open( sql_connection_c *sql_connection );
      int try_execute( char *sql );
      int execute( char *sql );
      sql_result_c *store();
      void free_result( sql_result_c *sql_result );
      void free_result();

	   unsigned int Error();
	   char * ErrorMsg();

      __int64 insert_id();

      MYSQL_FIELD *fetch_field();
      char *fetch_fieldname();

      int ping();

   protected:
	  MYSQL_RES *res;
	  MYSQL_ROW row;
      sql_connection_c *sql_connection;
      OPENDB *odb;
      short rowcount;

   private:
      vector_c <sql_result_c *> results;
};

///////////////////////////////////////////////////////////////////////////////////////
//// sql_var_c

class sql_var_c
{
   public:
      sql_var_c();
      sql_var_c( int i );
	  sql_var_c( long i );
      sql_var_c( double d );
	  sql_var_c( float d );
      sql_var_c( char *s );
	  sql_var_c( char s );
      sql_var_c( const char *s );
      ~sql_var_c();

      operator short ();
      operator int ();
	  operator long ();
      operator double ();
	  operator float ();
      operator char * ();
	  operator char ();
      operator const char * ();

      int operator = ( int i );
	  int operator = ( long i );
      int operator = ( double d );
	  int operator = ( float d );
	  int operator = ( char s );
      int operator = ( char *s );
      int operator = ( const char *s );

	  int Type;
	  int Size;

   protected:
      char *value;

      void release();
};

///////////////////////////////////////////////////////////////////////////////////////
//// sql_row_c

class sql_row_c
{
   public:
      sql_row_c();
	  sql_row_c( MYSQL_ROW row);
      sql_row_c( MYSQL_ROW row, MYSQL_RES *res);

      void allow_null( int allow = 1 ); //// return null values as empty strings ( "" ) if allow == 0

      sql_var_c operator [] ( int idx );
	  sql_var_c operator [] ( char *Name );

   protected:
      MYSQL_ROW row;
	  MYSQL_RES *res;
	  char Fields[100][50];
      int __allow_null;
};

///////////////////////////////////////////////////////////////////////////////////////
//// sql_field_c

class sql_field_c
{
   public:
      sql_field_c();
      sql_field_c( MYSQL_FIELD *field );

      char *get_name();
      char *get_default_value();
      enum_field_types get_type();
      unsigned int get_max_length();

   protected:
      MYSQL_FIELD *field;
};

///////////////////////////////////////////////////////////////////////////////////////
//// sql_result_c

class sql_result_c
{
   public:
      sql_result_c();
      sql_result_c( sql_query_c *query, MYSQL_RES *res );
      ~sql_result_c();

      __int64 n_rows();
      sql_row_c fetch_row();

      unsigned int n_fields();
      sql_field_c fetch_field( unsigned int field_idx );

   protected:
	  MYSQL_RES *res;
      sql_query_c *query;
};

///////////////////////////////////////////////////////////////////////////////////////
//// sql_query

class sql_query
{
   public:
      sql_query();
	  virtual ~sql_query();

      void AddData(char *Field, int Value);
	  void AddData(char *Field, long Value);
	  void AddData(char *Field, float Value);
	  void AddData(char *Field, double Value);
	  void AddData(char *Field, char Value);
	  void AddData(char *Field, char * Value);
	  void AddData(char * Field, const char * Value);
	  void SetTable(char *Table);
	  const char * CreateQuery();

   protected:
	  std::string m_Buffer;
	  std::string m_Table;
	  std::string m_Fields;
	  std::string m_Values;
	  /*
	  char * m_Fields;
	  char * m_FieldsTmp;
	  char * m_Values;
	  char * m_ValuesTmp;
	  */
};

#endif