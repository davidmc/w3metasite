/*
  Newsletter Form
  ===================================
  CGI User Interface for Pay Drop Newsletter
*/
#include <iostream>
#include <iomanip>
#include "ocTypes.h"
#include "ocString.h"
#include "connectstring"
#include "read_write_base.hpp"
#include "forms_base.hpp"
#include "list_base.hpp"
#include "cgiTemplates.h"
#include "Newsletter.hpp"
#include "nav_menu.h"
#include "siteLimit.h"
#include "mail_control.hpp"

/*
  Newsletter Form - Glues the business object to the form object through 
                    multiple inheritance.
*/
class Newsletter_Form:  public Newsletter, public forms_base
{
public: 
  // Constructor
  Newsletter_Form(cgiScript & in):Newsletter(),forms_base(in)
  {    
    setKey(*this);
    // Also set the Newsletter site id
    // establish cookies
    cgiCookie cookie;
    // cookies valid for entire site
    cookie.setPath("/"); 
    // check for cookie
    string site =  cookie.get("site_id");
    if(site.length())
    {
      site_id = atoll(site.c_str() );
    }
  }  
  ~Newsletter_Form(){;}
  // Implement pure virtual methods of form_base
  void form_id_transfer( void )
  {
    llongFXfer( "id", id );
  }
  void form_data_transfer( void )
  {    
    llongFXfer( "page_id", page_id );
    stringFXfer( "address_query",  address_query);    
  } 
  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  } 
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    string page_sql = "select id, name from pages where site_id ";
    page_sql += siteFocus(oLogin);    
    
    // for newsletter run link
    ocString link = "<a href='newsletter_ui.cgi?do=newsletter&id=%id'>";
    string stat = m_result;
    if( address_query.length() )
    {
      ocString Id;
      Id.append(id);
      stat += "<br>";      
      stat += link.replace("%id",Id.c_str());
      stat += "RUN THE NEWSLETTER</a>";
    }
    
    script << makeTop( "newsletter_ui.cgi", "Newsletter" )
           << makeTextBox( "Newsletter ID","id",id,"12" ) 
            << makeComboBox( "Newsletter Page", "page_id", page_id, 
                             page_sql )                            
           << makeTextBox("Contact List SQL:<br>"
                          "Must have id, followed by email address","address_query",address_query,"255", "55")
           << makeButtons( key() )
           << makeBottom( stat ) << endl;            
    return breturn;
  }      
};

class Newsletter_List:  public list_base
{
public: 
  // Constructor
  Newsletter_List(cgiScript&sc):list_base(sc)
  {
    ;                   
  }  
  ~Newsletter_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;
    string news_sql = "select newsletter.id, pages.name from newsletter "
                      "inner join pages on newsletter.page_id = pages.id where newsletter.site_id ";
                      
    news_sql += siteFocus(oLogin);    
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","newsletter_ui.cgi");    
    hotCol=1;  
    emitHeadings("Id|Page"); 
    getData( news_sql );
    emitData();    
    emitEnd();
    
    return breturn;
  }
  void derived_data_event( void )
  {  
      
  }
        
};
void doNewsLetter(cgiScript & script)
{
  cgiInput & args = script.ClientArguments();
  quickQuery qry;
  openRS & rs = qry.getRS();
  if( args.count("do") && args.count("id") && args["do"] == "newsletter" )
  {
    script << "<h3>Running Newsletter...</h3>";
    Newsletter letter;
    string ID = args["id"].c_str();
    if( ID.length() )
    {
      long long nid = atoll(ID.c_str());
      letter.key(nid);
      if( letter.get_data() )
      {
        ocString pgUrlSql = "select url from pages where id  = ";
        pgUrlSql.append( letter.page_id );
        if( rs.open(pgUrlSql) )
        {
          string page_url = rs.getField(0).format();
          rs.close(); 
          for( bool opened = rs.open(letter.address_query);
              opened;
              opened = rs.next())
          {

            script << rs.getField(1).format() << " " << endl;
            sendMail( rs.getField(1).format(), 
                      "newsletter@paydrop.com",
                      "", // When subject left blank pulls from page meta_description 
                      page_url,
                      script.ServerName(),
                      rs.getField(0).format() ); 
          }
          script << "<h4>Finished Running Newsletter</h4>";           
        }
      }
    } 
  } 
}    
int main( int argc, char ** argv )
{    
  cgiScript script( "text/html", false );
  Newsletter_Form  marktr(script); 
  Newsletter_List mylist(script); // so you can set cookies 
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/adminPane.htmp");
    
    script << pgTemplate.getParagraph("top");

    marktr.loadControlTemplates("Templates/form.htmp");  
    marktr.form_action();  
    marktr.form_display();
    mylist.loadListTemplates("Templates/list.htmp"); 
    script << "<p>&nbsp;</p>" << endl;
    mylist.list_display();
    
    // OK now check to see if we need to send the news later
    doNewsLetter(script);  
    
    script << pgTemplate.getParagraph("bottom");
  }
  else
  {
    script.Redirect("signIn.html"); 
  } 
};

// as an alternative to building more objects
#include "read_write_base.cpp"
#include "forms_base.cpp"

