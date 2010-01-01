/*
  Copyright 2002 - 2004 by D.K. McCombs.
  ======================================
  Needs:    adminPane.htmp divform.htmp navlist.htmp
*/


#include <iostream>
#include <iomanip>
#include "cgiTemplates.h"
#include "connectstring"
#include "forms.h"
#include "ocTypes.h"
#include "ocString.h"
#include "read_write_base.hpp"
#include "forms_base.hpp"
#include "cgiTemplates.h"
#include "role_services.hpp"
#include "openLogin.h" 
#include "siteLimit.h"
#include "ocXML.h"
using namespace std;
openLogin oLogin;
#include "InfoPoints.hpp"
#include "page_obj.hpp"
#include "superlist_base.hpp"
#include "page_template.hpp"


class pages_List:  public list_base
{
public: 
  // Constructor
  pages_List(cgiScript&sc,Session_Obj & session):list_base(sc,session){;}   
  ~pages_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;

    hotCol=-2;
    itemsPerPg = 25;
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","pageBuilder.cgi");

    addedCriteria = " site_id ";
    addedCriteria += siteFocus();

    emitFilter( "pageBuilder.cgi",
                "<strong>Pages · Search by Name</strong>" );
    string heading =
      "<a class='sortcol' href='pageBuilder.cgi?sort=id'>Page Id</a>|"
      "<a class='sortcol' href='pageBuilder.cgi?sort=name'>Page Name</a>"
                    ;
    emitHeadings(heading);
    getFilteredData( "id, "
                     "name " 
                     ,"pages",
                     "name like '$filter$%'" );
    emitData();
    emitNavigation("pageBuilder.cgi");
    emitEnd();
    return breturn;
  }      
};
 
class page_form:  public page, public forms_base
{
public:
  string currentsite;
  string program;
  
  page_form(cgiScript & script):page(),forms_base(script){setKey(*this);} 
  virtual ~page_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "id", id );
  }
  void form_data_transfer( void )
  {
    // Make data in controls back to object
    currentsite = currentSite();
    site_id = atoll(currentsite.c_str());
    llongFXfer( "template_id", template_id );
    stringFXfer( "name", name );
    stringFXfer( "program", program );
    stringFXfer( "url", url );
    stringFXfer( "meta_description", meta_description );
    
    if( program.length() > 0 && url.find("/") == string::npos )
    {
      string temp = url;
      url = program;
      if( temp.length() )
      {
        url += "/";
        url += temp;
      }
    }
  } 

  bool dbf_action( string mode, changeMap & changes )
  {
    // xlate program changes to url changes
    if(changes.find("program") != changes.end() )
    {
      changes["url"]="url";
    } 
    bool ret = db_action( mode, changes );

    ocString testURL = url;
    if( testURL.find("meta") != string::npos )
    {
      program = testURL.parse("/");
      
      if( testURL.endOfParse() == false )
        url = testURL.remainder();
      else
        url = "";
    }
    return ret;
  } 
  
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    ocString sql;
    string siteLimitation =   siteFocus();
    script << makeTop("pageBuilder.cgi", "Page Builder")
           << formTemplate.getParagraph("advanced_begin")
           << makeStaticBox("Id", "id", id, "12")
           << makeStaticBox( "Site Id", "site_id", site_id, "12");
    script << "<br class='clearall'>" << endl
           << makeTextBox("SEO Key Phrases","meta_description",meta_description, "355", "50")
           << "<br class='clearall'>" << endl
           << formTemplate.getParagraph("advanced_end");
                            
    sql = "select id, name from templates";
    if( siteLimitation.length() )
    {
      sql += " where site_id ";
      sql += siteLimitation;
    }  
    script << makeComboBox( "Template", "template_id", template_id, sql );
    script << "<a href=\"javascript:jopen('templates_Popup.cgi','scrollbars,resizable,width=450,height=300','subchild')\">Manage Templates</a>" << endl;
    script << "<br class='clearall'>" << endl;
    script << makeTextBox("Title","name",name,"50", "25")
            << "<br class='clearall'>" << endl
            << makeTextBox("URL","url",url,"255", "25");
    script << "<br class='clearall'>" << endl
           << makeProgramComboChoice() << "<br class='clearall'>" << endl;
    if( id )
    {
      script << "<br class='clearall'>" << endl;
      script << "<a style=\"float: right; color: green;\" target='_blank' href=\"" << program << (url.length()>0?"/":"") << url << "\">Show Page</a>" << endl;
      script << "<br class='clearall'>" << endl;
    }

    script  << makeButtons( key() );

    script << makeBottom( m_result ) << endl; 
    return breturn;
  }
  string makeProgramComboChoice(void )
  {
    ocString ret;
    ocString values = "please choose=,";
    if( template_id )
    {
      // Now we have the template id
      page_template ptmp;
      if( ptmp.load(template_id) )
      {
        // Now we want the control section and parse the xml to get the menu section named mnu.name
        ocString itemsToLookyAt =ptmp.getParagraph("compatible");
        if( itemsToLookyAt.length() )
        {
          // create a combo values string
          string val;
          do
          {
            val = itemsToLookyAt.parse(",");
            values += val;
            values += "=";
            values += val;
            values += ",";

          }
          while( !itemsToLookyAt.endOfParse() );
          
          // pull the last comma
          if( values.length())  values = values.substr(0, values.length()-1);
          
        } else  script << "No Could Not Find programs in template !<br>" << endl;
      } else script << "Could Not Load Template " << template_id << "!<br>" << endl;
      ret = makeManualComboBox( "Program", "program", program, values );
    }
    return ret;
  } 
};

bool checkLogin(cgiScript & script)
{
  infoPoints iPoints;
  if( iPoints.idToken.length() )
  {
    oLogin.token = iPoints.idToken;
  }

  bool isSignedOn = oLogin.testLoginStatus();
  
  if ( (isSignedOn == false ) &&
       (script.ClientArguments().count("login")==1) &&
       (script.ClientArguments().count("password")==1) && 
       (script.ClientArguments().count("group_id")==0) ) // this last bit is for user entry form detection
  {

    aString & asLogin = script.ClientArguments()["login"];
    aString & asPassword = script.ClientArguments()["password"];
    //DBG     cout << "\r\n\r\nCHECK: " <<  asLogin  << " . " <<   asPassword  << "<br>" << endl;
    isSignedOn = oLogin.checkUser( asLogin.str(), asPassword.str() );
  }
  return isSignedOn;
} 
int main( int argcount, char ** args )
{
  cgiScript script( "text/html", false );
  page_form myFrm(script); 
  
  cgiTemplates pgoo;
  pgoo.load("Templates/childPane.htmp");

  
  if( checkLogin(script) )
  { 
    script.closeHeader();
    script << pgoo.getParagraph("top");
    myFrm.loadControlTemplates("Templates/divform.htmp");  
    myFrm.form_action();  
    myFrm.form_display();
    pages_List mylist(script,oLogin.Session());
    mylist.loadListTemplates("Templates/navlist.htmp");  
    mylist.list_display();
  }
  else
  {
    script.closeHeader();
    script << pgoo.getParagraph("top");
    script << "<form action=\"\" id=\"uiForm\" name=\"uiForm\" method=\"post\">"
           << "<h2>User Login</h2>" << endl
           << "<label> Login: <input name=\"login\" size=\"25\" maxlength=\"25\" id=\"login\" type=\"text\"></label>" << endl
           << "<br>&nbsp;<br class=\"clearall\">" << endl
           << "<label> Password: <input name=\"password\" size=\"25\" maxlength=\"25\" id=\"password\" type=\"password\"></label>" << endl
           << "<br class=\"clearall\">" << endl
           << "<input value=\"Sign In\" type=\"submit\">"
           "<br class=\"clearall\"></form> " << endl;
  }
  ocString end = pgoo.getParagraph("bottom");
  script << end;
}

// compile implementations here
#include "read_write_base.cpp"
#include "forms_base.cpp"
