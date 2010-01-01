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
        replace_tag=x->second;
        paragraphMap & paras  = pg.get_template().getParagraphs();
        emit( paras, items, replace_tag ); 
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
  void emit( paragraphMap & paras, ocString templateTags, string replace_tag )
  {  
    page_paragraphs & pgcontent = pg.get_page_paragraphs();
    paragraph_vector::iterator pos;
    for( pos=pgcontent.begin(); pos!=pgcontent.end(); ++pos )
    {  
      ocString tags = "," + templateTags + ",";
      string testTag = "," + pos->template_tag() + ",";
       
      string pName = pos->name(); // a regular expression  
      ocString pQ =  script.QueryString().c_str();
     
      if( tags.regExMatch( testTag.c_str() ) && pQ.regExMatch(pName.c_str()) )
      {
        pos->emit(paras,script,replace_tag);        
      }
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
          emit( paras, items, replace_tag );
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
    void emit( paragraphMap & paras, ocString templateTags, string replace_tag )
    {
      stringstream s;
      page_paragraphs & pgcontent = pg.get_page_paragraphs();
      paragraph_vector::iterator pos;
      for( pos=pgcontent.begin(); pos!=pgcontent.end(); ++pos )
      {
        ocString tags = "," + templateTags + ",";
        string testTag = "," + pos->template_tag() + ",";
       
        ocString pName = pos->name(); 

        if( tags.regExMatch( testTag.c_str() ) && pName.regExMatch(roles.c_str()) )
        {
          pos->emit(paras,s,replace_tag);
          ocString content = s.str();
          script << content.replaceAll("{{name}}",oLogin.FullName())
                           .replaceAll("{{last}}",oLogin.Last())
                           .replaceAll("{{first}}",oLogin.Last())
                           .replaceAll("{{Id}}",oLogin.Id())
                           .replaceAll("{{email}}",oLogin.Email())
                           .replaceAll("{{phone}}",oLogin.PhoneNumber());
        }
      }
    }
};
#endif

#endif 
