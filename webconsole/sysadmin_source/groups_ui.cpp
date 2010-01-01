/*
  groups Form
  ===================================
  CGI User Interface for groups
            
*/
#include "metaconnectstring"
#include <iostream>
#include <iomanip>
#include "ocTypes.h"
#include "ocString.h"
#include "read_write_base.hpp"
#include "forms_base.hpp"
#include "list_base.hpp"
#include "cgiTemplates.h"
#include "groups.hpp"
#include "openLogin.h"
openLogin oLogin;

class services_List:  public list_base
{
public: 
  long long group_id;
  
  // Constructor
  services_List(cgiScript&sc, long long id):list_base(sc),group_id(id)
  {
    ;                   
  }  
  ~services_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","group_services_ui.cgi");
    hotCol=1;    
    ocString sql= "select sp.id, s.name from group_services sp "
                "inner join services s on s.id = sp.service_id where sp.group_id = "; 
    sql.append(group_id);
    emitHeadings("Id|Service Name"); 
    getData(sql);
    emitData();    
    emitEnd();
    return breturn;
  }
  void derived_complete_event( void )
  {  
    ocString loco = "group_services_ui.cgi";
    if( group_id)
    {
      loco += "?group_id=";
      loco.append(group_id);
    }
    ocString adder = listTemplate.getParagraph("list_new");
    webIO << adder.replace("$cols","2")
                  .replace("$location",loco.c_str())
                  .replace("$item","Service");
  } 
  
  // override this to get the group_id in the link
  virtual void sendHotField( int iField, ocString & td )
  {
    ocString linkId = rs.getField(0).format();
    if( group_id)
    {
      linkId += "&group_id=";
      linkId.append(group_id);
    }
    webIO <<  td.replace( "$data$",
                    editLink.replaceAll( "$key$", linkId.c_str() )
                            .replaceAll( "$col$", rs.getField(iField).format().c_str() ).c_str());
  }
};
/*
  groups Form 
    - Glues the business object to the form object through 
      multiple inheritance.      
                  
*/                 
class groups_Form:  public groups, public forms_base
{
public: 
  // Constructor
  groups_Form(cgiScript & script):groups(),forms_base(script){setKey(*this);}  
  ~groups_Form(){;}
  // Implement pure virtual methods of form_base
  void form_id_transfer( void )
  {
    llongFXfer( "id", id );
  }
  void form_data_transfer( void )
  {     
    stringFXfer( "name", name );
    // stringFXfer( "domain", domain );
    // stringFXfer( "url", url );
    stringFXfer( "role_id",  group_roles);
    boolFXfer( "site_admin", site_admin );    
  }  
  
  bool isupplemental( void )
  {
    return setRoles();
  }  
  // event fired after update
  bool usupplemental( changeMap & changes )
  { 
    bool bRet = deleteRoles();
    if(bRet)  bRet = setRoles();
    return bRet; 
  }  
  // event fired after delete
  bool dsupplemental( void )
  { 
    return deleteRoles(); 
  }
  // event fired after gettring data
  bool ssupplemental( void )
  {
    return retrieveRoles();
  } 
  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  } 
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    script << makeTop("groups_ui.cgi", "GROUPS")
           << makeStaticBox("Id","id",id,"12" )     
           << makeTextBox("Name","name",name,"24")
           // << makeTextBox("Domain","domain",domain,"255","50")
           // << makeTextBox("Site URL","url",url,"255","50")     
           << makeBoolBox("Site Administrators?","site_admin", site_admin) 
           << makeComboBox("Roles","role_id",group_roles,
                           "select id,  name from roles", 
                           "", " multiple='multiple' size='6'" )                                                                                  
           << makeButtons( key() );
    if ( id != 0 )
    {
      script << "<tr><td colspan='2'>";
      services_List svclist(script,id);
      svclist.loadListTemplates("Templates/poplist.htmp");  
      svclist.list_display();
      script << "<b>GROUP Services:</b></td></tr>";
    }           
    script << makeBottom( m_result ) << endl; 
    return breturn;
  }      
};


class groups_List:  public list_base
{
public: 
  // Constructor
  groups_List(cgiScript&sc):list_base(sc)
  {
    ;                   
  }  
  ~groups_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","groups_ui.cgi");
    hotCol=1;    
    
    emitFilter( "groups_ui.cgi",  // program name
                "&nbsp;&nbsp;&nbsp;&nbsp;<B>FILTER</B>" ); // Label
    
    emitHeadings("Id|Group Name"); 
            
    getFilteredData( "id, name", 
                     "groups", 
                     "name like '$filter$%'", 
                     "name" );     
    emitData();
    
    emitNavigation("groups_ui.cgi");
    
    emitEnd();
    return breturn;
  }
  void derived_data_event( void )
  {  
      
  }
        
};

int main( int argc, char ** argv )
{  
  cgiScript script( "text/html", false );
  groups_Form myFrm(script); 
  groups_List mylist(script); // so you can set cookies 
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/adminPane.htmp");
    
    script << ocString(pgTemplate.getParagraph("top"))
            .replaceAll("$heading$","groups");

    myFrm.loadControlTemplates("Templates/form.htmp");  
    myFrm.form_action();  
    myFrm.form_display();
    
    
    mylist.loadListTemplates("Templates/list.htmp");  
    mylist.list_display();
    
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


 
