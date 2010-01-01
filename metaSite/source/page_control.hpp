/* 
  Page Control Class - page_control.hpp
  ----------------------------------------  
  This class represents 
  an abstraction of a web pages menus.  
*/

// Sentry
#ifndef Web_Page_Control_Hpp
#define Web_Page_Control_Hpp

/*
  This handler is installed if the menu template has 
  paragraph_key and paragraph_item attributes.
  
  paragraph_level is optional, indicating at what level the
  handler begins inserting paragraph links in the menu.
  

class paragraph_menu_handler: public base_pg_handler
{  
public:
  long level;
  paragraph_menu_handler(page & ipg):base_pg_handler(ipg),level(0L){;}
  virtual ~paragraph_menu_handler(){;}
  
  virtual  bool operator()( void )
  {     
    page_paragraphs & ps = pg.get_page_paragraphs();
    page_menus & ms = pg.get_menus();    
    
    if( ms.get_sel_item().place_level() >= level )
    {    
      paragraph_vector::iterator pos;
      for( pos=ps.begin(); pos!=ps.end(); ++pos )
      {  
        // a - at the beginning will signify 'subtract this from the list'
        if( pos->name()[0] != '-' )
        {
          // Archtype: <a class="navl$level" href="$location">$label</a>
          ocString link = "#paragraph_"; // # + (exactly as paragraph id is built)
          link.append( pos->place_order());
          ocString newLevel;
          newLevel.append(ms.get_sel_item().place_level()+1);
          pg.get_script() << 
            uiTemplate.replace("$level", newLevel.c_str())
                      .replace("$location",link.c_str())
                      .replace("$label",pos->name().c_str()) << endl;
        }            
      }
    }
    return true;  
  }
};
*/
#include "ocXML.h"

class base_functor 
{
 protected:
  page & pg;
 public:
  base_functor(page & ipg):pg(ipg){;}
  virtual ~base_functor(){;}
  
  virtual bool operator()( xmlNode & node )
  {
    return false;
  }
  virtual string getXmlAttribute( xmlNode & node, string name )
  {
    string sRet;           
    node_attr::iterator x = node.attr.find(name);   
    if( x!=node.attr.end())
    {
      sRet=x->second;        
    }   
    return sRet;
  }
};

class top_functor: public base_functor
{
public:
  top_functor(page & ipg):base_functor(ipg){;}
  virtual ~top_functor(){;}
  
  virtual  bool operator()( xmlNode & node )
  {
    return pg.emitTop();    
  }
};

class end_functor: public base_functor
{
public:
  end_functor(page & ipg):base_functor(ipg){;}
  virtual ~end_functor(){;}
  
  virtual  bool operator()( xmlNode & node )
  {
    return pg.emitEnd();    
  }
};
class menu_functor: public base_functor
{
public:
  menu_functor(page & ipg):base_functor(ipg){;}
  virtual ~menu_functor(){;}
  
  virtual  bool operator()( xmlNode & node )
  {
    string open, close;        
    node_attr::iterator x = node.attr.find("open");    
    if( x!=node.attr.end() )
    {
      open=x->second;
      x = node.attr.find("close");   
      if( x!=node.attr.end())
      {        
        close=x->second;
        // start fresh
        cleanHandlerMap( pg.get_menus().handlers );              
        /*
        // For listing this pages paragraphs paragraph_key='pkey'
        x = node.attr.find("paragraph_key");
        if( x!=node.attr.end())
        {
          paragraph_menu_handler * pHandler = new paragraph_menu_handler(pg);
          pg.get_menus().handlerKey = x->second;
          x = node.attr.find("paragraph_item");
          
          if( x!=node.attr.end())
          {            
            pHandler->uiTemplate = pg.paragraph_string(x->second);
            x=node.attr.find("paragraph_level");
            if( x!=node.attr.end()) pHandler->level = atol(x->second.c_str());
            pg.get_menus().handlers.insert( make_pair( pg.get_menus().handlerKey, pHandler ) );
          }
          else
          {
            delete pHandler;
          }                                           
        }
        */
        string boundStart, boundEnd;
        x = node.attr.find("child_open");
        if( x!=node.attr.end()) boundStart = x->second;
        x = node.attr.find("child_close");
        if( x!=node.attr.end()) boundEnd = x->second;
        pg.emitMenu( open, close, boundStart, boundEnd );        
      }   
    }
    return true;  
  }
};
class content_functor: public base_functor
{
public:
  content_functor(page & ipg):base_functor(ipg){;}
  virtual ~content_functor(){;}
  
  virtual  bool operator()( xmlNode & node )
  {
    bool bRet = false;
    string items, replace_tag;        
    node_attr::iterator x = node.attr.find("items");
    if( x!=node.attr.end() )
    {
      items=x->second;
      x = node.attr.find("replace-tag");
      if( x==node.attr.end())
      { // try this
        x = node.attr.find("replace_tag"); 
      }  
      if( x!=node.attr.end())
      {
        replace_tag=x->second;
        bRet = pg.emitContent( items, replace_tag );        
      }
      else
      {
        cout << "no content replace tag" << endl;
      }       
    }
    else
    {
      cout << "no content items" << endl;
    }
    return bRet;    
  }
};

class spacer_functor: public base_functor
{
public:
  spacer_functor(page & ipg):base_functor(ipg){;}
  virtual ~spacer_functor(){;}
  
  virtual  bool operator()( xmlNode & node )
  {
    bool bRet = false;
    string name, replace_tag;        
    node_attr::iterator x = node.attr.find("name");
    if( x!=node.attr.end() )
    {
      name=x->second;
      bRet = pg.emitSpacer( name );             
    }
    else
    {
      cout << "no spacer name specified" << endl;
    }
    return bRet;    
  }
};
typedef map< string, base_functor *> pf_map;

class page_control
{
protected:
  
  page & pg;   
  string control; 
  pf_map function_map;
public:
  page_control(page & ipg):pg(ipg),control(pg.loaded_control_string())
  {
    function_map.insert( make_pair( string("top"), new top_functor(pg) ) );
    function_map.insert( make_pair( string("end"), new end_functor(pg) ) );
    function_map.insert( make_pair( string("menu"), new menu_functor(pg) ) );
    function_map.insert( make_pair( string("content"), new content_functor(pg) ) );
    function_map.insert( make_pair( string("spacer"), new spacer_functor(pg) ) );
  }
  ~page_control()
  {
    pf_map::iterator pos;
    for( pos=function_map.begin(); pos!=function_map.end(); ++pos )
    {
      delete pos->second;
    }
  }
  page_control & addOp ( string name,  base_functor * op)
  {
    function_map.insert( make_pair( name, op ) );
    return *this;
  }
  virtual bool emit( void )
  {
    bool bRet = false;
    xmlParser ctrlParse(control);
    ctrlParse.parse();
    node_vector & xnodes = ctrlParse.nodeList();
    int i;

    // start with 1 to skip xml container
    for(i=1;i<xnodes.size();i++)
    {
      xmlNode & node = xnodes[i];
      bRet = emit( node );      
    }
    return bRet;
  }
  
  bool emit( xmlNode & node )
  {
    string name = node.name;
    pf_map::iterator pos = function_map.find(name);
    if( pos!=function_map.end() )
    {
      base_functor & rFunc = * pos->second;      
      rFunc(node);
    }
  }
};

#endif 
