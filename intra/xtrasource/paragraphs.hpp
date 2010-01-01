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
#include "cgiTemplates.h"
#include "ocXML.h"

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
  llong edit_type;
  paragraphs_Obj():read_write_base()
  ,id(0LL)
  ,site_id(0LL)
  ,page_id(0LL)
  ,place_order(0)
  ,template_tag("")
  ,replace_tag("$content")
  ,name("")
  ,content("")
  ,author(0LL)
  ,time_authored( )
  ,time_start( )
  ,time_end( )
  ,approved(true)
  ,section("")
  ,edit_type(0LL)
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
    addDXMap( new llongXfer("author", &author ));
    addDXMap( new time_dateXfer("time_authored", &time_authored ));
    addDXMap( new time_dateXfer("time_start", &time_start ));
    addDXMap( new time_dateXfer("time_end", &time_end ));
    addDXMap( new boolXfer("approved", &approved ));
    addDXMap( new stringXfer("section", &section ));
    addDXMap( new llongXfer("edit_type", &edit_type ));
    time_end.addMonths(1);
    time_end.normalize();
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
    if( bres )
    {
      if( template_tag.length() == 0 )
      {
        getFirstTemplateTag();
      }
      nameFixup();

      // all OK
      if( place_order == 0 )
      {
        ocString msql = "select max(place_order) from paragraphs "
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
  void nameFixup( void )
  {
    if( name.length() == 0 )
    {
      // just give it a name
      ocString Parse(content);
      while( Parse.endOfParse() == false )
      {
        name += Parse.parse( "<" );
        Parse.parse( ">" );
      }
      if( name.length() )
      {
        Parse = name;
        name = "";
        for( int i=0; i < 6 && (Parse.endOfParse() == false ); i++ )
        {                     
          name += Parse.parse( " " );
          name += " ";
        }
      }
      else
      {
        name="all tags";
      }
    }
  }
  // if we have no content, we may not know what the tagged template section of the page to work with
  bool getFirstTemplateTag( void )
  {
    bool ret = false;
    ocString sql = "select t.path from templates t inner join pages p on p.template_id = t.id where p.id = ";
    sql.append(page_id);
    
    if( rs.open(sql) )
    {
      string path = "./Templates/" + rs.getField(0).format();
      cgiTemplates cTemp;
      if( cTemp.load(path.c_str()) )
      {
        // get the control section
        xmlParser parser( cTemp.getParagraph("control") );
        parser.parse();
        node_vector & xnodes = parser.nodeList();
        for(int i=0;i<xnodes.size();i++)
        {
          xmlNode & node = xnodes[i];
          
          if( node.name == "content" ||  node.name == "contactus" )
          {
            if( node.attr["section"] == section )
            {
              ocString items = node.attr["items"];
              template_tag = items.parse(",");
              ret = true;
              break; // we are done
            }
          }
        }
      }
    }

    return ret;
  }
  
  virtual bool ssupplemental( void )
  { 
    return true; 
  }
  virtual bool uvalidate( changeMap & changes  )
  {
    if( changes.find("name") != changes.end() )
    {
      nameFixup();
    }
    // force this
    changes["template_tag"]="template_tag";
    return true; 
  }
  virtual bool usupplemental( changeMap & changes ) 
  {
    // make sure that there is no positioning ambiguity
    if( changes.find("place_order") != changes.end() )
    {
      isupplemental();
    }
    return true;
  }

  bool isupplemental( void )
  {
    ocString msql = "select id from paragraphs "
                      "where page_id = ";
    msql.append( page_id );
    msql += " and place_order = ";
    msql.append( place_order );
    msql += " and id <> ";
    msql.append( id );
    msql += " and section = '" +   section + "'";
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
      msql += " and section = '" + section + "'";
      cmd.execute(msql);
    }
    return true;
  }
};
#endif




