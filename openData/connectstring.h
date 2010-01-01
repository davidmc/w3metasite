/*
connectstring.h
*/
#ifndef CONNECTSTRING_H
#define CONNECTSTRING_H
#include "ocEnc.h"
#include <fstream>
ocEnc conn;
const int encSeed = 787;
const char * filename = "constr";
const char * getConnectstring( const char * use_file = 0 )
{
  if(!use_file) use_file = filename;
  ifstream in(use_file);
  char buf[512];
  in.getline (buf,sizeof(buf));
  conn = buf;
  conn.decode(encSeed);
  return conn.c_str();
}
bool setConnectstring( char * r , const char * use_file = 0)
{
  bool ret = false;
  if(!use_file) use_file = filename;
  ofstream out(use_file);
  if(out && r and strlen(r) )
  { 
    conn = r; 
    conn.encode(encSeed);
    out << conn << endl;
    ret = true;
  }
  return ret;
}


#endif
