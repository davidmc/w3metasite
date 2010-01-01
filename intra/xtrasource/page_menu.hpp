/*
  Page Menu Class and list - page_menu.hpp
  ---------------------------------------- 
  This class represents
  an abstraction of a web pages menus.

*/

// Sentry
#ifndef Web_Page_Menu_Hpp
#define Web_Page_Menu_Hpp

// includes
#include "cgiTemplates.h"
#include "read_base.hpp"
#include <vector>
#include <map>
#include <stack>
#include "pg_handlers.hpp"

using namespace std;

/* 
  Menu_Item class
  This class is a combination of 
   two database tables: menu_link and menu_item.
   They're combined for ease of access in 
   this read only environment.
  On the administration side, the classes should be distinct. 
*/   
class menu_item
{
protected: 
  // menu item properties 
  long long m_id;       // menu item id
  long long m_site_id;
  long long m_menu_id;
  long long m_link_id;
  string m_template_tag; // tag that identifies paragraph
  string m_locate_tag;   // tag for url replacement
  string m_target_tag;   // tag for target replacement
  string m_label_tag;    // tag for label replacement
  long m_place_order;    // vertical position
  long m_place_level;    // horizontal position and hiding
  // link properties
  string m_name;         // menu text
  string m_url;          // link url to place
  string m_target;       // target to place, if any

   
public:
  
  menu_item()
  :m_id(0),m_site_id(0),m_menu_id(0),m_link_id(0)
  ,m_place_order(0),m_place_level(0)
  {
    ;
  }
  virtual ~menu_item()
  {
    ;
  }
  menu_item & operator = ( const menu_item & in )
  {
    id(in.id());       
    site_id(in.site_id());
    menu_id(in.menu_id());
    link_id(in.link_id());
    template_tag(in.template_tag()); 
    locate_tag(in.locate_tag());   
    target_tag(in.target_tag());   
    label_tag(in.label_tag());    
    place_order(in.place_order());    
    place_level(in.place_level());  
    name(in.name());         
    url(in.url());          
    target(in.target()); 
    return *this; 
  } 
  
  // Property Access Methods
  
  // get
  long long id(void)const{return m_id;}       
  long long site_id(void)const{return m_site_id;}
  long long menu_id(void)const{return m_menu_id;}
  long long link_id(void)const{return m_link_id;}
  string template_tag(void)const{return m_template_tag;}
  string locate_tag(void)const{return m_locate_tag;}  
  string target_tag(void)const{return m_target_tag;}   
  string label_tag(void)const{return m_label_tag;}   
  long place_order(void)const{return m_place_order;}   
  long place_level(void)const{return m_place_level;}  
  string name(void)const{return m_name;}        
  string url(void)const{return m_url;}         
  string target(void)const{return m_target;} 
              
  // set
  void id(long long in){m_id=in;}       
  void site_id(long long in){m_site_id=in;}
  void menu_id(long long in){m_menu_id=in;}
  void link_id(long long in){m_link_id=in;}
  void template_tag(string in){m_template_tag=in;} 
  void locate_tag(string in){m_locate_tag=in;}   
  void target_tag(string in){m_target_tag=in;}   
  void label_tag(string in){m_label_tag=in;}    
  void place_order(long in){m_place_order=in;}    
  void place_level(long in){m_place_level=in;}  
  void name(string in){m_name=in;}         
  void url(string in){m_url=in;}          
  void target(string in){m_target=in;} 
     
};

// for item list
typedef vector< menu_item> menu_items;

// for tracking nodes to selected menu item, level order
typedef map< long, long> node_walk;

class page_menu
{
private:

protected:

  // Properties
  long long m_id;
  long long m_site_id;
  long long m_page_menu_id;
  long long m_page_id;
  string m_name;
  string m_template_open_tag;
  string m_template_close_tag;
  long m_place_order;
  bool m_collapsible;

  // site info
  string m_domain;
  string m_url;
  menu_items m_links;

public:


  page_menu ()
  :m_id(0),m_site_id(0)
  ,m_page_menu_id(0),m_page_id(0)
  ,m_place_order(0),m_collapsible(false)
  ,m_domain("")
  ,m_url("")
  {;}
  ~page_menu(){;}
  page_menu & operator = (const page_menu & in)
  {    
    m_id = in.id();
    m_site_id = in.site_id();
    m_page_menu_id = in.page_menu_id();
    m_page_id = in.page_id();
    m_name = in.name();
    m_template_open_tag = in.template_open_tag();
    m_template_close_tag = in.template_close_tag();
    m_place_order = in.place_order();
    m_collapsible = in.collapsible();
    m_domain = in.m_domain;
    m_url = in.m_url;
    m_links = in.m_links; // direct access
    /*
    ServerProtocol: HTTP/1.1
    domain devlinux
    relpath /
    */
    return *this;    
  }

  // Property Access Methods
  // get
  long long id(void)const{return m_id;}
  long long site_id(void)const{return m_site_id;}
  long long page_menu_id(void)const{return m_page_menu_id;}
  long long page_id(void)const{return m_page_id;}
  string name(void)const{return m_name;}
  string template_open_tag(void)const{return m_template_open_tag;}
  string template_close_tag(void)const{return m_template_close_tag;}
  long place_order(void)const{return m_place_order;}
  bool collapsible(void)const{return m_collapsible;}

  // non const get, means the elements are changeable
  menu_items & links(void){return m_links;}

  // set
  void id(long long in) {m_id=in;}
  void site_id(long long in) {m_site_id=in;}
  void page_menu_id(long long in) {m_page_menu_id=in;}
  void page_id(long long in) {m_page_id=in;}
  void name(string in) {m_name=in;}
  void template_open_tag(string in) {m_template_open_tag=in;}
  void template_close_tag(string in) {m_template_close_tag=in;}
  void place_order(long in) {m_place_order=in;}
  void collapsible(bool in) {m_collapsible=in;}

  // get
  string & domain( void ) { return m_domain; }
  // set
  void domain( const string & in ) { m_domain = in; }

    // get
  string & url( void ) { return m_url; }
  // set
  void url( const string & in ) { m_url = in; }

};

typedef map<string,page_menu> page_menu_map;

class page_menus : public page_menu_map, protected read_base
{
  // the menu currently being built
  string current_menu;
  // The current selected menu item (if any)
  menu_item sel_item;
  // Is there a current selected menu item
  bool isSelected;
  // 12/6/06 new for dynamic menus (typically for putting divs around child menu items)
  string m_boundStart;
  string m_boundEnd;
  // 8.2007 for child item binding
  long last_saw_children_at_level;
  
public:
  pgHandlerMap handlers;  // used to extend the functionality of the menu
  string handlerKey;

  page_menus()
  :page_menu_map(),read_base(),isSelected(false),last_saw_children_at_level(0)
  {
    ;
  }
  virtual ~page_menus()
  {
    cleanHandlerMap(handlers);
  }
  // query for all menus under this page id 
  string menuSql( long long pgid )
  {
    ocString sql = "select m.id,m.site_id,pm.id, "   // 0, 1, 2
                   "pm.page_id,m.name,m.template_open_tag, " // 3, 4, 5
                   "m.template_close_tag,pm.place_order, " // 6, 7
                   "pm.collapsible, s.domain, s.url " // 8, 9, 10
                   "from menus m "
                   "inner join page_menus pm on "
                   "pm.menu_id = m.id "
                   "inner join sites s on s.id = m.site_id "
                   "where pm.page_id = ";
    sql.append(pgid);
    sql += " order by pm.place_order ";    
    return sql;
  }
  // Get the selected menu item,  this page.
  string linkQ1( long long menu_id, string pathInfo )
  {
    ocString sql = 
      "select l.name, l.url, "
      "mi.id, mi.place_order, mi.place_level "
      "from links l " 
      "inner join menu_items mi on mi.link_id = l.id "
      "where mi.menu_id = ";
    sql.append(menu_id);
    sql += " and ( l.url = '";
    sql += baseUrl;
    sql += pathInfo; 
    sql +="' or l.url  like '%"; // accomodate absolute URL's
    sql += getenv("SERVER_NAME");
    sql += getenv("SCRIPT_NAME");
    sql += pathInfo;
    sql +="')";
    // DBG 
    cerr <<  "Q1: " << sql << endl;
    return sql;
  }
  void setSelectItem(menu_item & mi)
  {    
    mi.name(rs.getField(0).format());
    mi.url(rs.getField(1).format());
    mi.id(atoll(rs.getField(2).format().c_str()));
    mi.place_order(atol(rs.getField(3).format().c_str()));
    mi.place_level(atol(rs.getField(4).format().c_str()));
    
  }  
  // node walk to the selected item
  string linkQ2( long long menu_id, long place_order, long place_level )
  {
    ocString sql = "select max(mi.place_order),  mi.place_level "
      " from links l " 
      " inner join menu_items mi on mi.link_id = l.id "
      " where mi.menu_id = ";
    sql.append(menu_id);  
    sql += " and mi.place_order <= ";
    sql.append(place_order);
    sql += " and mi.place_level <= ";
    sql.append(place_level);
    sql += " group by mi.place_level "
           " order by mi.place_order"; 
    // cerr <<  "Q2: " << sql << endl;
    return sql;
  }
    
  node_walk walk;
  bool findNodesToWalk( page_menu & pm, string & pathInfo )
  {
    bool bret=true;
    long currentLevel = 1, lastParentPos = 0;    
        
    if( isSelected && pm.collapsible()  )
    {
      // Get parent items of selected items, put in level map
      for( bool bopen = rs.open( linkQ2( pm.id(), 
                                    sel_item.place_order(), 
                                    sel_item.place_level() ) ); 
          bopen; 
          bopen=rs.next() ) 
      {
        long place_order = atol(rs.getField(0).format().c_str());
        long place_level = atol(rs.getField(1).format().c_str());
        walk.insert( make_pair(place_order,place_level) );
      }
      rs.close();              
    }  
   
    return bret;    
  }  
  // for menu population
  string linkQ3( long long menu_id )
  {
    ocString sql = "select l.id, l.name, l.url, l.target,"
      " mi.id, mi.template_tag, mi.locate_tag, mi.target_tag, "
      "mi.label_tag, mi.place_order, mi.place_level "
      " from links l "
      " inner join menu_items mi on mi.link_id = l.id"
      " where mi.menu_id = ";
    sql.append(menu_id);  
    sql += " order by mi.place_order";
       
    return sql;
  }  
  void setMenuItem(menu_item & mi)
  {
    mi.link_id(atoll(rs.getField(0).format().c_str()));
    mi.name(rs.getField(1).format());
    mi.url(rs.getField(2).format());
    mi.target(rs.getField(3).format());
    mi.id(atoll(rs.getField(4).format().c_str()));
    mi.template_tag(rs.getField(5).format());
    mi.locate_tag(rs.getField(6).format());
    mi.target_tag(rs.getField(7).format());
    mi.label_tag(rs.getField(8).format());
    mi.place_order(atol(rs.getField(9).format().c_str()));
    mi.place_level(atol(rs.getField(10).format().c_str()));

  }
  void pageMenuPropSet( page_menu & pm  )
  {
    pm.id(atoll(rs.getField(0).format().c_str()));
    pm.site_id(atoll(rs.getField(1).format().c_str()));
    pm.page_menu_id(atoll(rs.getField(2).format().c_str()));
    pm.page_id(atoll(rs.getField(3).format().c_str()));
    pm.name(rs.getField(4).format());
    pm.template_open_tag(rs.getField(5).format());
    pm.template_close_tag(rs.getField(6).format());
    pm.place_order(atol(rs.getField(7).format().c_str()));
    pm.collapsible(atol(rs.getField(8).format().c_str())!=0);
    pm.domain(rs.getField(9).format().c_str()); 
    pm.url(rs.getField(10).format().c_str());
  }
  bool load( long long page_id )
  {
    bool bopen;
    // for each menu
    for( bopen=rs.open(menuSql(page_id)); 
         bopen; 
         bopen=rs.next() )
    {       
      // instance a page menu
      page_menu pm;
      // set the page menu properties
      pageMenuPropSet(pm);
      // add the page menu to the collection
      insert(make_pair(pm.name(),pm));
    }  
    // close the recordset
    rs.close();
    // All done
    return true;
  }
  /*
    Emits the bounding begin or end of a menu
  */
  bool emitMenuBoundary( paragraphMap & paras, 
                         string & Tag, 
                         ostream & toBrowser )
  {
    bool breturn=false;    
    paragraphMap::iterator pos = static_cast<paragraphMap &>(paras).find(Tag);
    if( pos != paras.end() )
    {
      ocString bndry = pos->second;
      if( bndry.length() )
      {
        ocString menuItemId;
        menuItemId.append(sel_item.id());
        if(isSelected)
          toBrowser << bndry.replace("$selected$",menuItemId.c_str());
        else
          toBrowser << bndry.replace("$selected$","0"); // nothing selected
      }
    }
    else
    {
      toBrowser << "<!-- Can't find menu tag " 
                << Tag 
                << endl
                << "Check that the data matches the file template. -->"
                << endl;
    }    
    return breturn;
  }

  /*
       (return the fixed up url) string
       fullyQualifiedFixup( page_menu & pm,
                            menu_item & mi )

        New!
         all links translated to absolute URLs...
         (unless they specify base relative)
  */
  string fullyQualifiedFixup( page_menu & pm,
                              menu_item & mi )
  {
    ocString url = mi.url();
    // Good if not external link and/or fully qualified
    if ( url.find( ":") == string::npos )
    {

      if( url[0] != '/' )
      {
        string tmpUrl;
        char * port = getenv("SERVER_PORT");
        if( port && strlen(port) )
        {
          int iport = atol(getenv("SERVER_PORT"));
          if( iport > 88 )
          {
            tmpUrl = "https://";
          }
          else
          {
            tmpUrl = "http://";
          }
          tmpUrl += pm.domain();
          if( iport != 80 && iport != 443 )
          {
            // specify the (unusual) port
            tmpUrl += ":";
            tmpUrl += port;
          }
          tmpUrl += pm.url();
          tmpUrl += url;
          url = tmpUrl;
        }

      }
    }
    return url;
  }
  // Emit a single menu item
  bool emitItem( paragraphMap & paras,
                 page_menu & pm,
                 menu_item & mi,
                 ostream & toBrowser,
                 bool defaultPage, long long last_id )
  {
    bool breturn=false;    
    paragraphMap::iterator pos = paras.find(mi.template_tag());
    if( pos != paras.end() )
    {
      breturn = true;
      string base = baseUrl;      
      if( base.length() ) base += "/";
      ocString lastId;
      lastId.append(last_id);
      ocString url = fullyQualifiedFixup(pm,mi);
      
      ocString item = pos->second;
      ocString uniqueId = "menu_item_";
      uniqueId.append(mi.id());
           // what about place_level ???
      ocString place_level;
      place_level.append(mi.place_level());

      toBrowser << item.replaceAll("$id",uniqueId.c_str() )
                       .replaceAll("$level",place_level.c_str() )
                       .replaceAll("$last_id",  lastId )
                       .replaceAll(mi.locate_tag().c_str(),
                                url.c_str() )                                
                       .replace(mi.target_tag().c_str(),mi.target().c_str())
                       .replaceAll(mi.label_tag().c_str(),mi.name().c_str());
    }
    else
    { 
      toBrowser << "<!-- Can't find menu item tag " 
                << mi.template_tag() 
                << endl
                << "Check that the data matches the file template. -->"
                << endl;
    }    
    return breturn;
  }

  bool getSelected(string & pathInfo)
  {
    page_menu_map::iterator it = find(current_menu);
    if( it != end() )
    {
      page_menu & pm = it->second;
      isSelected=false;

      // Get current selected item ( if any )
      // If any selected item
      string sql = linkQ1( pm.id(), pathInfo );

      if( rs.open( sql )  )
      {
        // DBG cerr << "Selected!" << endl;
        // Set bSelected true
        isSelected=true;

        // Set selected item properties:
        setSelectItem(sel_item);

        rs.close(); 
      }
    }
    return isSelected;  
  }
  /*
    found this item in the list of nodes to walk (ancestor nodes toward the selected item)
  */
  bool nodeToWalkDetected( menu_item & mi )
  {
    bool bFound = false;
    node_walk::iterator pos = walk.find( mi.place_order() );
    if( pos != walk.end() && pos->second == mi.place_level() )
    {
      bFound = true;
    }
    return bFound;
  }

  void childMenuHandler( void )
  {
    if( handlerKey.length() )
    {
      pgHandlerMap::iterator pos = handlers.find(handlerKey);
      if( pos != handlers.end() )
      {
        (*(pos->second))();
      }
    }
  }
  void emitBoundary(ostream & toBrowser, paragraphMap & paras, string name, long long id, long level )
  {
    if( name.length() )
    {
      paragraphMap::iterator pos = paras.find(name);
      if( pos != paras.end() )
      {
        ocString strId = "";
        ocString strLvl = "";
        strId.append(id);
        strLvl.append(level);
        ocString templt = pos->second;
        toBrowser << templt.replaceAll("$id",strId).replaceAll("$level",strLvl);
      }
    }
  }

  // Added child boundaries to contain children - useful for dynamic menus and relocating child menus
  void emitChildBoundary( long place_level, long last_level, ostream & toBrowser, paragraphMap & paras, long long last_id )
  {
    int bndryCount = place_level -  last_level;
    // DBG     toBrowser << "<!-- place level " << place_level << " last_level: " <<  last_level << " --> ";
    // emit as many child boundaries as needed on the way up
    for( int i=0; i<bndryCount; i++ )
    {
      emitBoundary(toBrowser,paras, m_boundStart, last_id, last_level);
      last_saw_children_at_level++;
    }
    // emit as many boudary closures as needed on the way down
    for( int i=bndryCount; i<0; i++ )
    {
      emitBoundary(toBrowser,paras, m_boundEnd, last_id, last_level);
      last_saw_children_at_level--;
    }
    // this is just in case
    if( last_saw_children_at_level < 1 )   last_saw_children_at_level = 1;
  }

  bool emitRecursor( int currentLevel,
                     paragraphMap & paras,
                     page_menu & pm, 
                     ostream & toBrowser,
                     bool defaultPage )
  {
    bool bret=true;
    bool seeChildren = false;
    bool sawChildren = false;
    bool previousSelected = false;
    long long last_id = 0;
    long long last_parent_id = 0; // for case of non-collapsible, to keep up with parents, for client side menu scripting
    vector<long long> last_parents; // keep up by pushing and popping, no recursion for no collapse
    long last_level = currentLevel;
    menu_item mi;
    do
    {
      setMenuItem(mi); 
      previousSelected = isSelected && sel_item.id() == mi.id();
      if( pm.collapsible() )
      {

        if( mi.place_level() == currentLevel ) // at caller level
        {
          emitItem( paras, pm, mi, toBrowser, defaultPage, last_parent_id );
          seeChildren = nodeToWalkDetected( mi ); // level change detection
        }
        else if( mi.place_level() > currentLevel ) // at higher level  (sub-menu start)
        {

          if( seeChildren == true &&
              sawChildren == false )
          {
            // REALLY RECURSE!
           emitChildBoundary( mi.place_level(), last_level, toBrowser, paras, last_id );
           sawChildren = emitRecursor( mi.place_level(),
                                       paras,
                                       pm,
                                       toBrowser,
                                       defaultPage );
          }
        }
        else  // back to parent level
        {
          emitChildBoundary( mi.place_level(), last_level, toBrowser, paras, last_id );          
          emitItem( paras, pm, mi, toBrowser, defaultPage, last_parent_id );
          break;
        }
      }
      else // not collapsible
      {
        // DBG toBrowser << "<!-- last_level: " << last_level << ",  mi.place_level(): " <<  mi.place_level() << " -->" << endl;
        if( last_level < mi.place_level() )
        {
          last_parent_id = last_id;
          last_parents.push_back(last_parent_id);
        }
        if( last_level > mi.place_level() )
        { last_parent_id = 0;
          if(  last_parents.size() )
          { last_parents.pop_back();
            if(  last_parents.size() )
              last_parent_id = last_parents.back();
          }
        }
        emitChildBoundary( mi.place_level(), last_level, toBrowser, paras, last_id );
        emitItem( paras, pm, mi, toBrowser, defaultPage , last_parent_id);
      }
      if( previousSelected ) 
      {
        childMenuHandler(); // this is for special menus, local links, menu embedded catalogs, and such.
      }
      // for the detection of children
      last_id = mi.id();
      last_level = mi.place_level();
    }
    while( rs.isOpen() && rs.next() );
    return bret;
  }
  
  

  // Emit the menu items
  bool emitItems( paragraphMap & paras,
                  page_menu & pm, 
                  ostream & toBrowser, 
                  string & pathInfo,
                  bool defaultPage)
  {    
    bool breturn=false, bopen=false;     
    
    findNodesToWalk( pm, pathInfo );
    
    if( rs.open( linkQ3( pm.id() ) ) )
    {
      last_saw_children_at_level = 1;
      breturn = emitRecursor( 1,
                              paras,
                              pm, 
                              toBrowser, 
                              defaultPage );
      emitChildBoundary( 1, last_saw_children_at_level, toBrowser, paras, 0 );
      rs.close();
    }
    return breturn;
  }  
  
  // new emit based on template control
  bool emit( string name,
             paragraphMap & paras,
             ostream & toBrowser, 
             string pathInfo,
             bool defaultPage,
             string startTag, 
             string endTag,    
             string boundStart = "", string boundEnd="")
  {
    bool breturn=false; 
    m_boundStart = boundStart;
    m_boundEnd = boundEnd;
    
    page_menu_map::iterator it = find(name);
    
    // this determines the menu item that might be selected
    if( it != end() )
    {
      current_menu = name;
      getSelected(pathInfo);
    } 
    else cerr << "could not find Menu  Name: " << name << endl;
    // DBG  cerr << "Menu Name: " << name << " sel id: " << sel_item.id() << endl;
    
    // emit the top boundary
    emitMenuBoundary( paras, startTag, toBrowser );

    if( it != end() )
    {
      page_menu & pm = it->second;
      emitItems(paras,pm,toBrowser,pathInfo,defaultPage);
      breturn=true;
    }
    // emit the bottom boundary      
    emitMenuBoundary( paras, endTag, toBrowser );      
   
    return breturn;  
  } // end emit   
  
  // return the selected item. ie: this pages link in the menu
  menu_item & get_sel_item()
  { 
    return sel_item; 
  }
};

#endif

