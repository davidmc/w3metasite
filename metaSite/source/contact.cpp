/*
 
  contact.cpp    
  
  OLD STUFF DON'T USE
  DKMc 12/28/04
  
*/
#include "cgiClass.h"
#include"ocMail.h"

static char * mailPath = "/usr/sbin/sendmail";
string baseUrl;

#include "page.hpp"
using namespace std;


class mailpage: public page
{
  bool sentMail;
  string mailBody;
  ocSendMail sendMail;
  bool parseItem(  const char * name, 
                   const char * prefix = NULL, 
                   const char * suffix = NULL )
  {
    bool bRet = false;
    if( script.ClientArguments().count(name) &&
        script.ClientArguments()[name].length() )
    {
      string var = script.ClientArguments()[const_cast<char *>(name)].c_str();
      if( var.length() > 0 )
      {
        if( prefix && strlen(prefix) )
        {
          mailBody += prefix;
        }
        mailBody += var;
        if( suffix && strlen(suffix) )
        {
          mailBody += suffix;
        }
        bRet = true;
      }
    }
    return bRet;
  }
  
  void parseForm(void)
  {
    mailBody += "<h2>Customer Request:</h2>\n<hr>\n";
    mailBody += "<table>\n";
    mailBody += "<tr><th>Customer Information</th><td> ... </td></tr>\n";
    parseItem( "Name","<tr><th>Name:</th><td>", "</td></tr>" );
    parseItem( "Title","<tr><th>Title:</th><td>", "</td></tr>" );
    parseItem( "Company","<tr><th>Company Size:</th><td>", "</td></tr>" );   
    parseItem( "Address","<tr><th>Address 1:</th><td>", "</td></tr>" );    
    parseItem( "City","<tr><th>City:</th><td>", "</td></tr>" );
    parseItem( "State","<tr><th>State:</th><td>", "</td></tr>" );
    parseItem( "Zip","<tr><th>Zip:</th><td>", "</td></tr>" );
    parseItem( "Phone","<tr><th>Phone:</th><td>", "</td></tr>" ); 
    parseItem( "Fax","<tr><th>Fax:</th><td>", "</td></tr>" );  
    parseItem( "Email","<tr><th>Email:</th><td>", "</td></tr>" );        
    parseItem( "Request",
               "<tr><th>Request:</th><td class='request'><pre>", 
               "</pre></td></tr>" );
    mailBody += "</table>\n";
  }
public:
  mailpage(cgiScript & script):page(script),sendMail(mailPath)
  {
  
    sentMail = false;
  }
  virtual ~mailpage() {;}
  
  void send( void )
  {
    if( script.RequestMethod().upper() == "POST" )
    {
      sentMail = true; 
      // parse the form to make up the mail body
      parseForm();
      sendMail.openRoute( script.ClientArguments()["To"].c_str(),
                          script.ClientArguments()["To"].c_str(), //
                          "Customer Request" );
      sendMail.setMimeType("text/html");
      sendMail.write( "<html><head>");
      sendMail.write( "<style>\n"
                      "body{ background: #f5f5f0; color: navy;}\n"
                      "td.request{ background: white; color: #000087; padding: 5px;}\n"
                      "pre{ font: normal 10pt arial; }\n"                      
                      "h1, h2{ color: #000087; }\n"                     
                      "</style>"
                      "</head>" );
      sendMail.write( "<body>" );
      sendMail.write( mailBody.c_str() );
      sendMail.write( "</body></html>");
      sendMail.send();
      
    }
  }
  virtual bool emit( void )
  {
    bool bRet = emitTop();
    // if template loaded
    if( bRet )
    {   
      // get the map of all parsed (template) paragraphs
      // paragraphMap & paras = pg_template.getParagraphs();
                
      // emit menus
      emitMenu( "menu1_start", "menu1_end" );
      emitMenu( "menu2_start", "menu2_end" );

      // emit the mail form, or the acknowledgment
      bRet = emitFormOrAck();
            
      // emit 'end'
      emitEnd();
      
    } 
    else
    {
      script << "The page you requested is not available" << endl;
      bRet = false;
    } 
    return bRet;
  }
  virtual bool emitFormOrAck( void )
  {
    bool bRet = true;
    if( sentMail )
    {
      script << pg_template.getParagraph("Thanks") << endl;      
    }
    else
    {
      // Template object 
      cgiTemplates states;
      states.load("Templates/states.html");
      ocString form(pg_template.getParagraph("Form"));      
      script << form.replace("$state_field$",
                             states.getUnparsedHtml().c_str()) << endl;
    }  
    return bRet;
  }
};
// Standard main function
int main(int argc, char ** argv)
{  
  baseUrl = "contact.meta";  // this is my compiled name!
  cgiScript script;
  mailpage pg(script);
  // load the data
  pg.load();
  // perform mail send
  pg.send();
  // emit the form, or the send acknowledment
  pg.emit();
    
  return 0;
}
