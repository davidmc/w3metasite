/* 
  avsmap.h
  
  avs map for autofilling CC values

*/

#ifndef AVSMAP_H
#define AVSMAP_H
#include <string>
#include <map>
using namespace std;

typedef map<string,string> avs_key_to_values;

class avs_map: public avs_key_to_values
{
public:
  avs_map()
  {
    // Tailored for trust commerce for now.
    // I will have contitional ifdefs as we use this for others
    insert(make_pair("address1",""));
    insert(make_pair("address2",""));
    insert(make_pair("city",""));
    insert(make_pair("state",""));
    insert(make_pair("zip",""));   
  }
  avs_map(const avs_key_to_values & in):avs_key_to_values(in) 
  {  
  }
  virtual ~avs_map() { ;}
  avs_map & operator = (const avs_key_to_values & in)
  {
    dynamic_cast<avs_key_to_values&>(*this) = in;
    return *this;
  }
};




#endif
