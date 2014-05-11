///////////////////////////////////////////////////////////////////////////////////////
//// mysqlplus.cpp
///////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "mysqlplus.h"
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////////////
//// sql_connection_c

sql_connection_c::sql_connection_c()
{
   host = user = password = database = 0;
   opendbbase = 0;
   errc = 0;
   portn = 3306;
}

sql_connection_c::sql_connection_c( char *__database, char *__host, char *__user, char *__password )
{
   host = user = password = database = 0;
   opendbbase = 0;
   errc = 0;

   connect( __database, __host, __user, __password );
}

sql_connection_c::~sql_connection_c()
{
   if ( host )
   {
	   delete[] host;
      host = 0;
   }

   if ( user )
   {
	   delete[] user;
      user = 0;
   }

   if ( password )
   {
	   delete[] password;
      password = 0;
   }
   if ( database )
   {
	   delete[] database;
      database = 0;
   }

   OPENDB *odb;
	for ( odb = opendbbase; odb; odb = odb->next )
   {
	  if (odb && &odb->mysql)
	  {
		mysql_close( &odb->mysql );
	  }
   }

   while ( opendbbase )
   {
	   odb = opendbbase;
	   opendbbase = opendbbase->next;
	   if ( odb -> busy )
      {
		   //fprintf(stderr,"destroying Database object before Connect object(s)\n");
      }

	   delete odb;
   }
}

///////////////////////////////////////////////////////////////////////////////////////

void sql_connection_c::connect( char *__database, char *__host, char *__user, char *__password )
{
   char * hostd, * port;

   host = new char[ strlen( __host ) + 1 ];
   hostd = new char[ strlen( __host ) + 1 ];
   port = new char[ 6 ];
   strcpy( hostd, __host );
   
   if ( __user )
   {
      user = new char[ strlen( __user ) + 1 ];
      strcpy( user, __user );
   }
   if ( __password )
   {
      password = new char[ strlen( __password ) + 1 ];
      strcpy( password, __password );
   }

   database = new char[ strlen( __database ) + 1 ];
   strcpy( database, __database );

   host=strtok(hostd, ":");
   port=strtok(NULL, "");
   if (!port)
	   portn = 3306;
   else
	   portn = atoi(port);

   freedb( grabdb() ); //// open one connection
}

///////////////////////////////////////////////////////////////////////////////////////

OPENDB *sql_connection_c::grabdb()
{
   OPENDB *odb;

   // See if any databases are open
   for ( odb = opendbbase; odb; odb = odb->next )
   {
      //if ( !odb->busy )
      if(odb)
      {
         break;
      }
   }

   // if none open create a new connection
   if ( !odb )
   {
	   odb = new OPENDB;
	   if ( !mysql_init( &odb->mysql ) )
	   {
		   fprintf( stderr, "mysql_init() failed\n" );
		   errc = 1;
	   }
	   if ( !mysql_real_connect( &odb->mysql, host, user, password, database, portn, 0, 0) )
	   {
		   fprintf( stderr, "mysql_connect(%s,%d,%s,***) failed\n", host, portn, user );
		   errc = 2;
	   }
	   
	   odb->busy = 1;
	   odb->next = opendbbase;
	   opendbbase = odb;
   }
   else
   {
      odb->busy++;
   }

   return odb;
}

void sql_connection_c::freedb( OPENDB *odb )
{
   odb->busy = 0;
}

short sql_connection_c::errcode()
{
   return errc;
}

unsigned int sql_connection_c::Error( OPENDB *odb )
{
	return mysql_errno(&odb->mysql);
}

char * sql_connection_c::ErrorMsg( OPENDB *odb )
{
	return (char *) mysql_error(&odb->mysql);
}

///////////////////////////////////////////////////////////////////////////////////////
//// sql_query_c

sql_query_c::sql_query_c()
{
   sql_connection = 0;
   odb = 0;
   res = 0;
   row = 0;

   results.set_size( 0 );
}

sql_query_c::sql_query_c( sql_connection_c *__sql_connection )
{
   sql_connection = __sql_connection;
   odb = sql_connection->grabdb();
   res = 0;
   row = 0;

   results.set_size( 0 );
}

sql_query_c::sql_query_c( sql_connection_c *__sql_connection, char *sql )
{
   sql_connection = __sql_connection;
   odb = sql_connection->grabdb();
   res = 0;
   row = 0;

   try_execute( sql );

   results.set_size( 0 );
}

sql_query_c::~sql_query_c()
{
   for ( int idx = 0; idx < results.n_items(); idx++ )
   {
      delete results[ idx ];
      results[ idx ] = 0;
   }

   if ( res )
   {
	   mysql_free_result( res );
   }

   if ( odb )
   {
	   sql_connection->freedb( odb );
   }
}

void sql_query_c::Open( sql_connection_c *__sql_connection )
{
   sql_connection = __sql_connection;
   odb = sql_connection->grabdb();
   res = 0;
   row = 0;

   results.set_size( 0 );
}

///////////////////////////////////////////////////////////////////////////////////////

int sql_query_c::try_execute( char *sql )
{
   if ( sql_connection && odb && !res)
   {
	  if ( !mysql_query( &odb->mysql, sql ) )
      {
         return 1;
      }
   }

   return 0;
}

int sql_query_c::execute( char *sql )
{
   if ( sql_connection && odb && !res ) 
   {
      odb->busy++;
      if ( try_execute( sql ) )
      {
         res = mysql_store_result( &odb->mysql );
         odb->busy--;
      }
       
   }
   return (int)res;
} 

/*
int sql_query_c::execute( char *sql )
{
   if ( sql_connection && odb && !res )
   {
	   if ( try_execute( sql ) )
      {
		   res = mysql_store_result( &odb->mysql );
      }
   }

   return (int)res;
}
*/

sql_result_c *sql_query_c::store()
{
   if ( !res )
   {
      return 0;
   }

   sql_result_c *p = new sql_result_c( this, res );
   results.add( p );

   res = 0; //// prevent someone else from freeing this block

   return p;
}

void sql_query_c::free_result( sql_result_c *sql_result )
{
   for ( int idx = 0; idx < results.n_items(); idx++ )
   {
      if ( results[ idx ] == sql_result )
      {
         delete results[ idx ];
         results[ idx ] = 0;
         results.del( idx );
         break;
      }
   }
}

void sql_query_c::free_result()
{
   if ( sql_connection && odb && res )
   {
	   mysql_free_result( res );
	   res = 0;
	   row = 0;
   }
}

unsigned int sql_query_c::Error()
{
	return mysql_errno(&odb->mysql);
}

char * sql_query_c::ErrorMsg()
{
	return (char *) mysql_error(&odb->mysql);
}

///////////////////////////////////////////////////////////////////////////////////////

__int64 sql_query_c::insert_id()
{
   if ( sql_connection && odb )
   {
	   return mysql_insert_id( &odb->mysql );
   }

   return -1;
}

///////////////////////////////////////////////////////////////////////////////////////

MYSQL_FIELD *sql_query_c::fetch_field()
{
   if ( sql_connection && odb && res )
   {
      return mysql_fetch_field( res );
   }

   return 0;
}

char *sql_query_c::fetch_fieldname()
{
   MYSQL_FIELD *field = 0;

   if ( sql_connection && odb && res )
   {
      field = mysql_fetch_field( res );
   }

   return field ? field->name : "";
}

///////////////////////////////////////////////////////////////////////////////////////

int sql_query_c::ping()
{
   if ( odb )
   {
      return mysql_ping( &odb->mysql );
   }

   return 0;
}

///////////////////////////////////////////////////////////////////////////////////////
//// sql_var_c

sql_var_c::sql_var_c()
{
   value = 0;
}

sql_var_c::sql_var_c( int i )
{
   value = 0;
   *this = i;
}

sql_var_c::sql_var_c( double d )
{
   value = 0;
   *this = d;
}

sql_var_c::sql_var_c( float d )
{
   value = 0;
   *this = d;
}


sql_var_c::sql_var_c( long d )
{
   value = 0;
   *this = d;
}

sql_var_c::sql_var_c( char *s )
{
   value = 0;
   *this = s;
}

sql_var_c::sql_var_c( char s )
{
   value = 0;
   *this = s;
}

sql_var_c::sql_var_c( const char *s )
{
   value = 0;
   *this = (char *)s;
}

sql_var_c::~sql_var_c()
{
   release();
}

///////////////////////////////////////////////////////////////////////////////////////

sql_var_c::operator short ()
{
   if ( !value )
   {
      return 0;
   }

   return short(atoi( value ));
}

sql_var_c::operator int ()
{
   if ( !value )
   {
      return 0;
   }

   return atoi( value );
}

sql_var_c::operator long ()
{
   if ( !value )
   {
      return 0;
   }

   return atoi( value );
}

sql_var_c::operator double ()
{
   if ( !value )
   {
      return 0;
   }

   return atof( value );
}

sql_var_c::operator float ()
{
   if ( !value )
   {
      return 0;
   }

   float data = (float) atof( value );

   return data;
}

sql_var_c::operator char ()
{
   if ( !value )
   {
      return 0;
   }

   char data = (char) atoi( value );

   return data;
}

sql_var_c::operator char * ()
{
   return value;
}

sql_var_c::operator const char * ()
{
   return value;
}

///////////////////////////////////////////////////////////////////////////////////////

int sql_var_c::operator = ( int i )
{
   release();

   value = new char [ 11 + 1 ];
   itoa( i, value, 10 );
   return 0;
}

int sql_var_c::operator = ( long i )
{
   release();

   value = new char [ 11 + 1 ];
   itoa( i, value, 10 );
   return 0;
}

int sql_var_c::operator = ( double d )
{
   release();

   value = new char [ 32 ];
   sprintf( value, "%15.5f", d );
   return 0;
}

int sql_var_c::operator = ( float d )
{
   release();

   value = new char [ 32 ];
   sprintf( value, "%15.5f", d );
   return 0;
}

int sql_var_c::operator = ( char s )
{
   release();

   value = new char[4];
   itoa( (int) s, value, 10 );

   return 0;
}

int sql_var_c::operator = ( char *s )
{
   release();

   value = new char [ strlen( s ) + 1 ];
   strcpy( value, s );
   return 0;
}

int sql_var_c::operator = ( const char *s )
{
   *this = (char *)s;
   Size=1;
   return 0;
}

///////////////////////////////////////////////////////////////////////////////////////

void sql_var_c::release()
{
   if ( value )
   {
      delete[] value;
      value = 0;
   }
}

///////////////////////////////////////////////////////////////////////////////////////
//// sql_row_c

sql_row_c::sql_row_c()
{
   row = 0;
   res = 0;
   __allow_null = 1; //// by default, return null values
}


sql_row_c::sql_row_c( MYSQL_ROW __row)
{
   row = __row;
   __allow_null = 1; //// by default, return null values
}

sql_row_c::sql_row_c( MYSQL_ROW __row, MYSQL_RES *__res )
{
   row = __row;
   res = __res;
   __allow_null = 1; //// by default, return null values

	for(int idx=0;idx<(int)res->field_count;idx++)
	{
		strcpy(Fields[idx], mysql_fetch_field_direct( res, idx )->name);
	}	
}

///////////////////////////////////////////////////////////////////////////////////////

void sql_row_c::allow_null( int allow )
{
   __allow_null = allow;
}

///////////////////////////////////////////////////////////////////////////////////////

sql_var_c sql_row_c::operator [] ( char *Name )
{
	unsigned int idx = 0;

	for(idx=0; idx<=res->field_count; idx++)
	{
        if(idx == res->field_count)
        {
            printf("Field `%s` does not exist in this row!\n", Name);
            return sql_var_c(0);
        }

		if(!strcmpi(Fields[idx], Name))
			break;
	}

	return sql_var_c( __allow_null ? ( row[ idx ] ) : ( row[ idx ] ? row[ idx ] : "" ) );
}

sql_var_c sql_row_c::operator [] ( int idx )
{
   return sql_var_c( __allow_null ? ( row[ idx ] ) : ( row[ idx ] ? row[ idx ] : "" ) );
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
   query = 0;
   res = 0;
}

sql_result_c::sql_result_c( sql_query_c *__query, MYSQL_RES *__res )
{
   query = __query;
   res = __res;
}

sql_result_c::~sql_result_c()
{
   if ( res )
   {
	   mysql_free_result( res );
   }
}

///////////////////////////////////////////////////////////////////////////////////////

__int64 sql_result_c::n_rows()
{
   if ( query && res )
   {
      return mysql_num_rows( res );
   }

   return 0;
}

sql_row_c sql_result_c::fetch_row()
{
   MYSQL_ROW row = mysql_fetch_row( res );

   return sql_row_c( row, res );
}

///////////////////////////////////////////////////////////////////////////////////////

unsigned int sql_result_c::n_fields()
{
   if ( query && res )
   {
      return mysql_num_fields( res );
   }

   return 0;
}

sql_field_c sql_result_c::fetch_field( unsigned int field_idx )
{
   if ( query && res )
   {
      MYSQL_FIELD *field = mysql_fetch_field_direct( res, field_idx );
      return sql_field_c( field );
   }

   return sql_field_c();
}

/////////////////////////////////////////////////////////////////////////////////////////

sql_query::sql_query()
{
	m_Buffer.empty();
	m_Table.empty();
	m_Values.empty();
	m_Fields.empty();
}

sql_query::~sql_query()
{
}


void sql_query::AddData(char * Field, int Value)
{

	char m_TmpField[50];
	char m_TmpValue[50];

	if (m_Fields.length() == 0)
	{
		sprintf(m_TmpField, "`%s`", Field);
		sprintf(m_TmpValue, "'%d'", Value);
	} else {
		sprintf(m_TmpField, ", `%s`", Field);
		sprintf(m_TmpValue, ", '%d'", Value);
	}

	m_Fields.append(m_TmpField);
	m_Values.append(m_TmpValue);
}

void sql_query::AddData(char * Field, long Value)
{
	char m_TmpField[50];
	char m_TmpValue[50];

	if (m_Fields.length() == 0)
	{
		sprintf(m_TmpField, "`%s`", Field);
		sprintf(m_TmpValue, "'%d'", Value);
	} else {
		sprintf(m_TmpField, ", `%s`", Field);
		sprintf(m_TmpValue, ", '%d'", Value);
	}

	m_Fields.append(m_TmpField);
	m_Values.append(m_TmpValue);
}

void sql_query::AddData(char * Field, float Value)
{
	char m_TmpField[50];
	char m_TmpValue[50];

	if (m_Fields.length() == 0)
	{
		sprintf(m_TmpField, "`%s`", Field);
		sprintf(m_TmpValue, "'%f'", Value);
	} else {
		sprintf(m_TmpField, ", `%s`", Field);
		sprintf(m_TmpValue, ", '%f'", Value);
	}

	m_Fields.append(m_TmpField);
	m_Values.append(m_TmpValue);
}

void sql_query::AddData(char * Field, double Value)
{
	char m_TmpField[50];
	char m_TmpValue[50];

	if (m_Fields.length() == 0)
	{
		sprintf(m_TmpField, "`%s`", Field);
		sprintf(m_TmpValue, "'%f'", Value);
	} else {
		sprintf(m_TmpField, ", `%s`", Field);
		sprintf(m_TmpValue, ", '%f'", Value);
	}

	m_Fields.append(m_TmpField);
	m_Values.append(m_TmpValue);
}

void sql_query::AddData(char * Field, char Value)
{
	char m_TmpField[50];
	char m_TmpValue[2];

	if (m_Fields.length() == 0)
	{
		sprintf(m_TmpField, "`%s`", Field);
		sprintf(m_TmpValue, "'%c'", Value);
	} else {
		sprintf(m_TmpField, ", `%s`", Field);
		sprintf(m_TmpValue, ", '%c'", Value);
	}

	m_Fields.append(m_TmpField);
	m_Values.append(m_TmpValue);
}

void sql_query::AddData(char * Field, char * Value)
{
	char m_TmpField[50];
	char m_TmpValue[1000];

	if (m_Fields.length() == 0)
	{
		sprintf(m_TmpField, "`%s`", Field);
		sprintf(m_TmpValue, "'%s'", Value);
	} else {
		sprintf(m_TmpField, ", `%s`", Field);
		sprintf(m_TmpValue, ", '%s'", Value);
	}

	m_Fields.append(m_TmpField);
	m_Values.append(m_TmpValue);
}

void sql_query::AddData(char * Field, const char * Value)
{
	char m_TmpField[50];
	char m_TmpValue[1000];

	if (m_Fields.length() == 0)
	{
		sprintf(m_TmpField, "`%s`", Field);
		sprintf(m_TmpValue, "'%s'", Value);
	} else {
		sprintf(m_TmpField, ", `%s`", Field);
		sprintf(m_TmpValue, ", '%s'", Value);
	}

	m_Fields.append(m_TmpField);
	m_Values.append(m_TmpValue);
}

void sql_query::SetTable(char * Table)
{
	m_Table = Table;
}

const char * sql_query::CreateQuery()
{

	m_Buffer = "INSERT INTO `" + m_Table + "` (" + m_Fields + ") VALUES (" + m_Values + ");";

	return m_Buffer.c_str();	
}