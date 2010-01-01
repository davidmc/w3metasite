#include "sampler.hpp"

Sampler::Sampler() 
:portAddress(0) // int
,numPoints(0) // int
,numTriggerPoints(0) // int
,triggerLevel(0) // int
,timeOut(0) // int
,sampleMethod('\0') // byte
,interval(0.0) // double
{
  ;
}

Sampler::~Sampler()
{
  // goodbye portAddress
  // goodbye numPoints
  // goodbye numTriggerPoints
  // goodbye triggerLevel
  // goodbye timeOut
  // goodbye sampleMethod
  // goodbye interval
}


Sampler::Sampler( Sampler & input )
{
  setPortAddress( input.PortAddress() );
  setNumPoints( input.NumPoints() );
  setNumTriggerPoints( input.NumTriggerPoints() );
  setTriggerLevel( input.TriggerLevel() );
  setTimeOut( input.TimeOut() );
  setSampleMethod( input.SampleMethod() );
  setInterval( input.Interval() );
}


Sampler & Sampler::operator = ( Sampler & input )
{
  if( this == &input )
  {
    setPortAddress( input.PortAddress() );
    setNumPoints( input.NumPoints() );
    setNumTriggerPoints( input.NumTriggerPoints() );
    setTriggerLevel( input.TriggerLevel() );
    setTimeOut( input.TimeOut() );
    setSampleMethod( input.SampleMethod() );
    setInterval( input.Interval() );
  }
  return *this;
}

//
// Set and Get methods for Sampler::portAddress
//

int Sampler::PortAddress( void ) const
{
  return portAddress;
} 
void Sampler::setPortAddress( int input )
{
  portAddress = input;
}



//
// Set and Get methods for Sampler::numPoints
//

int Sampler::NumPoints( void ) const
{
  return numPoints;
} 
void Sampler::setNumPoints( int input )
{
  numPoints = input;
}



//
// Set and Get methods for Sampler::numTriggerPoints
//

int Sampler::NumTriggerPoints( void ) const
{
  return numTriggerPoints;
} 
void Sampler::setNumTriggerPoints( int input )
{
  numTriggerPoints = input;
}



//
// Set and Get methods for Sampler::triggerLevel
//

int Sampler::TriggerLevel( void ) const
{
  return triggerLevel;
} 
void Sampler::setTriggerLevel( int input )
{
  triggerLevel = input;
}



//
// Set and Get methods for Sampler::timeOut
//

int Sampler::TimeOut( void ) const
{
  return timeOut;
} 
void Sampler::setTimeOut( int input )
{
  timeOut = input;
}



//
// Set and Get methods for Sampler::sampleMethod
//

byte Sampler::SampleMethod( void ) const
{
  return sampleMethod;
} 
void Sampler::setSampleMethod( byte input )
{
  sampleMethod = input;
}



//
// Set and Get methods for Sampler::interval
//

double Sampler::Interval( void ) const
{
  return interval;
} 
void Sampler::setInterval( double input )
{
  interval = input;
}

 

byte * Sampler::Sample( void )
{
  ;
}

