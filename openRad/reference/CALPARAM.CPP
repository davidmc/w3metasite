#include "CalParam.hpp"

CalibatedParameters::CalibatedParameters() 
:openCircuitSourceNegative(0.0) // double
,openCircuitSourcePositive(0.0) // double
,negativeSourceResistance(0.0) // double
,positiveSourceResistance(0.0) // double
,zeroSystemOffset(0.0) // double
{
  ;
}

CalibatedParameters::~CalibatedParameters()
{
  // ended openCircuitSourceNegative
  // ended openCircuitSourcePositive
  // ended negativeSourceResistance
  // ended positiveSourceResistance
  // ended zeroSystemOffset
}


CalibatedParameters::CalibatedParameters( CalibatedParameters & input )
{
  setOpenCircuitSourceNegative( input.OpenCircuitSourceNegative() );
  setOpenCircuitSourcePositive( input.OpenCircuitSourcePositive() );
  setNegativeSourceResistance( input.NegativeSourceResistance() );
  setPositiveSourceResistance( input.PositiveSourceResistance() );
  setZeroSystemOffset( input.ZeroSystemOffset() );
}


CalibatedParameters & CalibatedParameters::operator = ( CalibatedParameters & input )
{
  if( this != &input )
  {
    setOpenCircuitSourceNegative( input.OpenCircuitSourceNegative() );
    setOpenCircuitSourcePositive( input.OpenCircuitSourcePositive() );
    setNegativeSourceResistance( input.NegativeSourceResistance() );
    setPositiveSourceResistance( input.PositiveSourceResistance() );
    setZeroSystemOffset( input.ZeroSystemOffset() );
  }
  return *this;
}

//
// Set and Get methods for CalibatedParameters::openCircuitSourceNegative
//

double CalibatedParameters::OpenCircuitSourceNegative( void ) const
{
  return openCircuitSourceNegative;
} 
void CalibatedParameters::setOpenCircuitSourceNegative( double input )
{
  openCircuitSourceNegative = input;
}

 

//
// Set and Get methods for CalibatedParameters::openCircuitSourcePositive
//

double CalibatedParameters::OpenCircuitSourcePositive( void ) const
{
  return openCircuitSourcePositive;
} 
void CalibatedParameters::setOpenCircuitSourcePositive( double input )
{
  openCircuitSourcePositive = input;
}

 

//
// Set and Get methods for CalibatedParameters::negativeSourceResistance
//

double CalibatedParameters::NegativeSourceResistance( void ) const
{
  return negativeSourceResistance;
} 
void CalibatedParameters::setNegativeSourceResistance( double input )
{
  negativeSourceResistance = input;
}

 

//
// Set and Get methods for CalibatedParameters::positiveSourceResistance
//

double CalibatedParameters::PositiveSourceResistance( void ) const
{
  return positiveSourceResistance;
} 
void CalibatedParameters::setPositiveSourceResistance( double input )
{
  positiveSourceResistance = input;
}

 

//
// Set and Get methods for CalibatedParameters::zeroSystemOffset
//

double CalibatedParameters::ZeroSystemOffset( void ) const
{
  return zeroSystemOffset;
} 
void CalibatedParameters::setZeroSystemOffset( double input )
{
  zeroSystemOffset = input;
}

 

