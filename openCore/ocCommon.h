#include <string>
#include <map>
#ifndef OC_COMMON_H
#define OC_COMMON_H
using namespace std;
/*
  make a file info class
  complete enough
  class to work with STL
*/
struct ocFile
{
  string name;
  string path;
  string type;
  ocFile(){;}
  ocFile( const ocFile & in )
  :name(in.name),path(in.path),type(in.type){;}
  ocFile & operator = ( const ocFile in )
  {
    name = in.name;
    path = in.path;
    type = in.type;
    return * this;
  }
  virtual ~ocFile(){;}
};

typedef map< string, ocFile> ocFiles;
#endif
