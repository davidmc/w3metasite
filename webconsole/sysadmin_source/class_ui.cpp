/*
  XXXX Form
  ===================================
  CGI User Interface for XXXX
            
*/

#include <iostream>
#include <iomanip>
#include "ocTypes.h"
#include "ocString.h"
#include "read_write_base.hpp"
#include "forms_base.hpp"
#include "list_base.hpp"
#include "cgiTemplates.h"
#include "XXXX.hpp"
#include "openLogin.h"
openLogin oLogin;

/*
  XXXX Form 
    - Glues the business object to the form object through 
      multiple inheritance.      
                  
*/                 
class XXXX_Form:  public XXXX, public forms_base
{
public: 
  // Constructor
  XXXX_Form(cgiScript & script):XXXX(),forms_base(script){setKey(*this);}  
  ~XXXX_Form(){;}
  // Implement pure virtual methods of form_base
  void form_id_transfer( void )
  {
    llongFXfer( "",  );
  }
  void form_data_transfer( void )
  {
  
    llongFXfer( "",  );
    longFXfer( "",  );          
    stringFXfer( "",  );
    dateFXfer( "",  );     
    boolFXfer( "",  ); 
    doubleFXfer( "",  );   
  } 
  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  } 
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    script << makeTop("XXXX_ui.cgi", "XXXX")
           << makeTextBox("Label goes here","membervar",,"100")
           << makeStaticBox("Label goes here","membervar",,"12" ) 
           << makeComboBox("Label goes here", "membervar", , 
                           "select ID, Name from XXXX order by Name" )                            
           << makeTextArea("Label goes here","membervar",  )
           << makeDateBox("Label goes here", "membervar", , "12")
           << makeStateBox("State","membervar",)  
           << makeBoolBox("?","", )                                                                       
           << makeButtons( key() )
           << makeBottom( m_result ) << endl; 
    return breturn;
  }      
};

class XXXX_List:  public list_base
{
public: 
  // Constructor
  XXXX_List(cgiScript&sc):list_base(sc)
  {
    ;                   
  }  
  ~XXXX_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","XXXX_ui.cgi");
    hotCol=1;    
    
    emitFilter( "XXXX_ui.cgi",  // program name
                "&nbsp;&nbsp;&nbsp;&nbsp;<B>FILTER</B>" ); // Label
    
    emitHeadings("Id|XXXX Name"); 
            
    getFilteredData( "id, name", 
                     "XXXX", 
                     "Name like '$filter$%'", 
                     "Name" );     
    emitData();
    
    emitNavigation("XXXX_ui.cgi");
    
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
  XXXX_Form myFrm(script); 
  XXXX_List mylist(script); // so you can set cookies 
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/adminPane.htmp");
    
    script << ocString(pgTemplate.getParagraph("top"))
            .replaceAll("$heading$","XXXX");

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


 
