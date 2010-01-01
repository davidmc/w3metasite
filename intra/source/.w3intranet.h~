/*
  W3 Intranet.h
  Common file for intranet business applications
*/
#ifndef W3_Intranet_h
#define W3_Intranet_h
// for a new system, to make it show all services before secuity is set up
// #define BOOTSTRAP


#include "openLogin.h" // open-login has own connectstring

openLogin oLogin;

#include "connectstring"
#include "cgiClass.h"
#include "page.hpp"
#include "page_control.hpp"
#include "page_enhancements.hpp"
#include "forms.h"
#include "ocTypes.h"
#include "read_write_base.hpp"
#include "superlist_base.hpp"
#include "forms.h"
#include "forms_base.hpp"
#include "lookup.hpp"
#include "InfoPoints.hpp"

// forward declaration for compiler.
bool intraMain(page & pg ,cgiScript & script);

string flexButtons( long long key,
                    string iLabel="&nbsp;Insert&nbsp;",
                    string uLabel="&nbsp;Update&nbsp;", 
                    string dLabel="&nbsp;Delete&nbsp;",
                    string rLabel="&nbsp;New - Reset&nbsp;",
                    string nLabel="&nbsp;New - Keep Data&nbsp;",
                    bool doInsert = true,
                    bool doUpdate = true,
                    bool doDelete = true,
                    bool doReset = true,
                    bool doKeepData = true )
{
  string ret;
  if( key )       
  { // Appropriate for update/delete
    if( doUpdate )
    { 
      ret += "<input class='submitButtons' type='button' onclick='doUpdate()' name='action' class='action' value='";
      ret += uLabel;
      ret += "'>";
    }
    if( doDelete )
    {
      ret += "<input class='submitButtons' type='button' onclick='doDelete()' name='action' class='action' value='";
      ret += dLabel;
      ret += "'>";
    }
    if( doReset )
    {
      ret += "<inputclass='submitButtons'  type='button' onclick='doReset()' name='action' class='action' value='";
      ret += rLabel;
      ret += "'>";
    }
    if( doKeepData )
    {
      ret += "<input type='button' onclick='doNew()' name='action' class='action' value='";
      ret += nLabel;
      ret += "'>";
    }
  }
  else
  { // Appropriate for insert
    if( doInsert )
    {
      ret += "<input type='button' onclick='doInsert()' name='action' class='action' value='";
      ret += iLabel;
      ret += "'>";
    }
    else
    { // hide the fact your inserting, make it look like an update, you will never get here unless this is the intent
      ret += "<input type='button' onclick='doInsert()' name='action' class='action' value='";
      ret += uLabel;
      ret += "'>";
   }
  }
  return ret;
}

bool check(cgiScript & script)
{
  bool isSignedOn = false;

  // New: need to be able to override where to get the data and what cookie to set for id
  infoPoints iPoints;
  if( iPoints.idToken.length() )
  {

    oLogin.token = iPoints.idToken;
    // DBG cout <<  "\n\n oLogin.token: : " << oLogin.token << endl;
  }                                        
  // end of new
  
  if ( (script.ClientArguments().count("login")==1) &&
       (script.ClientArguments().count("password")==1) && 
       (script.ClientArguments().count("group_id")==0) ) // this last bit is for user entry form detection
  {

    aString & asLogin = script.ClientArguments()["login"];
    aString & asPassword = script.ClientArguments()["password"];
    //DBG     cout << "\r\n\r\nCHECK: " <<  asLogin  << " . " <<   asPassword  << "<br>" << endl;
    isSignedOn = oLogin.checkUser( asLogin.str(), asPassword.str() );
  }
  else
  {
    //DBG  cout << "\r\n\r\nTEST LOGIN: <br>" << endl;
    // attempt to get cookie
    if ( oLogin.testLoginStatus() )
    {
      if( script.ClientArguments().count("signoff") )
      {
        cgiCookie cookie;
        cookie.setTimeout( 1, 1, 1970, 0, 0 );
        cookie.setPath("/");
        cookie.set( oLogin.token.c_str(), "" );
        isSignedOn = false;
      }
      else
      {
        isSignedOn = true;
      }
    }
  }
  // New - retention of this service id
  if( isSignedOn && script.ClientArguments().count("svc_id") )
  {
    string svc_id = script.ClientArguments()["svc_id"].c_str();
    oLogin.Session().SetData("svc_id",svc_id);
  } 
  return isSignedOn;
} 

class servicelist_functor: public base_functor
{
  cgiScript & script;
  bool isSignedOn;

  // string for path lookup
  string path;
public:
  
  servicelist_functor(page & ipg,cgiScript & script, bool isSignedOn)
  :base_functor(ipg),script(script),isSignedOn(isSignedOn){;}
  virtual ~servicelist_functor()
  {
    ;
  } 
  
  virtual  bool operator()( xmlNode & node )
  {
    bool bRet = false;    
    string item;
    if( isSignedOn )
    {
      bRet = true;

      // This is where the menu of services goes
      ocString catheadingTemplate = pg.paragraph_string( "catheading" );
      ocString catitemTemplate = pg.paragraph_string( "catitem" );

      // New! from creating a cat item container
      ocString catContainerSeperator = pg.paragraph_string( "serviceContainerSeperator" );
      ocString catContainerStart = pg.paragraph_string( "serviceContainerStart" );
      ocString catContainerEnd = pg.paragraph_string( "serviceContainerEnd" );

      // New! to seperate the cat items from the service items
      bool bSeperateServices = false;
      node_attr::iterator x =  node.attr.find("seperateServices");
      if( x !=node.attr.end() )  bSeperateServices = true;
      
      ocString lastCat = "";
      if( oLogin.fetchServices() )
      {
        serviceVector & svcNames = oLogin.ServiceNames();
        serviceMap & svcs = oLogin.Services();
        for(int ix=0;ix<svcNames.size();++ix)
        {
          openService & service = svcs[svcNames[ix]];
          if( lastCat != service.cat_name )
          {
            lastCat = service.cat_name;
            script << catheadingTemplate.replaceAll("$content",lastCat.c_str()).replaceAll("$id",lastCat.replaceAll(" ","_"));
          }
          if( bSeperateServices == false )
          {
            script << catitemTemplate.replaceAll("$level",service.tree_depth.c_str())
                                    .replaceAll("$location", fixURI(service.uri,svcNames[ix]).c_str())
                                    .replaceAll("$label",service.menu_name.c_str());
          }
        }
        if(  bSeperateServices )
        {
          lastCat = "";
          int ix = 0;
          if( catContainerSeperator.length() )
          {
            script << catContainerSeperator;
          }
          for(ix=0;ix<svcNames.size();++ix)
          {
            openService & service = svcs[svcNames[ix]];
            if( lastCat != service.cat_name )
            {
              lastCat = service.cat_name;
              if( ix > 0 && catContainerEnd.length() )
              {
                script << catContainerEnd;
              }
              
              if(  catContainerStart.length() > 0 )
              {
                script << catContainerStart.replaceAll("$content",lastCat.c_str()). replaceAll("$id",lastCat.replaceAll(" ","_"));
              }
            }
            script << catitemTemplate.replaceAll("$level",service.tree_depth.c_str())
                                     .replaceAll("$location", fixURI(service.uri,svcNames[ix]).c_str())
                                     .replaceAll("$label",service.menu_name.c_str());
          }
          // finish closing out the last service container
          if( ix > 0 && catContainerEnd.length() )
          {
            script << catContainerEnd;
          }
        }
        
      }
      
      if( ! bSeperateServices )
      {
        if( catheadingTemplate.length() > 0 )
          script << catheadingTemplate.replace("$content",oLogin.FullName().c_str());
      }
      else  if (catContainerStart.length() > 0 )
      {
         script << catContainerStart.replaceAll("$content",oLogin.FullName().c_str()).replaceAll("$id",ocString(oLogin.FullName()).replaceAll(" ","_"));
      }

      script << catitemTemplate.replace("$level","1")
                               .replace("$location","?signoff=true")
                               .replace("$label","Sign Off");
      if(  bSeperateServices  )
      {
        if(  catContainerEnd.length() > 0 )
          script << catContainerEnd;
      }
    }
    return bRet;
  }
  string fixURI( ocString uri, string & id )
  {
    // if it already has a http:, ftp: or whatever in front, good, just return
    if( uri.regExMatch(":") )  return uri;

    // make and absolute URI
    string fixedURI = "http://";
 
    // add the domain
    fixedURI += script.ServerName().c_str();
    
    // get the path if need be
    if( path.length() == 0 )
    {
      ocString sql = "select url from metasite.sites where id = ";
      sql.append(pg.site_id());
      path = tableLookup( sql );
    }

    // if not base relative uri
    if( uri.regExMatch("/") == false )
    {
      // use the metasite path
      fixedURI += path;
    }
    // finish building a fully qualified uri
    fixedURI += uri;
    // now add the service id
    if( uri.regExMatch("\\?") )
    {
      fixedURI += "&svc_id=" + id;
    }
    else
    {
      fixedURI += "?svc_id=" + id;
    }
    return  fixedURI;
  }
};

class form_functor: public base_functor
{
  cgiScript & script;
  bool isSignedOn;
  //list_base & myList;
  //forms_base & myForm;
public:

  form_functor(page & ipg
               ,cgiScript & script
               ,bool isSignedOn
               //,list_base & theList,
               //,forms_base & theForm 
               )
  :base_functor(ipg)
  ,script(script)
  ,isSignedOn(isSignedOn)
  // ,myList(theList),myForm(theForm)
  {;}  
  virtual ~form_functor(){;} 

  virtual  bool operator()( xmlNode & node )
  {
    bool bRet = false;    

    if( isSignedOn )
    {
      bRet = intraMain( pg , script);      
    }
    else
    {
      // please sign in!
      script << pg.paragraph_string( "sign_on_form" );
      // no content after the form
      pg.get_page_paragraphs().clear();
    }
    return bRet;
  }

};


#endif
