/*
  
  Copyright 2007 by D.K. McCombs.
  ======================================
    
*/
#include "connectstring"
#include <iostream>
#include <iomanip>
#include "cgiTemplates.h"
#include "forms.h"
#include "ocTypes.h"
#include "ocString.h"
#include "read_write_base.hpp"
#include "forms_base.hpp"
#include "list_base.hpp"
#include "openLogin.h"
#include "siteLimit.h"
#include "menu_item.hpp"
#include "link.hpp"
#include "menu.hpp"
#include "lookup.hpp"

using namespace std;
openLogin oLogin;

class menu_item_form:  public menu_item, public forms_base
{
  void addPageParagraphMenuItems()
  {
    ocString selPage = "select pg.id, pg.url from metasite.pages pg inner join links ln on pg.url = ln.url where ln.id = ";
    selPage.append( link_id );
    if( rs.open(selPage ) )
    {
      ocString URL = rs.getField(1).format();
      ocString selPars = "select place_order, name from metasite.paragraphs where name not like '-%' and page_id = ";
      selPars += rs.getField(0).format();
      rs.close();
      int order = place_order;
      bool opened = rs.open( selPars );
      while( opened )
      {
        script << "<p> add " <<  URL << "#paragraph_" << rs.getField(0).format() << "</p>" << endl;
        class link lnk;
        lnk.key(link_id);
        // get a link like this one.
        if( lnk.get_data() )
        {
          lnk.id = 0;
          lnk.key(0);
          lnk.name = rs.getField(1).format();
          lnk.url =  URL +  "#paragraph_" + rs.getField(0).format();
          
          if( lnk.db_insert() )
          {
            order++;
            menu_item mitm;
            mitm.site_id = site_id;
            mitm.menu_id =  menu_id;
            mitm.link_id = lnk.key();
            mitm.template_tag = template_tag;
            mitm.locate_tag = locate_tag;
            mitm.target_tag = target_tag;
            mitm.label_tag = label_tag;
            mitm.place_order = order;
            mitm.place_level = place_level+1;
            if( mitm.db_insert() )
            {
              mitm.id =  mitm.key();
              changeMap changes;
              changes["place_order"]="place_order";
              mitm.usupplemental(changes);
            }
          }
        }
        opened = rs.next();
      }
    }
  }
public: 
  menu_item_form(cgiScript & script):menu_item(),forms_base(script){setKey(*this);} 
  virtual ~menu_item_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "id", id );
  }
  void form_data_transfer( void )
  {
    // Make data in controls back to object
    llongFXfer( "site_id", site_id );
    llongFXfer( "menu_id", menu_id );  
    llongFXfer( "link_id", link_id );    
    stringFXfer( "template_tag", template_tag);  
    stringFXfer( "locate_tag", locate_tag);
    stringFXfer( "target_tag", target_tag);    
    stringFXfer( "label_tag", label_tag);    
    intFXfer( "place_order", place_order); 
    intFXfer( "place_level", place_level);                                           
  } 
  
  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  } 
  
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    string siteLimitation = siteFocus(oLogin);
    ocString sql = "select id, name from metasite.sites";
    if( siteLimitation.length() != 0 )
    {
      sql += " where id ";
      sql += siteLimitation;
    }
    script << makeTop("menu_item_ui.cgi", "Menu Item")
        << formTemplate.getParagraph("advanced_begin");
    // Add PageParagraphs
    ocString grok = "<a href='?";
    if( id > 0 )
    {
      grok += "Id=";
      grok.append(id);
      grok += "&";
    }
    if(menu_id)
    {
      grok += "menu_id=8";
      grok.append(menu_id);
      grok += "&";
    }
    grok += "addPageItems=true'>Add Page Paragraph Items</a>";
    script << grok;
    script << makeStaticBox("Id", "id", id, "12")
           << "<br class='clearall'>" << endl
           << makeComboBox( "Site", "site_id", site_id, 
                            sql.c_str() );
    sql = "select id, name from metasite.menus where id = ";
    sql.append(menu_id);                         
    script << makeComboBox( "Menu", "menu_id", menu_id, 
                            sql.c_str() )
           << "<br class='clearall'>" << endl
           << makeTextBox("locate_tag","locate_tag",locate_tag,"150","25")
           << makeTextBox("target_tag","target_tag",target_tag,"150","25")
           << "<br class='clearall'>" << endl;

    script << makeMetaBox("template_tag","template_tag",template_tag,"35","","menu items")
           << "<br class='clearall'>" << endl;
    script << formTemplate.getParagraph("advanced_end");

    if( script.ClientArguments().count("addPageItems") == 1 ) addPageParagraphMenuItems();

    script << makeTextBox("place_order","place_order",place_order,"8")
           << "<br class='clearall'>" << endl
           << makeTextBox("place_level","place_level",place_level,"8")
           << "<br class='clearall'>" << endl;
    sql = "select id, name from metasite.links where site_id ";
    sql += siteLimitation;  
    sql += " order by name";
    script << makeComboBox( "Link", "link_id", link_id, sql.c_str() ); 
    script << "&nbsp; <a href='javascript:addLink()'>Create</a>";
    if( link_id ) script << "&nbsp; <a href='javascript:updateLink()'>Update</a>";
    script << "<br class='clearall'>" << endl;

    script << makeButtons( key() );
    script << makeBottom( m_result ) << endl; 
    return breturn;
  }
};


int main( int argcount, char ** args )
{
  cgiScript script( "text/html", false );
  menu_item_form myFrm(script); 

  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgoo;    
    pgoo.load("Templates/adminPane.htmp");
    
    script << ocString(pgoo.getParagraph("top"))
            .replaceAll("$heading$","SITES")
            .replace("// More Functions?",
                     "var xdoc = myDom.newXMLHttpRequest();"
                     "function addLink(){\n"
                     "  jopen('link_ui.cgi','scrollbars,resizable,width=450,height=300','link');\n"
                     "}\n"
                     "function updateLink(){\n"
                     "  var link='link_ui.cgi?Id=';"
                     "  link += myDom.getElementById('link_id').value;\n"                     
                     "  jopen(link,'scrollbars,resizable,width=450,height=300','link');\n"
                     "}\n");

    myFrm.loadControlTemplates("Templates/childdivform.htmp"); 

    myFrm.form_action();
    if( myFrm.menu_id && myFrm.template_tag.length() == 0 )
    {
      // get the template name from the menu_id
      menu menuObj;
      menuObj.key(myFrm.menu_id);
      if( menuObj.get_data() )
      {
        ocString sel = "select path from metasite.sites where id = ";
        sel.append(menuObj.site_id);
        string dirpath = tableLookup( sel );
        string templatePath = dirpath + "/Templates/" + menuObj.reference_template;
        
        cgiTemplates cTemp;
        if( cTemp.load(templatePath.c_str()) )
        {
          // get the control section
          xmlParser parser( cTemp.getParagraph("control") );
          parser.parse();
          node_vector::iterator & it = parser.findFirstNodeByName( "menu" );
          int order=0;
          while( it !=  parser.states.nodes.end() )
          {
            xmlNode & node = *it;
            string name_tag = node.attr["name"];
            string open_tag = node.attr["open"];
            string close_tag = node.attr["close"];
            if( name_tag == menuObj.name || ( open_tag ==  menuObj.template_open_tag &&  close_tag ==   menuObj.template_close_tag ) )
            {
              myFrm.template_tag = node.attr["items"];
            }
            it = parser.findNextNodeByName( "menu" );
          }
        }
      }
    }

    myFrm.form_display();

    
    ocString end = pgoo.getParagraph("bottom");
    script << end;
      
  }
  else
  {
    script.Redirect("signIn.html"); 
  } 
}

// compile implementations here
#include "read_write_base.cpp"
#include "forms_base.cpp"
