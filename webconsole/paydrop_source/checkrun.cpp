/*
  Member Check Run 
  output virtual csv files (if logged in)
  ===================================
  CGI User Interface for Pay Drop Member
*/

#include <iostream>
#include <iomanip>
#include "ocString.h"
#include "cgiClass.h"
#include "read_base.hpp"
#include "nav_menu.h" // for global paydropLogin oLogin;

int main( int argc, char ** argv )
{  

  cgiScript scrpt("text/plain");  // pretty sure this is the mime type, the extension should help too
  // could use "application/vnd.ms-excel"
  // someone made up "text/comma-separated-values"
  if( oLogin.testLoginStatus() )
  {    
    ocString sqlTemplate = "select concat(MemberFirstName, ' ', MemberLastName ) as MemberName, MemberAddress1, "
                           "MemberAddress2, MemberCity, MemberState, MemberPostalCode, MemberEmail, CheckAmount "
                           "from Member m inner join MemberCheck mc on m.MemberID = mc.MemberId "
                           "where mc.CheckDate = '__DATE__'";
    ocString resultTemplate                      
                  = "\"MemberName\",\" MemberAddress1\",\" MemberAddress2\",\" MemberCity\",\" MemberState\","
                    "\" MemberPostalCode\",\" MemberEmail\", CheckAmount\r\n";
    quickQuery qry;
    openRS & rs = qry.getRS();
    ocString pathInfo = scrpt.PathInfo().c_str();
    string checkDate = pathInfo.parse(".");
    if( checkDate.length() )
    {
      string sql = sqlTemplate.replace("__DATE__",checkDate);
      scrpt << resultTemplate; // header
      for( bool isOpen = rs.open(sql); isOpen; isOpen=rs.next() )
      {
        scrpt <<
          resultTemplate
            .replace( "MemberName", rs.getField(0).format().c_str() )
            .replace( "MemberAddress1", rs.getField(1).format().c_str() )
            .replace( "MemberAddress2", rs.getField(2).format().c_str() )
            .replace( "MemberCity", rs.getField(3).format().c_str() )
            .replace( "MemberState", rs.getField(4).format().c_str() )
            .replace( "MemberPostalCode", rs.getField(5).format().c_str() )
            .replace( "MemberEmail", rs.getField(6).format().c_str() )
            .replace( "CheckAmount", rs.getField(7).format().c_str() );               
      }
    }    
  }
};

#include "read_write_base.cpp"


 
