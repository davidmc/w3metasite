/* 
  Page Paragraph Class and list - pg_paragraph.hpp
  ----------------------------------------  
  This class represents 
  an abstraction of a web pages paragraphs.  
*/

// Sentry
#ifndef Web_Page_Paragraph_Hpp
#define Web_Page_Paragraph_Hpp

// includes
#include "cgiTemplates.h"
#include "read_base.hpp"
#include <vector>

// #define DO_OPEN_LOGGING
#include "openLogger.h"

using namespace std;

class page_paragraph 
{
protected:
  long long m_id;
  long long m_site_id;
  long long m_page_id;
  long m_place_order;
  string m_template_tag; 
  string m_replace_tag;  
  string m_name;
  string m_content;
  
public:
  page_paragraph()
  :m_id(0),m_site_id(0),m_page_id(0),m_place_order(0)
  {
    ;
  }
  
  virtual ~page_paragraph()
  {
    ;
  }
  
  page_paragraph & operator = ( const page_paragraph & in )
  {
    id(in.id()); 
    site_id(in.site_id());
    page_id(in.page_id());
    place_order(in.place_order());
    template_tag(in.template_tag());
    replace_tag(in.replace_tag());
    name (in.name());
    content(in.content());  
    return *this;
  }
  
  void propset( openRS & rs )
  {
    id(atoll(rs.getField(0).format().c_str()));
    site_id(atoll(rs.getField(1).format().c_str()));
    page_id(atoll(rs.getField(2).format().c_str()));
    place_order(atol(rs.getField(3).format().c_str()));
    template_tag(rs.getField(4).format());
    replace_tag(rs.getField(5).format()); 
    name(rs.getField(6).format());
    content(rs.getField(7).format());
  }
  
  // new emit base on template control
  void emit( paragraphMap & paras, ostream & toBrowser, string replace_tag )
  {
    paragraphMap::iterator pos = paras.find(template_tag());
    if( pos != paras.end() )
    {
      ocString uniqueId = "paragraph_";
      uniqueId.append(place_order());
      ocString output = pos->second;
      toBrowser << 
        output.replace( "$id",uniqueId.c_str() ) 
              .replace( replace_tag.c_str(),
                        content().c_str() ); 
    }
  }
  
  // Property access
  // get
  long long id(void) const {return m_id;}
  long long site_id(void) const {return m_site_id;}
  long long page_id(void) const {return m_page_id;}
  long place_order(void) const {return m_place_order;}
  string template_tag(void) const {return m_template_tag;} 
  string replace_tag(void) const {return m_replace_tag;}  
  string name(void) const {return m_name;}
  string content(void) const {return m_content;}
  // set
  void id(long long in) { m_id=in; }
  void site_id( long long in) { m_site_id=in; }
  void page_id( long long in) { m_page_id=in; }
  void place_order( long in) { m_place_order=in; }
  void template_tag( string in) { m_template_tag=in; } 
  void replace_tag( string in) { m_replace_tag=in; }  
  void name (string in) { m_name=in; }
  void content( string in) { m_content=in; }  
};
 
typedef vector<page_paragraph> paragraph_vector;
  
class page_paragraphs : protected ref_base, public paragraph_vector
{ 
public:
  page_paragraphs(openDB & dbIn):ref_base(dbIn),paragraph_vector()
  {
    ;
  }
  ~page_paragraphs()
  {
    ;
  }
  string getSQL( long long pgid )
  {
    ocString sql = "select id, site_id,page_id,place_order, "
                   "template_tag,replace_tag,name,content "
                   "from metasite.paragraphs where page_id = ";
    sql.append( pgid );
    sql += " order by place_order";
    writelog( sql );
    return sql;               
  }
  
  bool load( long long pgid )
  {
    bool breturn=false;
    bool bData;
    for( bData=rs.open(getSQL(pgid));bData;bData=rs.next() )
    {      
      page_paragraph pp;      
      pp.propset(rs);      
      push_back(pp);
    }    
    rs.close();
    return breturn;
  }  
    
  void emit( paragraphMap & paras, ostream & toBrowser, 
             ocString templateTags, string replace_tag )
  {
    paragraph_vector::iterator pos;
    for( pos=begin(); pos!=end(); ++pos )
    {  
      ocString tags = "," + templateTags + ",";
      string testTag = "," + pos->template_tag() + ",";
      if( tags.regExMatch( testTag.c_str() ) )
      {
        pos->emit(paras,toBrowser,replace_tag);        
      }
    }
  }  
};

#endif
