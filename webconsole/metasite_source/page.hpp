/*
  page.hpp 
  
  page object abstraction

  Copyright (late) 2004 - 2004 by D.K. McCombs.
  ======================================
    
*/
#ifndef page_hpp
#define page_hpp

#include "connectstring"
#include "ocTypes.h"
#include "ocString.h"
#include "read_write_base.hpp"
#include "openLogger.h"
#ifndef SUPPRESS_LINK_CREATION

#include "link.hpp"
#include "site.hpp"
#include "templates.hpp"
#include "menu.hpp"
#include "sys_page_menus.h"

#include "cgiTemplates.h"
#include "ocXML.h"

#endif
using namespace std;

/*

CREATE TABLE pages (
  id bigint not null primary key auto_increment,
  site_id     bigint NOT NULL,  -- the id to the containing web site  
  template_id bigint NOT NULL,  -- the id to the page template
  name        text NOT NULL,    -- name
  url         text,             -- location
  meta_description text         -- description for robots.
);

*/

class page: public read_write_base
{
public:
  identifier id;
  long long site_id;
  long long template_id;  
  string name;
  string url;
  string meta_description;
  
  page():read_write_base(),id(0LL),site_id(0LL),template_id(0LL)
  {  
    // set name
    data_name("metasite.pages");
    
    // add fields
    addDXMap( new llongXfer("id", &id ));
    addDXMap( new llongXfer("site_id", &site_id ));
    addDXMap( new llongXfer("template_id", & template_id));            
    addDXMap( new stringXfer("name", &name ));
    addDXMap( new stringXfer("url", &url ));
    addDXMap( new stringXfer("meta_description", &meta_description ));
  }

  virtual bool uvalidate( changeMap & changes  ) 
  { 
    // find out if changes hold url
    if( changes.find("url") != changes.end() )
    {
      // if so, find out what this url was before change
      ocString sql = "select url from metasite.pages where id = ";
      sql.append(id);
      if(rs.open(sql))
      {      
        // update links with that url to have the new url
        sql = "update metasite.links set url = '" + url + "' where site_id = ";
        sql.append(site_id);
        sql += " and url = '";
        sql += rs.getField(0).format();
        sql += "'";
        cmd.execute(sql);
        rs.close();        
      }        
    }
    return true; 
  }
#ifndef SUPPRESS_LINK_CREATION
  bool usupplemental( changeMap & changes )
{
  if( changes.find("template_id") != changes.end() )
  {
    deletePageMenus();
    createPageMenus();
  }
    return true;
  }
  bool isupplemental( void )
  {
    // automatically generate a link item with this url 
    class link ln;
    ln.site_id=site_id;
    ln.name = name;
    ln.url = url;
    ln.db_insert();
    // automatically create page menus;
    createPageMenus();
    return true; 
  }
  void deletePageMenus( void )
  {
    ocString sql = "delete from metasite.page_menus where page_id = ";
    sql.append(id);
    cmd.execute(sql);
  }
  void deletePageParagraphs( void )
  {
    ocString sql = "delete from metasite.paragraphs where page_id = ";
    sql.append(id);
    cmd.execute(sql);
  }
  void createPageMenus( void )
  {
    // find the menus associated with this page
    siteObject thisSite;
    thisSite.id - site_id;
    thisSite.key(site_id);
    if( thisSite.get_data() )
    {
      Template tmplt;
      tmplt.id = template_id;
      tmplt.key(template_id);
      if( tmplt.get_data() )
      {
        // build a path to the Template
        string fullPath = thisSite.path;
        fullPath += "/Templates/";
        fullPath += tmplt.path;

        cgiTemplates cTemp;
        if( cTemp.load(fullPath.c_str()) )
        {
          // get the control section
          xmlParser parser( cTemp.getParagraph("control") );
          parser.parse();
          node_vector::iterator & it = parser.findFirstNodeByName( "menu" );
          int order=0;
          while( it !=  parser.states.nodes.end() )
          {
            xmlNode & node = *it;
            string open_tag = node.attr["open"];
            string close_tag = node.attr["close"];

            class menu mnu;
            string clause = "reference_template = '";
            clause += tmplt.path;
            clause += "' and template_open_tag = '";
            clause += open_tag;
            clause += "' and  template_close_tag = '";
            clause +=  close_tag;
            clause += "'";

            if( mnu.get_data(clause) )
            {
              page_menus_Obj pg_mnu;
              pg_mnu.site_id = site_id;
              pg_mnu.menu_id = mnu.id;
              pg_mnu.page_id = id;
              pg_mnu.place_order = order++;
              pg_mnu.collapsible = false;
              pg_mnu.db_insert();
            }
            it = parser.findNextNodeByName( "menu" );
          }
        }
      }
    }
  }
  
  virtual bool dsupplemental( void )
  { 
    // delete all associated content
    deletePageParagraphs();
    deletePageMenus();
    return true;
  }
#endif  
};

#endif
