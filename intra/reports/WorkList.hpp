/*
  WorkList.hpp

  User Interface Class Definition 
  for Report.

  Copyright (C) 2004 by D.K. McCombs.
  W3 Systems Design  

*/
#ifndef REPORTS_HPP
#define REPORTS_HPP

#include <iostream>
#include <iomanip>
#include "cgiTemplates.h"
#include "connectstring"
#include "forms.h"
#include "ocTypes.h"
#include "ocString.h"
#include "cgiTemplates.h"
#include "openLogin.h"

#include "Report.hpp"

#include "superlist_base.hpp"

#include "col_handling_rep.hpp"

#include "Composite_Query.hpp"

#include "siteLimit.h"

#include "lookup.hpp"


using namespace std;

class WorkItems:  public list_base
{

  Composite_Query_Obj cQuery;
  ocString sql;
  string workDescription;  
  string workUser;
  string workUser_Group;
  string workUser_Role;
  string workURL;
  string workSearch;
  
protected:  
  ocString part( string id )
  {
    return listTemplate.getParagraph(id);
  }
  
public: 
  bool setQuery( identifier key )
  {
    bool bret=false;
    cQuery.key(key);
    if(cQuery.get_data())
    {
      sql = cQuery.asSQL();
      bret = true;
    }
    return bret;
  }

  // Constructor
  WorkItems(cgiScript&sc, Session_Obj & session):list_base(sc,session)  
  {
    itemsPerPg = 9999;
  }  
  ~WorkItems(){;}
  
  bool list_display( string workDescription,
                     string workUser,
                     string workUser_Group,
                     string workUser_Role,
                     string workURL,
                     string workSearch )
  {
    bool breturn = true;
    this->workDescription = workDescription;
    this->workUser = workUser;
    this->workUser_Group = workUser_Group;
    this->workUser_Role = workUser_Role;
    this->workURL = workURL;
    this->workSearch = workSearch;
    hotCol=-1; 
    skipCol=0;   
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","Reports.cgi").replace("_self","report");

    // Conditioning based on role group etc
    if( roleFilter() )
    {
      if( getData( sql ) )
      {
        webIO << "<br /><em>" << workDescription << " &nbsp (" << recCount << " task" << (recCount==1?"":"s") << ")</em>" << endl;
        webIO << "<img class='opener' src=\"pg_images/add.png\" alt=\"Open\" onclick=\"openItems(this)\">";
        webIO << "<img class='closer' src=\"pg_images/subtract.png\" alt=\"Close\" onclick=\"closeItems(this)\" style='display:none'>";
        webIO << listTemplate.getParagraph("list_begin");
        emitHeadings(); 
        emitData();             
        emitEnd();
        webIO << "<!--" << sql << "-->" << endl;
      }
      else
      {
        webIO << "<!--" << sql << "-->" << endl;
      }
    }
    return breturn;
  }
  bool roleFilter( void )
  {
  
    ocString uid;
    uid.append(oLogin.Id());
    string gid = oLogin.GroupId();
    
    bool bret=true;
    // check the work user, group and role
    if( workUser.length() && workUser != "-1" )
    {
      // this todo is for me?
      if( uid != workUser )
      {  
        bret=false;
        webIO << "<!--oLogin.Id() != " << workUser << "-->" << endl;
      }
      else
      {
        sql = sql.replaceAll("{{user}}",workUser.c_str());
      } 
    }   
    if( bret && workUser_Group.length() && workUser_Group != "-1" )
    {       
      // this todo is for my group?
      if( gid != workUser_Group ) 
      {
        bret=false;
        webIO << "<!--oLogin.GroupId() != " << workUser_Group << "-->" << endl;
      }
      else
      {  
        sql = sql.replaceAll("{{group}}",workUser_Group.c_str());
      }  
    }
    if( bret && workUser_Role.length()  && workUser_Role != "-1" )
    {
      // do I have such a role?
      string sql2 = "select count(*) from user_roles where user_id = ";
      sql2 += uid;
      sql2 += " and role_id = ";
      sql2 += workUser_Role;
      string cnt = tableLookup ( sql2 );
      if( cnt == "0" )
      {
        bret=false;
        webIO << "<!--'0' role result from query: " << sql2 << "-->" << endl;
      }  
      else  
      {
        sql = sql.replaceAll("{{role}}",workUser_Role.c_str());
      }      
    }
    if( bret )
    {
      // final filters
      
      // go ahead and filter by logged in uid
      sql = sql.replaceAll( "{{login}}", uid.c_str() );
      
      // go ahead and filter by staff
      sql = sql.replaceAll( "{{staff}}",oLogin.myStaff().c_str() );

      // check for session workflow filters
      ocString sessionFilters = session.GetData("LoginFilters");
      if( sessionFilters.length() )
      {
        ocString sessionFilter = sessionFilters.parse("&");
        while( sessionFilter.length() )
        {
          string name = sessionFilter.parse("=");
          string value =  sessionFilter.parse("=");
          
          sql = sql.replaceAll(name,value);
          sessionFilter = sessionFilters.parse("&");
        }
      }
    }
    
    return bret;
  }
    
  list_base & emitHeadings( void )
  {
    
    ocString head =  part("tr");
    ocString th = part("th");
    for( int i=0; i < rs.getFieldCount(); ++i )    
    {
      if( i != skipCol )
      {
        string heading = rs.getField(i).getName();
        head += th.replace( "$data$", heading.c_str() );
      }  
    } 

    head += part("end_tr");
    webIO << head;
    return * this;
  }
  
  virtual void sendField( int iField, ocString & td )
  { 
    td.parseInit();
    string tdStart = td.parse("$data$");
    string tdEnd = td.parse("$data$");
    
    webIO << tdStart;
    if( workURL.length() )
    {
      ocString  workURLParts = workURL;
      webIO << "<a href='" << workURLParts.parse("?") << "?Id=" << rs.getField(0).format();
      if( workSearch.length() )
      {
        // Added search criteria assumed in the form 'var={{n}}'
        ocString searchTemplate = workSearch;
        string searchMore = searchTemplate.parse("{{");
        string column = searchTemplate.parse("}}");
        if( column.length() )
        {
          int col = atoi(column.c_str());
          searchMore += rs.getField(col).format();
        }
        webIO << "&" << searchMore;
      }
      if( workURLParts.endOfParse() == false )
        webIO << workURLParts.remainder();
      webIO <<  "'>";
    }
    webIO << rs.getField(iField).format(); 
    if( workURL.length() )
    {
      webIO << "</a>";
    }  
    webIO << tdEnd;    
  }
      
}; 
class Work_List:  public list_base
{

public: 
  // Constructor
  WorkItems workItems;
  Work_List(cgiScript&sc, Session_Obj & session):list_base(sc,session),workItems(sc,session)
  {
    itemsPerPg = 9999;
    
  }  
  ~Work_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;
    workItems.loadListTemplates("Templates/list.htmp");

    hotCol=-1; 
    skipCol=0;   
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","Reports.cgi").replace("_self","report");
                 //       0    1       2          3     4       5           6        7     8     
    if( getData( "select Id, Name, Description, Query, User, User_Group, User_Role, URL, Search "
                 "from Work order by Sequence" ) )
    {     
      webIO << listTemplate.getParagraph("list_begin"); 
      emitData();
      emitEnd();
    }         
    
    return breturn;
  }
  // this role filter just checks to see if this item is appropriate for this user
  bool roleFilter(  string workUser,
                    string workUser_Group,
                    string workUser_Role )
  {
  
    ocString uid;
    uid.append(oLogin.Id());
    string gid = oLogin.GroupId();
    
    bool bret=true;
    // check the work user, group and role
    if( workUser.length() && workUser != "-1" )
    {
      // this todo is for me?
      if( uid != workUser )
      {
        bret=false;
        webIO << "<!--oLogin.Id() != " << workUser << "-->" << endl;
      }
    }
    if( bret && workUser_Group.length() && workUser_Group != "-1" )
    {
      // this todo is for my group?
      if( gid != workUser_Group )
      {
        bret=false;
        webIO << "<!--oLogin.GroupId() != " << workUser_Group << "-->" << endl;
      }
    }
    if( bret && workUser_Role.length()  && workUser_Role != "-1" )
    {
      // do I have such a role?
      string sql2 = "select count(*) from user_roles where user_id = ";
      sql2 += uid;
      sql2 += " and role_id = ";
      sql2 += workUser_Role;
      string cnt = tableLookup ( sql2 );
      if( cnt == "0" )
      {
        bret=false;
        webIO << "<!--'0' role result from query: " << sql2 << "-->" << endl;
      }
    }
    return bret;
  }
  bool derived_predata_event( void )
  {
    bool btrue = true;
    // should we even show this row?...
    btrue =  roleFilter(  rs.getField(4).format(),
                          rs.getField(5).format(),
                          rs.getField(6).format() );
    return btrue;
  }
  virtual void sendField( int iField, ocString & td )
  { 
    if( iField == 1 )
    {
      td.parseInit();
      string tdStart = td.parse("$data$");
      string tdEnd = td.parse("$data$");
      webIO << tdStart << rs.getField(iField).format(); 
      

      workItems.setQuery( atoll( rs.getField(3).format().c_str() ) );
      // ordinal access is slightly quicker
      workItems.list_display( rs.getField(2).format(), // Description
                              rs.getField(4).format(), //  User
                              rs.getField(5).format(), //  User_Group
                              rs.getField(6).format(), //  User_Role
                              rs.getField(7).format(), //  URL
                              rs.getField(8).format()  //  Search
                            );
      
      webIO << tdEnd;
    }                
  }    
};
#endif



