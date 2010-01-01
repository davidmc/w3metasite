/*
  links Form
  ===================================
  CGI User Interface for links
            

  Needs: childPane.htmp
         childdivform.htmp
         list.htmp
*/

#include "connectstring"
#include <iostream>
#include <iomanip>
#include "ocTypes.h"
#include "ocString.h"
#include "read_write_base.hpp"
#include "forms_base.hpp"
#include "list_base.hpp"
#include "cgiTemplates.h"
#include "openLogin.h"
#include "siteLimit.h"
#include "link.hpp"

openLogin oLogin;

// Include file
#include "sublist_base.hpp"

// Definition

class links_List:  public sublist_base
{
  cgiInput & args;
  public:
    ocString Parent_Id;
  // Constructor
    links_List(cgiScript&sc):sublist_base(sc),args(sc.ClientArguments()){;}
    ~links_List(){;}

    bool list_display( void )
    {
      bool breturn = true;
      Parent_Id = currentSite(  );
      string sql =  "select l.id, l.name, l.url, p.url from links l "
      "left join pages p on p.url = l.url "
      " where l.site_id = " ;
      sql +=  Parent_Id;
      sql += " order by  l.name";
      hotCol=-2;
      editLink = listTemplate.getParagraph("peerlink");
      editLink = editLink.replace("$prog$","link_ui.cgi");
      string heading = "id| name| url| page-url ";
      emitHeadings(heading);
      getData( sql );
      emitData();
      emitEnd();
      return breturn;
    }
};
/*
  Links Form 
    - Glues the business object to the form object through 
      multiple inheritance. 
*/
 
               
class links_Form:  public link, public forms_base
{
public: 
  // Constructor
  links_Form(cgiScript & script):link(),forms_base(script)
  {
    setKey(*this);
    site_id = atoll(currentSite().c_str());
  }
  ~links_Form(){;}
  
  // Implement pure virtual methods of form_base
  void form_id_transfer( void )
  {
    llongFXfer( "id", id );
  }
  
  void form_data_transfer( void )
  {   
    llongFXfer( "site_id", site_id );
    stringFXfer( "name", name );
    stringFXfer( "url", url );
    stringFXfer( "target", target );
  }
  
  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  }
   
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    script << makeTop("link_ui.cgi", "LINKS")
           << formTemplate.getParagraph("advanced_begin")
           << makeStaticBox("Id","id",id,"12" ) 
           << "<br class='clearall'>" << endl
           << makeHiddenBox("site_id",site_id )
           << makeTextBox("Target","target",target,"225", "25")
           << formTemplate.getParagraph("advanced_end")
           << "<br class='clearall'>" << endl
           << makeTextArea("Name *","name",name,"3","45")
           << "<br class='clearall'>" << endl
           << makeTextBox("URL *","url",url,"255","50")   
           << "<br class='clearall'>" << endl 
           << makeButtons( key() );             
    script << makeBottom( m_result ) << endl; 
    return breturn;
  }
};


int main( int argc, char ** argv )
{  
  cgiScript script( "text/html", false );
  links_Form myFrm(script); 

  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/childPane.htmp");
    
    script << ocString(pgTemplate.getParagraph("top"))
            .replaceAll("$heading$","links");

    myFrm.loadControlTemplates("Templates/childdivform.htmp");  
    myFrm.form_action();  
    myFrm.form_display();
    script << "<br class='clearall'>" << endl; 
    links_List linksList(script);
    linksList.loadListTemplates("Templates/list.htmp");
    script << "<div class=\"ui\">" << endl;
    script << "<div class='heading'>Links List:</div>" << endl;
    linksList.list_display();
    
    
    script << pgTemplate.getParagraph("bottom");
  }
  else
  {
    script.Redirect("signIn.html"); 
  }  
};

// compile implementations here
#include "read_write_base.cpp"
#include "forms_base.cpp"

