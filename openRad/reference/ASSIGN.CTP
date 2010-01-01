<CLASS>::<CLASS>() 
<<PARENTS>>
<<INITIALIZERS>>
{
  ;
}

<CLASS>::~<CLASS>()
{
  <<DESTROYERS>>
}


<CLASS>::<CLASS>( <CLASS> & input )
{
  <<ASSIGNS>>
}


<CLASS> & <CLASS>::operator = ( <CLASS> & input )
{
  if( this != &input )
  {
    <<ASSIGNS>> +2
  }
  return *this;
} 

