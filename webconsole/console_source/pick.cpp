/*
  pick.cpp
  ===================================
  CGI List for modules  
    
*/
#include "cgiClass.h"
#include "cgiTemplates.h"
#include "ocDynLib.hpp"
#include "ocFileSys.h"
#include "../metasite_source/openLogin.h"
#include "../metasite_source/site_pick.hpp"
#include "../metasite_source/site.hpp"

using namespace std;
class console_module 
{
public:  
  string module_name;
  string label;
  console_module()
  {
  }
  ~console_module()
  {
  }
  console_module & operator = ( const console_module & in )
  {
    module_name = in.module_name;
    label = in.label;
  }
};

typedef map< int, console_module> console_modules;

// global instances
console_modules cmodules;
string relPath = "./modules";

int pickOption( string module )
{

  string path = relPath;
  path += "/";
  path += module;
  // load the library
  ocDynamicLibrary mod1(path);

  if(mod1.IsLibraryLoaded()==false) 
  {
    cout << mod1.LastError() << endl;
    return 1;    
  }  

  // load the ordinal function 
  mod1.loadFunction("ordinal");
  if( mod1.IsFunctionLoaded()==false) 
  {
    cout << "couldn't find library function. " << mod1.LastError() << mod1.function() << endl;
    return 1;    
  } 
  // invoke it! 
  int ordinal_pos = ((int (*)())mod1.function())();
  
  // load the label function 
  mod1.loadFunction("label");  
  if( mod1.IsFunctionLoaded()==false) 
  {
    cout << "couldn't find library function. " << mod1.LastError() << mod1.function() << endl;
    return 1;    
  } 
  // invoke it! 
  string label = ((string (*)())mod1.function())();
  console_module mod;
  mod.module_name = module;
  mod.label = label;
  cmodules[ordinal_pos] = mod;
  // libary unloads as mod1 falls out of scope.
}

int main( int argc, char ** argv )
{
  cgiScript sc("text/html", false);
  cgiTemplates tmplt;
  tmplt.load("./Templates/pick.htmp");  
  /* 
    The next few lines provide an option list, 
    and set the site id cookie if only one site. 
    MUST be invoked before closing header 
  */
  site_pick pick_a_site(sc);
  openLogin login; 
  string options;
  if ( login.testLoginStatus() )
  { 
    options = pick_a_site.getSiteOptions(sc,login); 
  }
  // now close header
  sc.closeHeader();
  cgiHtml html;
  {
    cgiHead head;
    head << tmplt.getParagraph("headguts");    
  }
  cgiInput & args = sc.ClientArguments();
  cgiBody * pbody;
  string attributes;
  if( args.count("changeSite") )
  {
    siteObject site;    
    site.key(atoll(args["site_id"]));
    if(site.get_data())
    {
      attributes=" onload=\"reframe('http://";
      attributes+=site.domain;
      attributes+=site.url;
      attributes+="sys')\"";
      
    }    
  }
  pbody = new cgiBody((char*)attributes.c_str());;
  
  cgiBody & body = *pbody;  
  body << tmplt.getParagraph("pickformstart");  
  ocFileSys fs;
  if( fs.openDir(relPath.c_str()) )
  {
    ocDirectory & entries = fs.getDirectoryEntries();
    for(int i = 0; i < entries.size(); i++ )
    {
      ocString sotest = entries[i].name;
      if( sotest.regExMatch(".so$") )
      {
        pickOption( sotest );
      }
    }
  }
  console_modules::iterator pos;

  for( pos=cmodules.begin(); pos != cmodules.end(); ++pos )
  {
    ocString opt = tmplt.getParagraph("pickoption");
    console_module & mod = pos->second;
    body <<
      opt.replace("$value",mod.module_name.c_str())
        .replace("$label",mod.label);
  }
  ocString  pickformend =   tmplt.getParagraph("pickformend");    
  body << pickformend.replace("$OPTIONS$",options.c_str());

  delete pbody;  
  return 0;
};
#include "read_write_base.cpp"
