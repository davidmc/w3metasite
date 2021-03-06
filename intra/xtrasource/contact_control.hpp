/* 
  ContactUs Control Class - contact_control.hpp
  ----------------------------------------  
  This class represents 
  an abstraction of some sort

  ==============================================
  functor definitions for contactus.meta
  In addition to standard page tags, this requires a control item:
    - contactus
  elements:
    - mailform, -- the container for the form, a typical content tag
      mailbodystart, mailbodyitem, mailbodyend, - all to create the html content for the actual mail
      thankyou,  -- the thank you response
      sendto   -- the sendto address

  Note: form data items must start with mi(1,n)_
      (1,n) is an ordering number from 1 to as big as you need it
      (be aware that an alpha sort is going on in a map,
      so 11 goes before 2 (solution: use 02)!)
      examples: <input name='mi1_First Name'> <input name='mi2_Address 1'>
      Case Sensitive!
  Copyright (C) 2004 - 2008 David K. McCombs w3sys.com
  =============================================== 
*/

// Sentry
#ifndef Contact_Us_Control_Hpp
#define Contact_Us_Control_Hpp

#include "Form_Data_ui.h"

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
  ,script(scriptIn),pg(ipg),sendMail("127.0.0.1"){;}
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
    string section;
    x = node.attr.find("section");
    if(  x!=node.attr.end() )
    {
      section = x->second;
    }
    // now determine what to do
    if( script.RequestMethod().upper() == "GET" )
    {
      // show the form
      // script << "<h5>Showing Form!</h5>" << endl;  // DBG
      if( isEditMode )
      {
        editLinksInit();
        fixEditLinks( formName,  pg);
      }
      showForm(section);
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

  void showForm( string & section )
  {
    script << "<!-- form name = '" << formName << " - " << section << "' -->" << endl;

    pg.emitContent( formName, "$content$", section );
  }
  
  void doMail( void )
  {
    // New 1/5/08 - Store a permanent record of the request
    //Form_Data_form permRecord(script);
    
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
       
       if(  dataItem( pos->first, pos->second ) )
       ;
       /*
       {
         permRecord.label_list += pos->first;
         permRecord.label_list += ",";
       }
       */
    }
    mailBody += pg.paragraph_string(bodyEnd);

    // DBG    script << " openRoute TO: " <<   to << " FROM: " << from << endl;
    /* DBG - dont send really   */
    sendMail.openRoute( from.c_str(), to.c_str(), "Customer Request" );
    // DBG    script <<  " ... write TO: " << mailBody << endl;
    sendMail.setMimeType("text/html");
    sendMail.write( mailBody.c_str() );
    // DBG    script << " .. Sending .. " << endl;
    sendMail.send();

    
    // Save the data  - good unfinished idea
    //permRecord.recipient = to;
    //permRecord.form_action();
    
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
  
  bool  dataItem(  ocString name, aString value )
  {
    bool ret = false;
    // look at the name to be sure it's a form data item
    if( name.substr(0,2)=="mi")
    {
      ret = true;  
      name.parse("_"); // get rid of mi(n)_  
      ocString datum = pg.paragraph_string(itemTemplate);    
      string formatted = datum.replace("$label",name.remainder().c_str()).replace("$data",value.c_str());
      mailBody += formatted;
      dataSent += formatted;
    }
    return ret;
  }
};

#endif
