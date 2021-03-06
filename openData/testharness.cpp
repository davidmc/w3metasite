#include "openDB.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "ocString.h"

using namespace std;

string condition( ocString input )
{
  string ret = "\"";
  ret += input.replaceAll("\"","\"\"");
  ret += "\"";
  return ret;
}
int main( int argc, char * argv[])
{

  if( argc == 2 )
  {
    // cout << "Creating a db" << endl;
  
    openDbFactory DbMaker(MySQL, "host=localhost;uid=http_user;pwd=mydeal99;db=mgdist");
    openDB & db = DbMaker.db();
  
    //cout << "db established : provider is: " << db.getProvider() << endl;
  
    if( db.isGood() )
    {
      //cout << "Good Connection -- Creating an rs" << endl;
      openRsFactory RsMaker(db);
      openRS & rs = RsMaker.rs();
  
      //cout << "rs established - making a query" << endl << endl;
      ocString sql = "select * from ";
      sql += argv[1];
      if( rs.open(sql) )
      {
        //cout << "Query made!" << endl;
        int i;
        for( i=0; i<rs.getFieldCount();i++)
        {
          cout << rs.getField(i).getName();
          if(  i+1<rs.getFieldCount() ) cout << ",";
        }
        cout << endl ;
        do{
  
            for( i=0; i<rs.getFieldCount();i++)
            {
              string name =  rs.getField(i).getName();
              cout << condition(rs.getField(name.c_str()).format());
              if(  i+1<rs.getFieldCount() ) cout << ",";
            }
            cout << endl;
  
        }while( rs.isOpen() && rs.next() );
      }
      rs.close();
    }
    else
    {
      cout << "connection ain't no good" << endl;
      // cout << dynamic_cast<mySqlDB&>(db).errorString() << endl;
    }
  }

}
