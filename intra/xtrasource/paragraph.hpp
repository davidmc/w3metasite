/*
  Copyright 2004 by D.K. McCombs.
  =============================== 
  paragraph.hpp 
  
  OBSOLETE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
     
*/

#include "connectstring"
#include "forms.h"
#include "ocTypes.h"
#include "ocString.h"
#include "read_write_base.hpp"

// deprecated
#warn "DO NOT USE!"
#warn "DO NOT USE!"
#warn "DO NOT USE!"
#warn "DO NOT USE!"

using namespace std;

/*

CREATE TABLE paragraphs (
  id bigint not null primary key auto_increment,
  site_id     bigint NOT NULL,
  page_id     bigint NOT NULL,
  place_order int NOT NULL,
  template_tag char(128) NOT NULL, 
  replace_tag  char(128) NOT NULL,  
  name text, 
  content text
);

*/
class paragraph: public read_write_base
{
public:
  identifier id;
  long long site_id;
  long long page_id;
  int  place_order;   
  string template_tag;
  string replace_tag;
  string name;
  string content;
  
  // for ordering
  ocString section;
  
  paragraph()
  :read_write_base(),id(0LL),site_id(0LL)
  ,page_id(0LL),place_order(0),replace_tag("$content")
  {  
    // set name
    data_name("paragraphs");
        
    // add fields
    addDXMap( new llongXfer("id", &id ));
    addDXMap( new llongXfer("site_id", &site_id )); 
    addDXMap( new llongXfer("page_id", &page_id ));
    addDXMap( new intXfer("place_order", &place_order ));
    addDXMap( new stringXfer("template_tag", &template_tag ));  
    addDXMap( new stringXfer("replace_tag", &replace_tag ));      
    addDXMap( new stringXfer("name", &name ));
    addDXMap( new stringXfer("content", &content ));    
  }
  /*
  Event hooks for business logic
  =========================
  // Done before any action - a false return prevents action
  virtual bool ivalidate( void  ) { return true; }
  virtual bool uvalidate( changeMap & changes  ) { return true; }
  virtual bool dvalidate( void ) { return true; }
  
  // Done after any action - a false return sets a warning
  virtual bool isupplemental( void ){ return true; }
  virtual bool usupplemental( changeMap & changes ){ return true; }
  virtual bool dsupplemental( void ){ return true; }
  virtual bool ssupplemental( void ){ return true; } 
  */  
  virtual bool ivalidate( void  ) 
  { 
    bool bres=true;
    if( site_id == 0 )
    {
      bres=false;
      m_result="No Site Specified!";
    }
    else if ( page_id == 0 )
    {
      bres=false;
      m_result="No Page Specified!";
    }
    else if( template_tag.length() == 0 )
    {
      bres=false;
      m_result="No Template Text Specified!";
    }
    else if( replace_tag.length() == 0 )
    {
      bres=false;
      m_result="No Replacement Tag!";
    }
    else if( name.length() == 0 )
    {  
      // just give it a name      
      name="Section Name Here";  
    }
    else
    {
      // all OK
      if( place_order == 0 )
      {
        ocString msql = "select max(place_order) from paragraphs "
                      "where page_id = ";
        msql.append( page_id );
        if( section.length() )
        {
          msql += " and template_tag in ('" +  
                  section.replaceAll(",","','") + "')";
        }
        if(rs.open(msql))
        {
          place_order = atol(rs.getField(0).format().c_str()) + 1;
          rs.close();
        }
      }   
    }
    return bres; 
  }  
  virtual bool ssupplemental( void )
  { 
    return true; 
  }
  virtual bool uvalidate( changeMap & changes  )
  { 
    return true; 
  }
  virtual bool usupplemental( changeMap & changes ) 
  {
    // make sure that there is no positioning ambiguity
    if( changes.find("place_order") != changes.end() )
    {
      ocString msql = "select id from paragraphs "
                      "where page_id = ";
      msql.append( page_id );
      msql += " and place_order = ";
      msql.append( place_order );
      msql += " and id <> ";
      msql.append( id );
      if( section.length() )
      {
        msql += " and template_tag in ('" +  
                section.replaceAll(",","','") + "')";
      }
      if(rs.open(msql))
      {
        rs.close();
        // we want to increment all of the id's equal to or greater than this
        msql = "update paragraphs set place_order=place_order+1 where page_id = ";
        msql.append( page_id );
        msql += " and place_order >= ";
        msql.append( place_order );
        msql += " and id <> ";
        msql.append( id );
        if( section.length() )
        {
          msql += " and template_tag in ('" +  
                  section.replaceAll(",","','") + "')";
        }
        cmd.execute(msql);

      }
    }
    return true;
  }
};
