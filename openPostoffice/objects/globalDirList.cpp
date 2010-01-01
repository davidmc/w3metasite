/*

  Copyright 2002
  Author David McCombs.

*/
#include "openLogin.h"
#include "cgiTemplates.h"

void  globalDirList(cgiScript & script, openLogin & login)
{
  cgiTemplates htmlDoc;
  htmlDoc.load("filteredList.html");
  script << ocString(htmlDoc.getParagraph("top"))
            .replace( "_entity_", "Global Directory" )
            .replaceAll( "_link_", "globalDirList.cgi" )
            .replaceAll( "_flink_", "globalDirForm.cgi" )
            .replace( "_header_", "Co. Name</th><th>Mail Group</th><th>Name" )
         << endl;

  postgresDB db(CONNECTSTRING);
  postgresRS rs(db);
  string sql = "select gd.id, g.name as group_name, mg.name as mg_name, "
               "gd.first_name || ' ' ||  gd.last_name as name from vw_global_directory gd "
               "inner join groups g on g.id = gd.group_id "
               "inner join mail_groups mg on mg.id = gd.mail_group_id";
  if( script.ClientArguments().count("s") > 0 )
  {
    sql += " where lower(last_name) like '";
    sql += script.ClientArguments()["s"].str();
    sql += "%'";
  }
  if( !login.SiteAdmin() )
  {
    sql+=(script.ClientArguments().count("s") > 0 )?" and ":" where ";
    sql += " gd.group_id = ";
    sql += login.GroupId();
  }

  sql += " order by last_name";
  bool b = rs.open(sql);
  if( !b )
  {
     script << "SQL: " << rs.getErrors();
  }
  for( ;
       b;
       b= rs.next() )
  {
    string listData = rs.getField(1).format();
    listData += "</td><td>";
    listData += rs.getField(2).format();
    listData += "</td><td>";
    listData += rs.getField(3).format();
    script << ocString(htmlDoc.getParagraph("listitem"))
    .replace( "_link_", "globalDirForm.cgi" )
    .replaceAll( "_id_", rs.getField(0).format().c_str() )
    .replace( "_listdata_", listData.c_str() ) << endl;
  }
  script << htmlDoc.getParagraph("bottom") << endl;
}

int main( int argcount, char ** args )
{
  bool isSignedOn = false;

  // An instance of the script
  // must be created before any other output.
  cgiScript script("text/html",false);
  openLogin login;

  // attempt to get cookie
  if ( login.testLoginStatus() )
  {
      isSignedOn = true;
  }
  if( isSignedOn )
  {
     script.closeHeader();
     globalDirList(script,login);
  }
  else
  {
    // script.Redirect("signIn.html");
    script.closeHeader();
    script << login.getLastError() << "<br>" << endl;
    script << login.getSql() << endl;
    return(0);
  }
}
