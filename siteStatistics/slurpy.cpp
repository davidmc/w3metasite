
/*

Assumed common log file format
 LogFormat "%h %l %u %t \"%r\" %>s %b \"%{Referer}i\" \"%{User-Agent}i\"" combined     (from httpd.conf)
  "%h %l %u %t \"%r\" %>s %b \"%{Referer}i\" \"%{User-agent}i\""
 Where
  %h :=  Remote host
  %l :=  Remote logname 
  %u :=  Remote user
  %t :=  Time the request was received
  %r :=  First line of request
  %>s := Status.of original request before any redirects
  %b := Size of response in bytes
  %{Referer}i := Referer Header value
  %{User-Agent}i := User Agent Header value


  // We are not using this, but it would be really helpful for Session Cookies! 
  %...{Foobar}C := The contents of cookie Foobar in the request sent to the server.

*/

#include <iostream>
#include <iomanip>
#include "connectstring"
#include "Site_History.hpp"

int main( int argc, char ** argv )
{
  Site_History_Obj sitehist;
  sitehist.load();
  return 0;
}

#include "read_write_base.cpp"
