#include<stdio.h>
#include<iostream.h>
#include<iomanip.h>
#include<fstream.h>
#include<string.h>
#include"makeclas.hpp"


int main( int argc, char * argv[] )
{

  if (argc != 2)
  {
    cerr << "usage:" << endl;
    cerr << "makeclas classfile" << endl;
    return(1);
  }

  ClassDef defClass( argv[1] );
  defClass.WriteHpp();
  defClass.WriteCpp();

  return(0);

}
