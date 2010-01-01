#include <string.h>
#include <iostream>

class CalibatedParameters
{

private:


protected:

  double       openCircuitSourceNegative;
  double       openCircuitSourcePositive;
  double       negativeSourceResistance;
  double       positiveSourceResistance;
  double       zeroSystemOffset;

public:

  CalibatedParameters();
  ~CalibatedParameters();
  
  CalibatedParameters( CalibatedParameters & input );
  CalibatedParameters & operator = ( CalibatedParameters & input );
  
  
  double OpenCircuitSourceNegative( void ) const; 
  void setOpenCircuitSourceNegative( double input );

  double OpenCircuitSourcePositive( void ) const; 
  void setOpenCircuitSourcePositive( double input );

  double NegativeSourceResistance( void ) const; 
  void setNegativeSourceResistance( double input );

  double PositiveSourceResistance( void ) const; 
  void setPositiveSourceResistance( double input );

  double ZeroSystemOffset( void ) const; 
  void setZeroSystemOffset( double input );


};
