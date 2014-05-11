///////////////////////////////////////////////////////////////////////////////////////
//// vector.h
///////////////////////////////////////////////////////////////////////////////////////

#include <malloc.h>
#include <memory.h>

#ifndef __VECTOR_H__
#define __VECTOR_H__

template <class T> class vector_c
{
   public:
      vector_c();
      ~vector_c();

      int n_items();
      T &operator[]( int idx );
      void reset();
      void set_size( int size );
	   void del( int pos, int n_items = 1 );

      int add( T what );
      int add( T what, int pos );

	private:
		T *p;
		int n;
};

///////////////////////////////////////////////////////////////////////////////////////

template <class T> vector_c<T>::vector_c()
{
	p = 0;
	n = 0;
}

template <class T> vector_c<T>::~vector_c()
{
	reset();
}

///////////////////////////////////////////////////////////////////////////////////////

template <class T> T &vector_c<T>::operator[]( int idx )
{
	return( p[ idx ] );
}

template <class T> void vector_c<T>::reset()
{
	set_size( 0 );
}

template <class T> void vector_c<T>::set_size( int size )
{
	if ( !size )
	{

		if ( p )
		{
			free( p );
			p = 0;
		}

		n = 0;
	}
	else
	{
		T *old_p = p;
		p = ( T * ) malloc( sizeof( T ) * size );

      if ( size < n )
         n = size;

		if ( old_p )
		{
			memcpy( p, old_p, n * sizeof( T ) );
			free( old_p );
		}

		n = size;
	}
}

template <class T> int vector_c<T>::n_items()
{
	return( n );
}

///////////////////////////////////////////////////////////////////////////////////////

template <class T> int vector_c<T>::add( T what )
{
	set_size( n + 1 );

	( *this ) [ n - 1 ] = what;
	return( n - 1 );
}


template <class T> int vector_c<T>::add( T what , int pos )
{
	set_size( n + 1 );

	memmove( p + pos + 1, p + pos, sizeof( T ) * ( n_items() - pos - 1 ) );

	( *this ) [ pos ] = what;

	return pos;
}

template <class T> void vector_c<T>::del( int pos,int n_items )
{
   if ( !p )
      return;
	memcpy( p + pos, p + pos + n_items, sizeof( T ) * ( n - pos-n_items ) );
	set_size( n - n_items );
}

#endif