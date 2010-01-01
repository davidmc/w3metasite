/* 

  Page Template class - template.hpp
  
  This class represents 
  an abstraction of a web page template.
  
*/

// sentry
#ifndef Web_Page_Template_Hpp
#define Web_Page_Template_Hpp

// includes
#include "cgiTemplates.h"
#include "read_base.hpp"

class page_template: public cgiTemplates, protected read_base
{
  string getIdSql( long long Id )
  {
    // load page from lookup url
    ocString sql = "select id, "
                   "site_id, "                   
                   "name, "
                   "path "
                   "from metasite.templates "
                   "where id = ";
    sql.append(Id);           
    return sql;                
  }
  void propset( void )
  {
    m_id = atoll(rs.getField(0).format().c_str());
    m_site_id = atoll(rs.getField(1).format().c_str());
    m_name = rs.getField(2).format();
    m_path = rs.getField(3).format();
  }
protected:  
  // Properties
  long long m_id;
  long long m_site_id;  // link to site
  string    m_name;     // name (for menu item)
  string    m_path;     // path to file  
  
public:
  page_template( void )
  :cgiTemplates(),read_base(),m_id(0),m_site_id(0)
  { 
    ;   
  } 
  virtual ~page_template()
  {
    ;
  }
  bool load( long long id )
  {
    bool breturn=false;
        
    // load the template info by id from the db.
    if( rs.open(getIdSql(id)) )
    {       
      propset();
      // concatenate 'Templates/' and path info to lookup url
      string lookup = "Templates/" + path();
      
      // have the base class load the template file
      breturn = cgiTemplates::load ( lookup.c_str() );

    }
    
    // return result of load
    return breturn;
  }
  
  // Property Access Methods
  // gets
  long long id(void){ return m_id;}
  long long site_id(void){ return m_site_id;} 
  string name(void){ return m_name;}       
  string path(void){ return m_path;}
  // sets        
  void id( long long in ) {m_id=in;}
  void site_id( long long in ) {m_site_id=in;}
  void name( string in ) {m_name=in;}
  void path( string in ) {m_path=in;}
};

#endif
