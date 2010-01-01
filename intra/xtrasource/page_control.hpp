/*
  Page Control Class - page_control.hpp
  ----------------------------------------  
  This class represents 
  an abstraction of a web pages menus.  
*/

// Sentry
#ifndef Web_Page_Control_Hpp
#define Web_Page_Control_Hpp

#include "ocXML.h"
#include "cgiTemplates.h"

#include "Edit_Links.hpp"

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
    cgiScript & script = pg.get_script();
    ocString top = pg.getTop();
    script <<  top.parse("<!--@edit-->");
    if( top.endOfParse() == false )
    {
      ocString editLink =  top.parse("<!--/@edit-->");
      if( isEditMode )
      {
        // DBG:  cout << editLink << " - found this between edit tags..." << endl;
        // page id
        ocString page_id;
        page_id.append(pg.id());
        if( editLink == "[EDIT]" )
        {
          editLinksInit();
          /* DBG  see if all the links are what you expect
          paragraphMap::iterator enm = editLinkCollection.getParagraphs().begin();
          while(  enm != editLinkCollection.getParagraphs().end() )
          {
            cout <<  enm->first << " is " << enm->second << endl;
            enm++;
          }
          */
          editLink = editLinkCollection.getParagraph("GLOBALEDITLINK");
          // page id
          // DBG script << " editLink is {" << editLink << "}" << endl;
        }
        // send the edit link
        script <<  editLink.replace("$id$", page_id).replace("$name$","top");
      }
      if( top.endOfParse() == false );
      script << top.remainder();
    }
    return true;
  }
};

class end_functor: public base_functor
{
public:
  end_functor(page & ipg):base_functor(ipg){;}
  virtual ~end_functor(){;}

  virtual  bool operator()( xmlNode & node )
  {
    cgiScript & script = pg.get_script();
    ocString end = pg.getEnd();

    // next provide edits for global content
    script <<  end.parse("<!--@edit-->");
    if( end.endOfParse() == false )
    {
      ocString editLink =  end.parse("<!--/@edit-->");
      if( isSignedOn )
      {
        // DBG     script << "YOU ARE SIGNED ON! " << endl;

        // page id
        ocString page_id;
        page_id.append(pg.id());
        if( isEditMode )
        {
          // send the edit link
          if( editLink == "[EDIT]" )
          {
            editLinksInit();
            editLink = editLinkCollection.getParagraph("GLOBALEDITLINK");
          }
          script <<  editLink.replace("$id$", page_id).replace("$name$","end");
        }
        if( end.endOfParse() == false )
        {

           // look for go bar stuff
           script << end.parse("<!--@sign_in-->");
           script << "<a class='golink' href='?signoff=true'>Sign Off</a>";
           /** Pass over the Sign In tag
                    <div id='go_bar'  style='overflow:hidden;color:#000;background:transparent;height:2px; '>
                    <div style="padding: 5px;"><strong>GO:</strong>  &nbsp;
                      <!--@sign_in--> ...
            */
           end.parse("<!--@/sign_in-->");
           // script << "<!-- remainder: " << end.remainder()  << " -->";
           // depending on signon do <!--auto--> replacement
           if( end.endOfParse() == false )
           {
             /** other start menu items here

               ... <span class='golink' onclick="showSignOn()">Sign On</span><!--@/sign_in-->
                &nbsp; &nbsp; &nbsp; <!--@auto--> ...
                
             */
             string tok = end.parse("<!--@auto-->");
             if( tok.length() > 0 )
             {
                script << tok;

                /* DKMc New Page Edit and permissions  Check to see if they have edit permission on this page...
                   Check site admin permission on current site.

                   If this page is not a service, let them edit.
                   If this page is a service, then check to see if they have permission to edit this page.
                    If they have permission, let them edit
                      Also, MAYBE add Go: menu items in service XML for <auto> </auto>  container (if any)
                    If no permission, don't allow edits.
                */
                if( oLogin.SiteAdmin() )
                {
                  if( isEditMode )
                  {
                    // place edit Mode, new Page, and any other control you can think of
                    script << "<a class='golink' href=\"javascript:showBuilder()\">Manage pages</a> &nbsp;" << endl;
                    script << "<a class='golink' href=\"javascript:editMode('false')\">Editing off</a> &nbsp;" << endl;
                  }
                  else
                  {
                    script << "<a class='golink' href=\"javascript:editMode('true')\">Editing on</a> &nbsp;" << endl;
                  }
                } 
             }
           }
        }
      }
      // DBG
      else script << "<!-- YOU ARE NOT SIGNED ON! " << oLogin.getLastError() << " -->" << endl;
      /**
      
         ...  &nbsp; <span id='go_bar_close'>[X]</span>
          </div>
        </div>
        
      */
      if( end.endOfParse() == false )
        script << end.remainder();
    }// DBG  else script << "YOU ARE NOT PARSING RIGHT! " << endl;
    return true;
  }
};

class menu_functor: public base_functor
{
public:
  menu_functor(page & ipg):base_functor(ipg){;}
  virtual ~menu_functor(){;}
  
  virtual  bool operator()( xmlNode & node )
  {
    string open, close, name;
    node_attr::iterator x = node.attr.find("name");
    if( x!=node.attr.end() )
    {
      name = x->second;
      node_attr::iterator x = node.attr.find("open");

      if( x!=node.attr.end() )
      {
        open=x->second;

        // OK, fixup any edit section
        // <!--@edit--><div class="editlink" onclick="navedit('Collapsible','$id$')">[edit]</div><!--/@edit-->
        string buildNavOpen;
        string & openPart = pg.get_template().getParagraph(open);
        ocString navOpenCopy = openPart;
        buildNavOpen = navOpenCopy.parse("<!--@edit-->");
        if( navOpenCopy.endOfParse() == false )
        {
          ocString editLink =  navOpenCopy.parse("<!--/@edit-->");
          if( isEditMode )
          {
            // get menu id
            ocString menu_id;
            page_menus::iterator mnuIt = pg.get_menus().find(name);
            if( mnuIt !=  pg.get_menus().end() )
            {
              page_menu & mnu = mnuIt->second;
            
              menu_id.append(mnu.page_menu_id());
              // send the edit link
              if( editLink == "[EDIT]" )
              {
                editLinksInit();
                editLink = editLinkCollection.getParagraph("NAVEDITLINK");
              }
              buildNavOpen +=  editLink.replace("$id$", menu_id).replace("$name$",name);
            }
            
          }
          if( navOpenCopy.endOfParse() == false );
            buildNavOpen += navOpenCopy.remainder();
         // finally, overwrite the menu item top with the correct nav link (or none)
         openPart =  buildNavOpen;  
        }
        
        x = node.attr.find("close");   
        if( x!=node.attr.end())
        {        
          close=x->second;
          // start fresh
          cleanHandlerMap( pg.get_menus().handlers );              
          string boundStart, boundEnd;
          // this is for child containers around menu items
          x = node.attr.find("child_open");
          if( x!=node.attr.end()) boundStart = x->second;
          x = node.attr.find("child_close");
          if( x!=node.attr.end()) boundEnd = x->second;
          pg.emitMenu( name, open, close, boundStart, boundEnd ); 
        }   
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
      if( x==node.attr.end() )
      { // try this
        x = node.attr.find("replace_tag"); 
      }
      if( x!=node.attr.end() )
      {
        replace_tag=x->second;
      }
      string section;
      x = node.attr.find("section");
      if(  x!=node.attr.end() )
      {
        section = x->second;
      }
      if( isEditMode )
      {
        editLinksInit();
        fixEditLinks( items,  pg);
      }
      bRet = pg.emitContent( items, replace_tag, section );
    }
    else
    {
      cerr << baseUrl << " no content items" << endl;
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
    cgiScript & script = pg.get_script();
    string name, replace_tag;        
    node_attr::iterator x = node.attr.find("name");
    if( x!=node.attr.end() )
    {
      name=x->second;
      bRet = true;
      ocString Spacer = pg.getSpacer( name );
      script << "<!-- spacer " <<  name  << " --> " << endl;
      script << Spacer.parse("<!--@edit-->");
      if( Spacer.endOfParse() == false )      {
        
        ocString editLink =  Spacer.parse("<!--/@edit-->");
        if( isEditMode )
        {
          // page id
          ocString page_id;
          page_id.append(pg.id());
          // send the edit link
          if( editLink == "[EDIT]" )
          {
            editLinksInit();
            editLink = editLinkCollection.getParagraph("GLOBALEDITLINK");
            // DBG  script <<  editLink;
          } 
          script <<  editLink.replace("$id$", page_id ).replace("$name$",name);
        }
        if( Spacer.endOfParse() == false );
        script << Spacer.remainder();
      }
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
