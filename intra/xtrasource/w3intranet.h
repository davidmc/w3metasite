/*
  W3 Intranet.h
  Common file for intranet business applications
*/
#ifndef W3_Intranet_h
#define W3_Intranet_h
// for a new system, to make it show all services before secuity is set up
// #define BOOTSTRAP




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
#include "flexButtons.hpp"

// forward declaration for compiler.
bool intraMain(page & pg ,cgiScript & script);


bool check(cgiScript & script)
{
  bool isSignedOn = false;

  // New: need to be able to override where to get the data and what cookie to set for id
  infoPoints iPoints;
  if( iPoints.idToken.length() )
  {
    oLogin.token = iPoints.idToken;
    // DBG   cout <<  "\n\n oLogin.token: : " << oLogin.token << endl;
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
    //DBG    cout << "\r\n\r\nTEST LOGIN: <br>" << endl;
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
  isEditMode = false;
  
  if( isSignedOn )
  {
    // set the global edit mode
    Session_Obj & sess = oLogin.Session();
    string editMode = sess.GetData("editMode");
    if( editMode == "true" )
    {
      isEditMode = true;
    }
    if( script.ClientArguments().count("svc_id") )
    {
      string svc_id = script.ClientArguments()["svc_id"].c_str();
      oLogin.Session().SetData("svc_id",svc_id);
    } 
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
  virtual ~servicelist_functor(){;} 

  /*
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
      string lastCat;
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
            script << catheadingTemplate.replace("$content",lastCat.c_str());
          }
          script << catitemTemplate.replace("$level",service.tree_depth.c_str())
                                   .replace("$location", fixURI(service.uri,service.id).c_str())
                                   .replace("$label",service.menu_name.c_str());
        }
      }
 
      string loggedIn = oLogin.FullName();
      script << catheadingTemplate.replace("$content",loggedIn.c_str());
      script << catitemTemplate.replace("$level","1")
                               .replace("$location","?signoff=true")
                               .replace("$label","Sign Off"); 
    }
    return bRet;
  }*/
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
      string lastCat;
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
            script << catheadingTemplate.replace("$content",lastCat.c_str());
          }
          script << catitemTemplate.replace("$level",service.tree_depth.c_str())
                                   .replace("$location", fixURI(service.uri,svcNames[ix]).c_str())
                                   .replace("$label",service.menu_name.c_str());
        }
      }
      string loggedIn = oLogin.FullName();
      script << catheadingTemplate.replace("$content",loggedIn.c_str());
      script << catitemTemplate.replace("$level","1")
                               .replace("$location","?signoff=true")
                               .replace("$label","Sign Off");
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
      ocString sql = "select url from sites where id = ";
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
