/*
  menu_item.hpp 
  
  hyperlink object abstraction

  Copyright (late) 2004  by D.K. McCombs.
  ======================================
  www.w3sys.com
    
*/
#ifndef menu_item_hpp
#define menu_item_hpp

#include "connectstring"
#include "ocTypes.h"
#include "ocString.h"
#include "read_write_base.hpp"
#include "openLogger.h"

/*

CREATE TABLE menu_items (
  id bigint not null primary key auto_increment,
  site_id bigint NOT NULL,
  menu_id bigint NOT NULL,
  link_id bigint NOT NULL,
  template_tag char(128) NOT NULL,
  locate_tag char(128) NOT NULL,
  target_tag char(128) NOT NULL,
  label_tag char(128) NOT NULL,
  place_order int NOT NULL,
  place_level int NOT NULL  
);

*/

enum menu_item_direction 
{
  up,rgt,btm,lft,vabs
};

class menu_item: public read_write_base
{
public:
  identifier id;
  long long site_id; 
  long long menu_id;
  long long link_id;  
  string template_tag; // the name of the html snippet in the template file
  string locate_tag;   // the replacement name for the location
  string target_tag;   // the replacement name for the target window
  string label_tag;    // the replacement name for the label
  int place_order;  // the order of the menu items in the list
  int place_level;  // the called level of the menu item top = 0,1 etc.

  menu_item()
  :read_write_base(),id(0LL),site_id(0LL),menu_id(0LL)
  ,locate_tag("$location"),label_tag("$label"),target_tag("_self")
  ,place_order(0),place_level(1)
  {  
    // set name
    data_name("metasite.menu_items");
    // add fields
    addDXMap( new llongXfer("id", &id ));
    addDXMap( new llongXfer("site_id", &site_id )); 
    addDXMap( new llongXfer("menu_id", &menu_id )); 
    addDXMap( new llongXfer("link_id", &link_id ));                         
    addDXMap( new stringXfer("template_tag", &template_tag ));
    addDXMap( new stringXfer("locate_tag", &locate_tag ));
    addDXMap( new stringXfer("target_tag", &target_tag ));
    addDXMap( new stringXfer("label_tag", &label_tag ));
    addDXMap( new intXfer("place_order", &place_order ));
    addDXMap( new intXfer("place_level", &place_level ));
  }

  virtual bool ivalidate( void  ) 
  { 
    bool bres = true;
    if( template_tag.length() == 0 )
    {
      bres=false;
      m_result="No Template Text Specified!";
    }
    else
    {
      // all OK
      if( place_order == 0 ) // the default
      {
        place_order = 1; // the good default
        ocString msql = "select max(place_order) from metasite.menu_items "
                        "where menu_id = ";
        msql.append( menu_id );
        if(rs.open(msql))
        {
          place_order = atol(rs.getField(0).format().c_str()) + 1;
          rs.close();
        }
      }
    }
    return bres;
  }
  bool usupplemental( changeMap & changes )
  {
    if( changes.find("place_order") != changes.end() )
    {
      ocString mvSql = "update metasite.menu_items set place_order = place_order+1 where place_order >= ";
      mvSql.append(place_order);
      mvSql += " and menu_id = ";
      mvSql.append(menu_id);
      mvSql += " and id <> ";
      mvSql.append(id);
      cmd.execute(mvSql);
    }
    return true;
  }

  bool move( long long mid, menu_item_direction dir, long absPos=0 )
  {
    bool bret=false;
    int  nplace_order=0;
    int  oplace_order;
    long long oid=0;
    // load the id in question
    id=mid;
    key(mid);
    bret = get_data();
    ocString mvSql;    
    if(bret)
    {
      oplace_order=place_order;
      changeMap changes;
      // good now what direction
      switch(dir)
      {
        case up:
        // first check to see if any are before you
          mvSql = "select id, place_order from metasite.menu_items where place_order < ";
        mvSql.append(place_order);
        mvSql += " and menu_id = ";
        mvSql.append(menu_id);
        mvSql += " order by place_order desc limit 1";
        bret = false;
        if(rs.open(mvSql))
        {
          oid = atoi(rs.getField(0).format().c_str());
          place_order = atoll(rs.getField(1).format().c_str());          
        }
        if( !oid ) place_order--;
        changes["place_order"]="place_order";
        bret = db_action( "u", changes );
        if( oid )
        {
          id=oid;
          key(oid);
          bret = get_data();
          place_order=oplace_order;
          changes["place_order"]="place_order";
          bret = db_action( "u", changes );
        }
        break;
        
        case rgt:
          ++place_level;
          changes["place_level"]="place_level";
          bret = db_action( "u", changes );
        break;
        
        case btm:
        // first check to see if any are after you                
          mvSql = "select id, place_order from metasite.menu_items where place_order > ";
        mvSql.append(place_order);
        mvSql += " and menu_id = ";
        mvSql.append(menu_id);
        mvSql += " order by place_order asc limit 1";
        bret = false;
        if(rs.open(mvSql))
        {
          oid = atoi(rs.getField(0).format().c_str());
          place_order = atoll(rs.getField(1).format().c_str());          
        }
        if( !oid ) place_order++;
        changes["place_order"]="place_order";
        bret = db_action( "u", changes );
        if( oid )
        {
          id=oid;
          key(oid);
          bret = get_data();
          place_order=oplace_order;
          changes["place_order"]="place_order";
          bret = db_action( "u", changes );
        }
        break;
       case vabs: // vertical absolute position
        // first check to see if any are after you                
        mvSql = "select id, place_order from metasite.menu_items where place_order = ";
        mvSql.append(absPos);
        mvSql += " and menu_id = ";
        mvSql.append(menu_id);
        mvSql += " order by place_order asc limit 1";
        bret = false;
        if(rs.open(mvSql))
        {
          oid = atoi(rs.getField(0).format().c_str());
          place_order = atoll(rs.getField(1).format().c_str());          
        }
        if( !oid ) place_order=absPos;
        changes["place_order"]="place_order";
        bret = db_action( "u", changes );
        // switch positions
        if( oid )
        {
          id=oid;
          key(oid);
          bret = get_data();
          place_order=oplace_order;
          changes["place_order"]="place_order";
          bret = db_action( "u", changes );
        }
        break; 
        case lft:
        // can only go to 1
        if( place_level > 1 )
        {
          --place_level;
          changes["place_level"]="place_level";
          bret=db_action( "u", changes );          
        }
        break;
      }      
    }
    
    return bret;
  }
};
#endif
