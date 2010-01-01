/* 
  page_enhancements.hpp
  ----------------------------------------  
  This class holds enhancements to 
  the base metasite functionality
 
*/

// Sentry
#ifndef Page_Enhancements_Hpp
#define Page_Enhancements_Hpp

#include <sstream>
#include "ocString.h"
#include "ocXML.h"

/*
  Emit content with paragraph name related to the search string
  The paragraph name is a regular expression
*/
class qrystr_content_functor: public base_functor
{
protected:
  cgiScript & script;
public:
  qrystr_content_functor(page & ipg,cgiScript & in):base_functor(ipg),script(in){;}
  virtual ~qrystr_content_functor(){;}
  
  virtual  bool operator()( xmlNode & node )
  {  
    bool bRet = true;
    
    string items, replace_tag;        
    node_attr::iterator x = node.attr.find("items");
    if( x!=node.attr.end() )
    {
      items=x->second;
      x = node.attr.find("replace-tag");
      if( x==node.attr.end())
      { // try this
        x = node.attr.find("replace_tag"); 
      }  
      if( x!=node.attr.end())
      {
        string section;
        x = node.attr.find("section");
        if(  x!=node.attr.end() )
        {
          section = x->second;
        }
        replace_tag=x->second;
        paragraphMap & paras  = pg.get_template().getParagraphs();
        emit( paras, items, replace_tag, section ); 
      }
      else
      {
        cout << "no content replace tag" << endl;
      }
    }
    else
    {
      cout << "no content items" << endl;
    }
    return bRet;    
  }
  // Enum paragraphs, conditionally emit
  void emit( paragraphMap & paras, ocString templateTags, string replace_tag, string & section )
  { 
    paragraph_vector::iterator pos;
    int paragraphCount = 0;
    page_paragraphs & pgcontent = pg.get_page_paragraphs();
    for( pos=pgcontent.begin(); pos!=pgcontent.end(); ++pos )
    {
      if( pos-> section().length() &&
          section == pos->section() &&
          pos->approved() == true )
      {
        string pName = pos->name(); // a regular expression  
        ocString pQ =  script.QueryString().c_str();

        if( pQ.regExMatch(pName.c_str()) )
        {
          pos->emit(paras,script,replace_tag,paragraphCount);
          paragraphCount++;
        }
      }
    }
    if( isEditMode && paragraphCount == 0 )
    {
      // emit an empty paragraph so they can edit
      page_paragraph emptyOne;
      emptyOne.site_id(atoll(currentSite().c_str()));
      emptyOne.page_id(pg.id());
      emptyOne.section(section);
      emptyOne.template_tag(templateTags.parse(","));
      emptyOne.replace_tag(replace_tag);
      emptyOne.approved(true);
      emptyOne.emit(paras,script,replace_tag,paragraphCount);
    }
  }    
};

#ifdef OPEN_LOGIN_H
/* 
  Emit content with paragraph name related to the a list of roles
  The paragraph name is a regular expression
  
*/
class role_content_functor: public base_functor
{
  protected:
    cgiScript & script;
    string roles; // a regular expression
  public:
    
    role_content_functor(page & ipg,cgiScript & in):base_functor(ipg),script(in)
    {
      roles="[";
      string sql = "select role_id from user_roles where user_id = ";
      sql += oLogin.Id();
      quickQuery qq;
      openRS & rs = qq.getRS();
      for( bool isOpen = rs.open(sql); isOpen; )
      {
        roles+=rs.getField(0).format();
        isOpen = rs.next();
        if( isOpen )  roles+=",";
      }
      roles+="]";
    }
    virtual ~role_content_functor(){;}

    virtual  bool operator()( xmlNode & node )
    {
      bool bRet = true;
      

      string items, replace_tag;
      node_attr::iterator x = node.attr.find("items");
      if( x!=node.attr.end() )
      {
        items=x->second;
        x = node.attr.find("replace-tag");
        if( x==node.attr.end())
        { // try this
          x = node.attr.find("replace_tag");
        }
        if( x!=node.attr.end())
        {
          replace_tag=x->second;
          paragraphMap & paras  = pg.get_template().getParagraphs();
          string section;
          x = node.attr.find("section");
          if(  x!=node.attr.end() )
          {
            section = x->second;
          }
          emit( paras, items, replace_tag, section);
        }
        else
        {
          cout << "no content replace tag" << endl;
        }
      }
      else
      {
        cout << "no content items" << endl;
      }
      return bRet;
    }
  // Enum paragraphs, conditionally emit
    void emit( paragraphMap & paras, ocString templateTags, string replace_tag, string & section )
    {
      stringstream s;
      int paragraphCount = 0;
      page_paragraphs & pgcontent = pg.get_page_paragraphs();
      paragraph_vector::iterator pos;
      for( pos=pgcontent.begin(); pos!=pgcontent.end(); ++pos )
      {
        ocString pName = pos->name();
        if( pos-> section().length() &&
            section == pos->section() &&
            pos->approved() == true && pName.regExMatch(roles.c_str())
          )
        {
          pos->emit(paras,s,replace_tag,paragraphCount);
          ocString content = s.str();
          script << content.replaceAll("{{name}}",oLogin.FullName())
                           .replaceAll("{{last}}",oLogin.Last())
                           .replaceAll("{{first}}",oLogin.Last())
                           .replaceAll("{{Id}}",oLogin.Id())
                           .replaceAll("{{email}}",oLogin.Email())
                           .replaceAll("{{phone}}",oLogin.PhoneNumber());
         paragraphCount++;                  
        }
      }
    }
};
#endif

#ifdef BUILD_PARAGRAPH_MENU_HANDLERS
 /*
  This handler is installed if the menu template has 
  paragraph_key and paragraph_item attributes.
  
  paragraph_level is optional, indicating at what level the
  handler begins inserting paragraph links in the menu.
*/  

class paragraph_menu_handler: public base_pg_handler
{  
public:
  long level;
  paragraph_menu_handler(page & ipg):base_pg_handler(ipg),level(0L){;}
  virtual ~paragraph_menu_handler(){;}
  
  virtual  bool operator()( void )
  {     
    page_paragraphs & ps = pg.get_page_paragraphs();
    page_menus & ms = pg.get_menus();    
    
    if( ms.get_sel_item().place_level() >= level )
    {    
      paragraph_vector::iterator pos;
      for( pos=ps.begin(); pos!=ps.end(); ++pos )
      {  
        // a - at the beginning will signify 'subtract this from the list'
        if( pos->name()[0] != '-' )
        {
          // Archtype: <a class="navl$level" href="$location">$label</a>
          ocString link = "#paragraph_"; // # + (exactly as paragraph id is built)
          link.append( pos->place_order());
          ocString newLevel;
          newLevel.append(ms.get_sel_item().place_level()+1);
          pg.get_script() << 
            uiTemplate.replace("$level", newLevel.c_str())
                      .replace("$location",link.c_str())
                      .replace("$label",pos->name().c_str()) << endl;
        }            
      }
    }
    return true;  
  }
};
#endif

#endif 
