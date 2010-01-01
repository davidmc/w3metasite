#ifndef VECTOR_H
#define VECTOR_H

//
// Intended for holding pointers to data.
// Vector will delete held data at vector 
// destruction time.  Stack allocation
// of members added using the & operator
// is bad for this template and for the program.
//
template <class T> class Vector
{

private:

  T ** data;
  int size;
  int step;
  enum{ increment = 10 };

public:

  Vector();

  ~Vector();

  void add( T * input );

  T * operator [] ( int i );

  int max( void );

}

// Constructor constructs with zero size.
// The first add will cause the allocation of  
// an array of 10 pointers
//
template <class T> Vector<T>::Vector()
:data( (T*) 0l), size( 0 ), step(1) 
{
  ;
}

// The destructor first deletes all of the 
// elemants pointed to, then it deletes the 
// array of pointers.
//
template <class T> Vector<T>::~Vector()
{
  for( int i=0; i < size; i++)
  {
    delete data[i];
  }
  delete [] data;
}

// The add method places the pointer in the current 
// position in the array.  If the array is full
// (or null) it is (re)allocated for another 'increment'
// number of elements.
// 
template <class T> void Vector<T>::add( T * input )
{
  if( !(size%(step*increment)) )
  {
    // time to add pointers
    step++;
    T** temp = new T* [ step*increment];
    if( data )
    {
      memcpy( temp, data, sizeof(T**)*(step-1)*increment );
      delete data;
    }
    data = temp;
  }
  data[size] = input;
  size++;

}

// operator accesses the element at the (zero based)
// requested position.  It does not first check 
// the size of the array.  the 'max, method should 
// be used to determine the size fo the array.
//
template <class T> T * Vector<T>::operator [] ( int i )
{
  return data[i];
}


// returns the size of the array.  Valid elements
// to be accessed are up to [max()-1].
//
template <class T> int Vector<T>::max( void )
{
  return size;
}

#endif
