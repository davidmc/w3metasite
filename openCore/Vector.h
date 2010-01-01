/*
  Vector.h : pointerVector class template interface and implementation.

  Copyright (c) 1996 - 2001 
  
  David McCombs davidmc@newcottage.com

  Core Class Library

  freely re-usable, redistributable.

*/

#ifndef pointerVector_H
#define pointerVector_H

//
// Intended for holding pointers to data.
//

#include <iostream>

using namespace std;

template <class T> 
class pointerVector
{

private:
  int step;
  enum{ increment = 10 };

protected:
  int size;
  T ** data;

public:

  pointerVector();
  pointerVector( pointerVector<T> & input );

  virtual ~pointerVector();

  pointerVector<T> & operator = ( pointerVector<T> & input );

  void add( T * input );
  virtual void remove( int n );
  virtual void removeAll( void );
  
  T ** subArray ( int i );
 
  T * operator [] ( int i );
 
  int Size( void );

  // friend ostream & operator << <T>( ostream & s, pointerVector<T> & input );
  // friend istream & operator >> <T>( istream & s, pointerVector<T> & input );

};


//
// Version of pointerVector that does garbage collection.
//

template <class T>
class garbageCollector: public pointerVector<T>
{
public:
  virtual ~garbageCollector();
  virtual void remove( int n );
  virtual void removeAll( void );
};


// Constructor constructs with zero size.
// The first add will cause the allocation of  
// an array of 10 pointers
//
template <class T> pointerVector<T>::pointerVector()
:data( (T**) 0l), size( 0 ), step(1) 
{
  ;
}

// The destructor first deletes all of the 
// elements pointed to, then it deletes the 
// array of pointers.
//
template <class T> pointerVector<T>::~pointerVector()
{
  removeAll();
}

// The add method places the pointer in the current 
// position in the array.  If the array is full
// (or null) it is (re)allocated for another 'increment'
// number of elements.
// 
template <class T> void pointerVector<T>::add( T * input )
{
  if( !size || size < step*increment )
  {
    // time to add pointers
    step++;
    T** temp = new T* [ step*increment];
    if( data )
    {
      memcpy( temp, data, sizeof(T**)*(step-1)*increment );
      delete [] data;
    }
    data = temp;
  }
  data[size] = input;
  size++;
}

// The remove method removes an element from the array
// by copying all elements but the one to be deleted,
// then decrementing the size.
//
template <class T> void pointerVector<T>::remove( int n )
{
  if( size > n)
  {
    T* temp = (T*)0L;
    for( int i = 0, j = 0; i < size; i++ )
    {
      if( i != n )
      {
        temp = data[i];
        data[j] = temp;
        j++;
      }
    } // end for all elements

    --size;
    data[size]= (T*) 0L;
  }
}

template <class T> void pointerVector<T>::removeAll( void )
{
  delete [] data;
  data = NULL;
  size = 0;
}

// operator accesses the element at the (zero based)
// requested position.  It does not first check 
// the size of the array.  the 'Size, method should 
// be used to determine the size fo the array.
//
template <class T> T * pointerVector<T>::operator [] ( int i )
{
  return data[i];
}

template <class T> T ** pointerVector<T>::subArray( int i )

{
  return & data[i];
}


// returns the size of the array.  Valid elements
// to be accessed are up to [Size()-1].
//
template <class T> int pointerVector<T>::Size( void )
{
  return size;
}


template <class T> pointerVector<T>::pointerVector( pointerVector<T> & input )
:data( (T**) 0l), size( 0 ), step(1) 
{
  for( int i=0; i < input.Size(); i++)
  {
    add( new T ( *input.data[i] ) );
  }
}

template <class T> pointerVector<T> & pointerVector<T>::operator = ( pointerVector<T> & input )
{
  if( this != &input )
  {
    removeAll();

    for( int i=0; i < input.Size(); i++)
    {
      add( new T ( *input.data[i] ) );
    }
  }
  return *this;
}

template <class T> ostream & operator << ( ostream & s, pointerVector<T> & input )
{
  s << input.size << endl;
  for( int i=0; i < input.Size(); i++)
  {
    s << *(input.data[i]);
  }
  return s;
}

template <class T> istream & operator >> ( istream & s, pointerVector<T> & input )
{
  size_t tempSize;
  s >> tempSize >> ws;
  for( size_t i=0; i < tempSize; i++)
  {
    T * tdata = new T;
    s >> *tdata;
    input.add( tdata );
  }
  return s;
}

// The destructor first deletes all of the 
// elements pointed to, then it deletes the 
// array of pointers.
//
template <class T> garbageCollector<T>::~garbageCollector()
{
  removeAll();
}
// The remove method removes an element from the array
// by copying all elements but the one to be deleted, 
// then deleting the item, then decrementing the size.
//
template <class T> void garbageCollector<T>::remove( int n )
{
  if( pointerVector<T>::size > n)
  {
    T* temp = (T*)0L;
    for( int i = 0, j = 0; i < pointerVector<T>::size; i++ )
    {
      if( i != n )
      {
        temp = pointerVector<T>::data[i];
        pointerVector<T>::data[j] = temp;
        j++;
      }
      else
      {
        delete pointerVector<T>::data[i];
        pointerVector<T>::data[i]=NULL;
      }
    } // end for all elements

    --pointerVector<T>::size;
    pointerVector<T>::data[pointerVector<T>::size]= (T*) 0L;
  }
}

template <class T> void garbageCollector<T>::removeAll( void )
{
  for( int i=0; i<pointerVector<T>::size; i++ )
  {
    delete pointerVector<T>::data[i];
    pointerVector<T>::data[i]=NULL;
  }
  delete [] pointerVector<T>::data;
  pointerVector<T>::data = NULL;
  pointerVector<T>::size = 0;
}

#endif

