/*
  Page class - page.hpp
  
  This class represents 
  an abstraction of a web page.      
*/

// sentry
#ifndef Web_Page_Hpp
#define Web_Page_Hpp

// includes
#include "read_base.hpp"
#include "openLogin.h"


// Globally defined
openLogin oLogin;
string baseUrl;
bool isSignedOn;
bool isEditMode;


#include "pg_handlers.hpp"
#include "page_template.hpp"
#include "page_menu.hpp"
#include "pg_paragraph.hpp"
#include "global_content.hpp"

class page : protected read_base 
{
protected:
  string getSql( string url, string domain )
  {
    // load page from lookup url
    sql = "select p.id, "
          "p.site_id, "
          "p.template_id, "
          "p.name, "
          "p.url, "
          "p.meta_description "
          "from pages p "
          "inner join sites s on p.site_id = s.id "
          "where concat(s.url,p.url) = '";
    sql+= url + "' and s.domain = '" ;
    sql+= domain;
    sql+= "'";        
    return sql;                
  }
      
  void propset( void )
  {
    m_id = atoll(rs.getField(0).format().c_str());
    m_site_id = atoll(rs.getField(1).format().c_str());
    m_template_id = atoll(rs.getField(2).format().c_str());
    m_name = rs.getField(3).format();
    m_url = rs.getField(4).format();
    m_meta_description = rs.getField(5).format();
  }  

  // Properties
  long long m_id;               // auto_increment key
  long long m_site_id;          // the id to the containing web site
  long long m_template_id;      // the id to the page template
  string    m_name;             // page name
  string    m_url;              // relative location
  string    m_meta_description; // description 
  string    sql;   
  
  // Derived property
  bool defaultPage;
  
  // Web Interface
  cgiScript & script;  
  
  // Template object 
  page_template pg_template;
  
  
  // Menu list
  page_menus pg_menus;
    
  // Paragraph list  
  page_paragraphs pg_paragraphs;
  
  // site global contents
  global_contents m_global_contents; 
  ocString loadResults;
public:

  // Single icon type for top, bottom and spacer
  string editIcon;
  

  page(cgiScript & in)
  :read_base(),script(in)  
  ,pg_paragraphs(db),m_id(0),m_site_id(0),m_template_id(0)
  ,defaultPage(false)
  {
    ;
  }
  virtual ~page()
  {
    ;
  }
  string getLoadResult( void )
  {
    return loadResults;
  }
  virtual bool load( void )
  {
    bool breturn = false;

    // Create relative url lookup
    // concatenate pages.meta and path info to lookup url    
    string relUrl = script.ScriptName();    
    relUrl += script.PathInfo().c_str();
    loadResults = "Attempt to get page ";
    loadResults += getSql(relUrl,script.ServerName().c_str());
    loadResults += " failed";

    if( script.PathInfo().length() == 0 )
    {
      defaultPage = true;
    } 

    if( rs.open( sql.c_str() ) )
    {
      // set properties
      propset();
      rs.close(); // I'm done with this recordset
      
      // load template from page template_id, 
      //   literally loads the template from a file.
      breturn = pg_template.load(template_id());
      if( !breturn )
      {
        loadResults = "Attempt to load template failed<br>  ";
        loadResults += pg_template.errors();
      }
      else
      {
        loadResults = "Good!";
        // load menus from page id
        pg_menus.load(id());
        
        // load page paragraphs from page id
        pg_paragraphs.load(id(),script.ClientArguments() );
        
        // load any site global content
        m_global_contents.load(site_id());
      }
    }
    return breturn;
  }

  void addGlobalContent( string tag, ocString & part )
  {
    global_content_map::iterator pos = m_global_contents.find(tag);
    if( pos !=  m_global_contents.end() )
    {
      // the replacement tag for global content is ALWAYS $content$
      part = part.replace( "$content$", pos->second.content().c_str() );
    }
  }
  // new functions for template controled emission
  virtual string getTop( void )
  {
    string strRet = "";
    // if template loaded
    if( pg_template.id() != 0 )
    {
      // emit 'top'
      ocString top = pg_template.getParagraph("top");      
      addGlobalContent( "top", top );      
      strRet = top.replace( "$meta_description$",
                             m_meta_description.c_str() )
                  .replace( "$title$", m_name.c_str() );
    }
    return strRet;
  }
  virtual string getSpacer( string name )
  {
    ocString spacer = paragraph_string(name);
    addGlobalContent( name, spacer );
    return  spacer.replace( "$title$", m_name.c_str() );
  }
  virtual string getEnd( void )
  {
    // the 'end'
    ocString end = pg_template.getParagraph("end");
    addGlobalContent( "end", end );
    return end;
  }

  virtual bool emitMenu( string name, string startTag, string endTag,
                         string boundStart = "", string boundEnd="" )// 12/6/06 new for dynamic menus
  {
    bool bRet = true;
    // DBG  cerr << "PAGE EMIT MENU : " <<  name << endl;
    pg_menus.emit( name, pg_template.getParagraphs(), 
                   script, script.PathInfo().c_str(), 
                   defaultPage,
                   startTag, endTag, boundStart, boundEnd);
    return bRet;
  }
  
  virtual bool emitContent( string contentTags, string replaceTag, string section = "" ) 
  {
    bool bRet = true;    
    paragraphMap & paras  = pg_template.getParagraphs();
    
    pg_paragraphs.emit(paras,script,contentTags,replaceTag,section);    
    return bRet;                  
  }   
  
  
  // Property Access Methods
  // Gets
  long long id( void ){return m_id;}       
  long long site_id( void ){return  m_site_id;}    
  long long template_id( void ){return  m_template_id;}
  string    name( void ){return  m_name;}       
  string    url( void ){return  m_url;}  
  string    meta_description( void ){return  m_meta_description; }
  
  // static, valid only after load function 
  string loaded_control_string( void ) 
  { 
    
    if( pg_template.id() == 0 || pg_template.getParagraphs().empty() )
    {
      return "<xml>Empty Template!</xml>"; 
    }
    paragraphMap & paras  = pg_template.getParagraphs();
    paragraphMap::iterator pos = paras.find(string("control"));

    if( pos != paras.end() )
    {
      return pos->second;
    }
    else
    {
      return "<xml>Can't Find control!</xml>"; 
    }      
  }
  
  /*
    Return's static template paragraph.
    Valid only after load function. 
    Paragraph From the page Template, not to be confused with page paragraph.
  */  
  string paragraph_string( string name ) 
  {     
    if( pg_template.id() == 0 || pg_template.getParagraphs().empty() )
    {
      return "<xml>Empty Template!</xml>"; 
    }
    paragraphMap & paras  = pg_template.getParagraphs();
    paragraphMap::iterator pos = paras.find(name);

    if( pos != paras.end() )
    {
      return pos->second;
    }
    else
    {
      string err =  "<error>Can't Find named paragraph!</error><detail>";
      err += name;
      err += "</detail>";
      return err; 
    }      
  }
   
  // get the database paragraph content
  page_paragraphs & get_page_paragraphs( void )
  {
    return pg_paragraphs;
  }
  
  // get the file template abstraction
  page_template & get_template( void )
  {
    return pg_template;
  }
  
  // Sets      
  void id( long long in ){m_id=in;}       
  void site_id( long long in ){m_site_id=in;}    
  void template_id( long long in ){m_template_id=in;}
  void name( string in ){m_name=in;}       
  void url( string in ){m_url=in;}
  void meta_description ( string in ){m_meta_description=in; } 
  
  cgiScript & get_script(void) { return script; }
  page_menus & get_menus(void) { return pg_menus; }
};

// end sentry
#endif
