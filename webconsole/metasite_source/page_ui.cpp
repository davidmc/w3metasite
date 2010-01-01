/*
  Copyright 2002 - 2004 by D.K. McCombs.
  ======================================
    
*/

#include <iostream>
#include <iomanip>
#include "cgiTemplates.h"
#include "connectstring"
#include "forms.h"
#include "ocTypes.h"
#include "ocString.h"
#include "read_write_base.hpp"
#include "page.hpp" 
#include "forms_base.hpp"
#include "list_base.hpp"
#include "openLogin.h"
#include "siteLimit.h"
#include "ocXML.h"
#include "openLogger.h"
using namespace std;
openLogin oLogin;


class page_item_List:  public list_base
{
public: 
  long long id;
  long long site_id;
  ocString defaultElement;
  // Constructor
  page_item_List(cgiScript&sc, long long id, long long site_id, string & defElement)
  :list_base(sc),id(id),site_id(site_id),defaultElement(defElement)
  {
    ;                   
  }  
  ~page_item_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;
    editLink = listTemplate.getParagraph("hotcolumn");
    editLink = editLink.replace("$prog$","paragraph_ui.cgi");
    hotCol=-2; skipCol=0;   
    ocString  sql = "select id ,name, content, template_tag, place_order "
                    "from metasite.paragraphs "
                    "where page_id = ";
    sql.append(id); 
    sql += " and  template_tag in ('";
    sql += defaultElement.replaceAll(",","','");  
    sql += "') order by place_order";    
    webIO << "<!-- " << sql << "-->" << endl;  

    emitHeadings("Name|Content|Style|Order"); 
    getData(sql);
    emitData();    
    emitEnd();
    return breturn;
  }
  void derived_complete_event( void )
  {     
    if(id)
    {
      ocString DefaultElement = defaultElement.parse(",");
      webIO << "<tr><td colspan='3' style='text-align:right; width: 500px;'>" << endl;
      webIO << "  New Item by Style: &nbsp; " << endl;
      while( DefaultElement.length() )
      {
        ocString loco = "paragraph_ui.cgi?page_id=";
        loco.append(id);
        loco += "&site_id=";
        loco.append(site_id);
        loco += "&section=";
        loco += defaultElement;
        loco += "&template_tag=";
        loco += DefaultElement;
        webIO << "<a href=\"javascript:jopen('" <<  loco 
              <<  "')\">" << DefaultElement 
              <<  "</a> &nbsp;" << endl;
        DefaultElement = defaultElement.parse(",");
      }
      webIO << "</td></tr>" << endl;
    }                    
  } 
  
  // override this to get the group_id in the link
  virtual void sendHotField( int iField, ocString & td )
  {
    ocString linkId = rs.getField(0).format();
    if(id)
    {
      linkId += "&page_id=";
      linkId.append(id);
    }

    string data; 
    if( iField == 0 )
    {
      // do nothing
    }    
    else if(iField == 2 )
    { 
      ocString Parse(rs.getField(iField).format());
      
      while( Parse.endOfParse() == false )
      {
        data += Parse.parse( "<" );
        data += " ";
        Parse.parse( ">" );
      }
    }
    else
    {
      data = rs.getField(iField).format(); 
    }
    if( data.length() )
    {
      webIO <<  td.replace( "$data$",
                    editLink.replaceAll( "$key$", linkId.c_str() )
                            .replaceAll( "$col$", data.c_str() ).c_str());
    }                            
  }  
  
};

struct page_menu
{
  string id;
  string name;
  page_menu(const string & iid, const string & iname)
  {
    id=iid;
    name=iname;
  }
  ~page_menu(){;}
  page_menu & operator = ( const page_menu & in )
  {
  id=in.id;
  name=in.name;
  return *this;
  }
};
typedef vector<page_menu> page_menus;

/* 
  controlLayout
  ============================
  model the layout of the page 
  as determined by the control section
  of the page template
  
*/
class controlLayout
{  
public:
  
  string csPath;
  cgiCookie cookie;
  node_vector nodes;
  page_menus menus;
  long long page_id;
  long long site_id;
  controlLayout( long long template_id, long long page_id, long long site_id  )
  :csPath("Templates"),page_id(page_id),site_id(site_id)
  { 
    cgiTemplates chosenTemplate;
    // cookies valid for entire site
    cookie.setPath("/"); 
    // check for cookie
    string strSite_id = cookie.get("site_id");
    string iPath;
    ocString sql = "select path from metasite.sites where id = ";
    sql += strSite_id;
    quickQuery qqry;
    openRS & rs = qqry.getRS();

    if( rs.open(sql) )
    {
      string iPath = rs.getField(0).format().c_str();
      rs.close();
      sql = "select path from metasite.templates where id = ";
      sql.append(template_id);
      if( rs.open(sql) )
      {
        string tmplt = rs.getField(0).format().c_str();
        rs.close();
        iPath += "/";
        iPath += csPath;
        iPath += "/";
        iPath += tmplt; 
        
        chosenTemplate.load(iPath.c_str());

        string controlXML = chosenTemplate.getParagraph("control");
        if( controlXML.length() )
        {
          xmlParser ctrlParse(controlXML);
          ctrlParse.parse();  
          nodes = ctrlParse.nodeList();        
        }
      }  
    }
  }
  void getMenus( long long page_id )
  {
    ocString  sql = "select pm.id, m.name "
                    "from metasite.page_menus pm inner join metasite.menus m on pm.menu_id = m.id "
                    "where pm.page_id = ";
    sql.append(page_id);
    sql += " order by pm.place_order ";
    quickQuery qqry;
    openRS & rs = qqry.getRS();
    if( rs.open(sql) )
    {
      do      
      menus.push_back(page_menu(rs.getField(0).format(),rs.getField(1).format()));
      while( rs.next() );
    } 
                  
  }
  void emit( cgiScript & script )
  {
    ocString elList;
    string defaultElement;
    int contents=0, menus=0;
    getMenus(page_id);
    if(nodes.size())
    {
      script << "<div class='extra'>" << endl;
      script << "<h4>Page Layout</h4>" << endl;
      for( int i = 0; i < nodes.size(); ++ i )
      {
        string & name = nodes[i].name;
        if( name == "menu" )
        {
          node_attr::iterator pos = nodes[i].attr.find("items");
          if( pos != nodes[i].attr.end() )
          {
            elList = pos->second;
            defaultElement = elList.parse(",");
            script << "<div class='label'>Menu " << menus+1 << ":</div>"; 
            if( menus < this->menus.size() )
            {
              script << "<a href=\"javascript:jopen('page_menu_ui.cgi?Id=";
              script << this->menus[menus].id;
              script << "&page_id=";
              script << page_id;
              script << "')\">";
              script << this->menus[menus].name;
              script << "</a>";
            }
            else
            {
              script << "<a href=\"javascript:jopen('page_menu_ui.cgi?page_id=";
              script << page_id;
              script << "')\">NEEDS TO BE ASSIGNED</a>";           
            }
            
            script << "<br class='clearall'>" << endl;
            menus++;
          }
        }
        else if( name != "xml" && name != "top" && name != "end"&&  name != "spacer")
        {
          node_attr::iterator pos = nodes[i].attr.find("items");
          if( pos != nodes[i].attr.end() )
          {
            script << "<div class='label'>Content:</div>";
            elList = pos->second;
            page_item_List list(script,page_id,site_id,elList);
            list.loadListTemplates("Templates/poplist.htmp");  
            list.list_display();    
            script << "<br class='clearall'>" << endl; 
            contents ++; 
          }          
        }
      }
    }
  }
};

class page_form:  public page, public forms_base
{
public: 
  page_form(cgiScript & script):page(),forms_base(script){setKey(*this);} 
  virtual ~page_form(){;}
  
  void form_id_transfer( void )
  {
    llongFXfer( "id", id );
  }
  void form_data_transfer( void )
  {
    // Make data in controls back to object
    llongFXfer( "site_id", site_id );
    llongFXfer( "template_id", template_id );    
    stringFXfer( "name", name );
    stringFXfer( "url", url );
    stringFXfer( "meta_description", meta_description );    
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
   
    script << makeTop("page_ui.cgi", "Page Editor")
           << formTemplate.getParagraph("advanced_begin")
           << makeStaticBox("Id", "id", id, "12")
           << makeComboBox( "Site", "site_id", site_id, 
                            sql );
    script << "<br class='clearall'>" << endl
           << makeTextBox("SEO Key Phrases","meta_description",meta_description, "355", "50")
           << "<br class='clearall'>" << endl
           << formTemplate.getParagraph("advanced_end");
                            
    sql = "select id, name from metasite.templates";
    if( siteLimitation.length() )
    {
      sql += " where site_id ";
      sql += siteLimitation;
    }  
    script << makeComboBox( "Template", "template_id", template_id, 
                            sql );   
    script << "<br class='clearall'>" << endl;                                            
    script << makeTextBox("Title","name",name,"50", "25")
           << "<br class='clearall'>" << endl
           << makeTextBox("URL","url",url,"255", "25")
           << "&nbsp <a href=\"javascript:jcontrol('url','urlpick.cgi')\">Create</a>";
    script << "<br class='clearall'>" << endl;        
    

    script  << makeButtons( key() );
     
    if ( id != 0 )
    {
      script << "<a style='float:right; color:green;' target='view' href='../"
             << url
             << "'>View Page</a>" << endl; 
      controlLayout layout( template_id, id, site_id );
      layout.emit(script);


    }    
    script << makeBottom( m_result ) << endl; 
    return breturn;
  }
};


int main( int argcount, char ** args )
{
  cgiScript script( "text/html", false );
  page_form myFrm(script); 

  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    cgiTemplates pgoo;    
    pgoo.load("Templates/adminPane.htmp");
    
    script << ocString(pgoo.getParagraph("top"))
            .replaceAll("$heading$","SITES");

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
    script.Redirect("signIn.html"); 
  } 
}

// compile implementations here
#include "read_write_base.cpp"
#include "forms_base.cpp"
