/**
 
  contactus.cpp    
  ==============================================
  main file for contactus.meta
  In addition to standard page tags, this requires a control item:
    - contactus
  elements:
    - mailform,
      mailbodystart,
      mailbodyitem,
      mailbodyend,
      thankyou,
      sendto
      
  Note: form data items must start with mi(1,n)_
      (1,n) is an ordering number from 1 to as big as you need it
      (be aware that an alpha sort is going on in a map, so 11 goes before 2!)
      examples: <input name='mi1_First Name'> <input name='mi2_Address 1'>
      Case Sensitive!
  Copyright (c) 2004 - David K. McCombs w3sys.com
  ===============================================
  
*/
//#define OPEN_LOGGING_FILE
#include "openLogger.h"
#include "cgiClass.h"
#include"oc_Mail.h"
#include "connectstring"
static char * mailPath = "127.0.0.1";
string baseUrl;

#include "page.hpp"
#include "page_control.hpp"
using namespace std;


class mail_functor: public base_functor
{
public:
  cgiScript & script;
  page & pg;    
  ocSendMail sendMail;
    
  string bodyStart;    // Template item, for mail body
  string itemTemplate; // Template data element item, hardwired replacement items: $label, $data
  string bodyEnd;      // Template item, for mail body  
  string formName;     // Expect a form by the given name as a paragraph on the contact page
  string thankYou;     // Expect a response paragraph of the given name, 
                       //   optional $message, replaced by a metasite paragraph of the same tag name
                       //   optional $data item replace tag
  string sendTo;       // Used for both both the recipient and the sender
  
  string mailBody;     // the fully formed mail body comprised of bodyStart, listTemplate and bodyEnd.
  string dataSent;     // just the data items formatted with 'itemTemplate'
  
  mail_functor(page & ipg,cgiScript & scriptIn):base_functor(ipg)
  ,script(scriptIn),pg(ipg),sendMail(mailPath){;}
  virtual ~mail_functor(){;}
  
  virtual  bool operator()( xmlNode & node )
  {
    bool bRet = true;
          
    node_attr::iterator x = node.attr.find("items");
    if( x!=node.attr.end() )
    {
      formName=x->second;
    }
    x = node.attr.find("mailbodystart");   
    if( x!=node.attr.end())
    {
      bodyStart=x->second;
    }
    x = node.attr.find("mailbodyitem");   
    if( x!=node.attr.end())
    {
      itemTemplate=x->second;
    }
    x = node.attr.find("mailbodyend");   
    if( x!=node.attr.end())
    {
      bodyEnd=x->second;
    }
    x = node.attr.find("thankyou");   
    if( x!=node.attr.end())
    {
      thankYou=x->second;
    }
    x = node.attr.find("sendto");   
    if( x!=node.attr.end())
    {
      sendTo=x->second;
    }
    // now determine what to do
    if( script.RequestMethod().upper() == "GET" )
    {
      // show the form
      // script << "<h5>Showing Form!</h5>" << endl;  // DBG
      showForm();
    }
    else
    {
      // send the mail
      // as added security, insure form is from our domain.
      // script << "<h5>Sending Mail!</h5>" << script.HttpReferer()  << endl;  // DBG
     
      doMail();      
    }
    return bRet;    
  }

  void showForm( void )
  {
    script << "<!-- form name = '" << formName << "' -->" << endl;
    pg.emitContent( formName, "$content" );
  }
  
  void doMail( void )
  {
    dataSent = "";
    mailBody = pg.paragraph_string(bodyStart);
    queryStringMap::iterator pos;
    cgiInput & args = script.ClientArguments();
    string from = sendTo.c_str();
    string to = sendTo.c_str();
    ocString testForEmail;
    for(pos=args.TheMap().begin(); pos!=args.TheMap().end(); ++pos)
    {        
       testForEmail = pos->first;       
       if( testForEmail.regExMatch( "[eE][mM][aA][iI][lL]" ) && pos->second.length() )
       {
         from = pos->second.c_str();
       }
       dataItem( pos->first, pos->second );
    }
    mailBody += pg.paragraph_string(bodyEnd);
    sendMail.openRoute( from.c_str(), to.c_str(), "Customer Request" );
    sendMail.setMimeType("text/html");
    sendMail.write( mailBody.c_str() );
    sendMail.send();
    script <<  thanksMsg()<< endl; 
  }
  ocString find( page_paragraphs & paras, string name )
  {
    ocString output;
    // script << "Looking for " << name << endl;// DBG 
    for( int pos = 0; pos < paras.size(); ++pos )
    {
      // script << "<br>scanning " << paras[pos].name() << "<br>" << endl; // DBG
      if( paras[pos].name() == name )
      {
        output = paras[pos].content();
        break;
      }
    }
    // if( !output.length() ) script << "Couldn't find " << name << endl;// DBG 
    return output;
  }
  string thanksMsg( void )
  {
    string message = find( pg.get_page_paragraphs(), thankYou); 
    ocString thanksMsg = pg.paragraph_string(thankYou);                         
    return thanksMsg.replace("$data",dataSent.c_str()).replace("$message",message.c_str());  
  }
  
  void  dataItem(  ocString name, 
                   aString value )
  {
    // look at the name to be sure it's a form data item
    if( name.substr(0,2)=="mi")
    {    
      name.parse("_"); // get rid of mi(n)_  
      ocString datum = pg.paragraph_string(itemTemplate);    
      string formatted = datum.replace("$label",name.remainder().c_str()).replace("$data",value.c_str());
      mailBody += formatted;
      dataSent += formatted;
    }
  }
};


// Standard main function
int main(int argc, char ** argv)
{  
  baseUrl = "contactus.meta";  // this is my compiled name!
  cgiScript script;
  page pg(script);
  // load the data
  if( pg.load() )
  {
    page_control ctrl(pg);     
    ctrl.addOp ( "contactus",  new mail_functor(pg,script) );
    // emit the form, or the send acknowledment
    ctrl.emit();
  }
  else
  {
    script << "<html><head><title>Page Unavailable</title></head>" << endl
           << "<body><h1>Sorry</h1>" << endl
           << "<p>The page you requested is not available</p></body></html>";  
  }  
  return 0;
}
