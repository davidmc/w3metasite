/*
  
  Copyright 2002 - 2007 by D.K. McCombs.
  ======================================

  Needs: (all relative to invocation)
     Templates/adminPane.htmp,
     Templates/divform.htmp,
     scripts/menumanip.js
     Templates/divlist.htmp
     Templates/map.htm
*/

#include <iostream>
#include <iomanip>
#include "cgiTemplates.h"
#include "connectstring"
#include "forms.h"
#include "ocTypes.h"
#include "ocString.h"
#include "read_write_base.hpp"
#include "forms_base.hpp"
#include "list_base.hpp"
#include "menu.hpp"
#include "openLogin.h"
#include "siteLimit.h"
#include "ocFileSys.h"

using namespace std;
openLogin oLogin;


class menu_item_List:  public list_base
{
public: 
  long long menu_id;
  string id, place_order;
  long place_level;
  cgiTemplates movemap;
  
  // Constructor
  menu_item_List(cgiScript&sc, long long id):list_base(sc),menu_id(id)
  {
    movemap.load("Templates/map.htm");                   
  }  
  ~menu_item_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","menu_item_ui.cgi");
    hotCol=3; 
    ocString  sql = "select mi.id, mi.place_order, mi.place_level, l.name "
        "from metasite.menu_items mi "
        "inner join metasite.links l on l.id = mi.link_id "
                    "where mi.menu_id = ";
    sql.append(menu_id);
    sql += " order by mi.place_order";
    // emitHeadings("Item Name"); 
    getData(sql);
    emitData();    
    emitEnd();
    return breturn;
  }
  void derived_complete_event( void )
  {  
    ocString loco = "menu_item_ui.cgi";
    if(menu_id)
    {
      loco += "?menu_id=";
      loco.append(menu_id);
    }
    ocString adder = listTemplate.getParagraph("list_new");
    webIO << adder.replace("$location",loco.c_str())
                  .replace("$item","Menu Item");
  } 
  
  virtual void sendField( int iField, ocString & td )
  {
     // cache 
     if(iField==0)
     {
       id=rs.getField(iField).format();
     }
     if(iField==1)
     {
       place_order=rs.getField(iField).format();
     }
     if(iField==2)
     {
       place_level=atol(rs.getField(iField).format().c_str());
     }
  }
  
  // Override this to get the group_id in the link
  virtual void sendHotField( int iField, ocString & td )
  {
    ocString linkId = id;
    ocString moveMap =  movemap.getParagraph("movemap");
    ocString level, order;
    ocString FieldName = rs.getField(iField).format();

    level.append(place_level*30);
    order.append(place_order);
    if(menu_id)
    {
      linkId += "&menu_id=";
      linkId.append(menu_id);
    }
    webIO << moveMap.replaceAll("miid",id.c_str())
                    .replace( "$LEVEL$", level.c_str() )
                    .replace( "$ORDER$", order.c_str() )
          << td.replace( "$data$", 
                          editLink.replaceAll( "$key$", linkId.c_str() )
                         .replaceAll( "$col$", FieldName.c_str() ).c_str() );
  }
};


class menu_form:  public menu, public forms_base
{
public: 
  menu_form(cgiScript & script):menu(),forms_base(script)
  {
    setKey(*this);
    site_id = atoll(currentSite(script.ClientArguments()).c_str());
  } 
  virtual ~menu_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "id", id );
  }
  void form_data_transfer( void )
  {
    // Make data in controls back to object
    llongFXfer( "site_id", site_id );
    stringFXfer( "name", name);
    stringFXfer( "reference_template", reference_template);
    stringFXfer( "template_open_tag", template_open_tag);
    stringFXfer( "template_close_tag", template_close_tag);    
  } 
  
  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  } 
  
  // Implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    string siteLimitation = siteFocus(oLogin);
    string sql = "select id, name from metasite.sites";
    if( siteLimitation.length() != 0 )
    {
      sql += " where id ";
      sql += siteLimitation;
    }


    script << makeTop("", "Menu Manager")
           << formTemplate.getParagraph("advanced_begin")
           << makeStaticBox("id", "id", id, "12")
           << makeComboBox( "Site", "site_id", site_id, 
                            sql );
    sql = "select path, path from metasite.templates ";
    if( siteLimitation.length() != 0 )
    {
      sql += " where site_id ";
      sql += siteLimitation;
    }

    script << "<br class='clearall'>" << endl;
    script << makeComboBox("reference_template", "reference_template", reference_template ,sql,"Choose");
    script << "<br class='clearall'>" << endl;
    script << makeMetaBox("template_open_tag", "template_open_tag", template_open_tag ,"35",reference_template, "open");
    script << "<br class='clearall'>" << endl;
    script << makeMetaBox("template_close_tag", "template_close_tag", template_close_tag ,"35",reference_template, "close");
    script << "<br class='clearall'>" << endl; 

    script << "<br class='clearall'>" << endl
           << formTemplate.getParagraph("advanced_end")                 
           << makeTextBox("name","name",name,"50") 
           << "<br class='clearall'>" << endl
           << makeButtons( key() );
    if ( id != 0 )
    {
      script << "<tr><td colspan='2'>";
      script << "<b>Menu Items:</b><br>";
      menu_item_List list(script,id);
      list.loadListTemplates("Templates/divlist.htmp");  
      list.list_display();
      script << "</td></tr>";
    }    
    script << makeBottom( m_result ) << endl; 
    return breturn;
  }
};


int main( int argcount, char ** args )
{
  cgiScript script( "text/html", false );
  menu_form myFrm(script); 

  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgoo;    
    pgoo.load("Templates/adminPane.htmp");
    
    script << ocString(pgoo.getParagraph("top"))
              .replace( "// More Functions?",
                        "</SCRIPT><SCRIPT type=\"text/javascript\" src=\"scripts/menumanip.js\"></SCRIPT>" );

    myFrm.loadControlTemplates("Templates/divform.htmp");  
    myFrm.form_action();  
    myFrm.form_display();
    
    ocString end = pgoo.getParagraph("bottom");   
    if( myFrm.getMode() != "s" )
    {
      script << end.replace("/*_extra_js_*/","  remenu();");
    }
    else
    {
      script << end;
    } 
  }
  else
  {
    script.closeHeader();
    script << oLogin.getLastError() << endl;
    // script.Redirect("signIn.html"); 
  } 
}
// compile implementations here
#include "read_write_base.cpp"
#include "forms_base.cpp"
