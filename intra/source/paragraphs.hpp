/*
  paragraphs.hpp

  Object Definition and Implementation 
  for paragraphs.
  
  Copyright (C) 2006 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef paragraphs_HPP
#define paragraphs_HPP

#include "read_write_base.hpp"

class paragraphs_Obj: public read_write_base
{
public:
  identifier id;
  llong site_id;
  llong page_id;
  int place_order;
  string template_tag;
  string replace_tag;
  string name;
  string content;
  llong author;
  time_date time_authored;
  time_date time_start;
  time_date time_end;
  bool approved;
  string section;
  paragraphs_Obj():read_write_base()
  ,id(0LL)
  ,site_id(0LL)
  ,page_id(0LL)
  ,place_order(0)
  ,template_tag("")
  ,replace_tag("")
  ,name("")
  ,content("")
  ,author(0LL)
  ,time_authored( )
  ,time_start( )
  ,time_end( )
  ,approved(false)
  ,section("")
  {
    // set name
    data_name("metasite.paragraphs");
    // add fields
    addDXMap( new llongXfer("id", &id ));
    addDXMap( new llongXfer("site_id", &site_id ));
    addDXMap( new llongXfer("page_id", &page_id ));
    addDXMap( new intXfer("place_order", &place_order ));
    addDXMap( new stringXfer("template_tag", &template_tag ));
    addDXMap( new stringXfer("replace_tag", &replace_tag ));
    addDXMap( new stringXfer("name", &name ));
    addDXMap( new stringXfer("content", &content ));
    addDXMap( new llongXfer("author", &author ));
    addDXMap( new time_dateXfer("time_authored", &time_authored ));
    addDXMap( new time_dateXfer("time_start", &time_start ));
    addDXMap( new time_dateXfer("time_end", &time_end ));
    addDXMap( new boolXfer("approved", &approved ));
    addDXMap( new stringXfer("section", &section ));
  }
  virtual bool ivalidate( void  )
  { 
    bool bres=true;
    if( site_id < 1 )
    {
      bres=false;
      m_result="Please Specify a Site.";
    }
    else if ( page_id < 1 )
    {
      bres=false;
      m_result="Please Specify a Page.";
    }
    else if( section.length() == 0 )
    {
      bres=false;
      m_result="Please Specify the Section.";
    }
    else if( replace_tag.length() == 0 )
    {
      bres=false;
      m_result="Please Specify the Replacement Tag!";
    }
    else if( name.length() == 0 )
    {  
      // just give it a name      
      name=content.substr(0,20);
    }
    else
    {
      // all OK
      if( place_order == 0 )
      {
        ocString msql = "select max(place_order) from metasite.paragraphs "
                      "where page_id = ";
        msql.append( page_id );
        if( section.length() )
        {
          msql += " and section ='" + section + "'";
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
      ocString msql = "select id from metasite.paragraphs "
                      "where page_id = ";
      msql.append( page_id );
      msql += " and place_order = ";
      msql.append( place_order );
      msql += " and id <> ";
      msql.append( id );
      msql += " and template_tag = '" +   section + "'";
      if(rs.open(msql))
      {
        rs.close();
        // we want to increment all of the id's equal to or greater than this
        msql = "update metasite.paragraphs set place_order=place_order+1 where page_id = ";
        msql.append( page_id );
        msql += " and place_order >= ";
        msql.append( place_order );
        msql += " and id <> ";
        msql.append( id );
        msql += " and section = '" + section + "'";
        cmd.execute(msql);
      }
    }
    return true;
  }

};
#endif




