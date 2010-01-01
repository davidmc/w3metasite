#include <string.h>
#include <iostream.h>
#include "daqtypes.h"
#include "parallel.h"

class Sampler
{

private:


protected:

  int          portAddress;
  int          numPoints;
  int          numTriggerPoints;
  int          triggerLevel;
  int          timeOut;
  byte         sampleMethod;
  double       interval;

public:

  Sampler();
  ~Sampler();
  
  Sampler( Sampler & input );
  Sampler & operator = ( Sampler & input );
  
  
  int PortAddress( void ) const;
  void setPortAddress( int );

  int NumPoints( void ) const;
  void setNumPoints( int );

  int NumTriggerPoints( void ) const;
  void setNumTriggerPoints( int );

  int TriggerLevel( void ) const;
  void setTriggerLevel( int );

  int TimeOut( void ) const;
  void setTimeOut( int );

  byte SampleMethod( void ) const;
  void setSampleMethod( byte input );

  double Interval( void ) const; 
  void setInterval( double input );

  byte * Sample( void );

};
