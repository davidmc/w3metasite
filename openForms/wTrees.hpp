/*

  wTrees.hpp
  ------------------
  Extensible framework for building trees.

  Need some javascript on the client to expand and collapse.
  

*/

#ifndef wTreeS_hPp
#define wTreeS_hPp

#include <iostream>
#include <iomanip>
#include "ocTypes.h"
#include "ocString.h"
#include "read_base.hpp"
#include "cgiTemplates.h"
#include "Session.hpp"
#include <set>
typedef set< string > expandedNodes;

// base type tree node
class baseTreeNode // : public read_base
{
  protected:
  ostream & webIO;


  // Temparary data to fill out the node;
  ocString treeNodeStart;
  ocString treeNodeLabel;
  ocString activeLeaf;
  ocString treeNodeEnd;
  int count;
  bool isLastNode;

  public:
    cgiTemplates & treeTemplate;
    // for persistance
    string treeId;
    // for recursion decisions
    expandedNodes * pExpandedNodes;

    baseTreeNode( cgiTemplates & templateIn, ostream & webIo )
    : webIO( webIo ),treeTemplate( templateIn ),count(0),level(0),isLastNode(false),pExpandedNodes(0)
    {
      treeNodeStart = treeTemplate.getParagraph("treeNodeStart");
      treeNodeLabel = treeTemplate.getParagraph("treeLabel");
      activeLeaf = treeTemplate.getParagraph("activeLeaf");
      treeNodeEnd = treeTemplate.getParagraph("treeNodeEnd");
    }
    virtual ~baseTreeNode() {;}
    virtual bool emit( string filter = "" ){;}

    // for replacing the active leaf to make it more flexible
    void altActiveLeaf( string leaf )
    {
      activeLeaf = leaf;
    } 
    
    // for the caller to say what they are looking for.
    ocString fields;
    ocString from;
    ocString location;
    
    // for the caller to limit child nodes
    ocString childFilter;
    int level;

    // for the caller to name the nodes
    ocString label;
    
    // for the caller to add a new item
    ocString addLink;
    string ParentId;
};
// sequenced handlers
typedef vector<baseTreeNode*> nodeHandlers;

class ocTreeNode: public baseTreeNode
{
public:
  nodeHandlers handlers;
  
  
  ocTreeNode( cgiTemplates & templateIn, ostream & webIo )
  :baseTreeNode( templateIn, webIo )
  {
    
  }
  virtual ~ocTreeNode()
  {
    for( int hi=0; hi < handlers.size(); hi++ )
    {
      // since we can recurse this may be us
      if( this != handlers[hi] )
      {
        delete handlers[hi];
      }
    }
  }

  virtual void indent( int level )
  {
    for( int i=0; i>level;++i)
    {
      webIO << "  ";
    }
  }
  string UniqID( string id )
  {
    ocString ID;
    ID.append(level);
    ID += "_";
    ID += id;
    return ID;
  }
  // gives each leaf a unique id
  virtual string NodeStart( int level, string id )
  {
    return treeNodeStart.replace("$ID",UniqID(id).c_str());
  }

  virtual string NodeLabel( string name )
  {
    return treeNodeLabel.replace("$label",name);
  }

  string buildLeafText( openRS & rs )
  {
    // convenience for multiple fields in recordset
    string label = "";
    for( int lx=1; lx<rs.getFieldCount(); lx++ )
    {
      if( label.length() && rs.getField(lx).format().length() ) label += " - ";
      label += rs.getField(lx).format();
    }
    return label;
  }
  void  emitLeaf( string icoBase, openRS & rs )
  {
    string id = rs.getField(0).format();
    string tempLoc = location.replace("$ID", id );
    webIO << activeLeaf.replace("$location", tempLoc)
                       .replace("$label", buildLeafText(rs) )
                       .replaceAll("$icon$", icoBase )
                       .replaceAll("$uid",UniqID(id))
                       .replace("$treeId$",treeId)
                       .replace("$ID",id);
  }
  string Filter( baseTreeNode * child, openRS & rs )
  {
    string temp;
    if( child && child->childFilter.length() )
    {
      temp = child->childFilter.replace("$ID", rs.getField(0).format() );
    }
    return temp;
  }
  virtual bool emit( string filter = "" )
  {
    bool bRet = false;
    // This needs to be an method instance because of select recursion
    quickQuery qqry;
    openRS & rs =qqry.getRS();
    
    // We expect id to always be the first item in the list of fields
    string qry = "select " + fields;
    qry += " from " + from;
    qry += filter;
    
    if( addLink.length() )
    {
      // add leading label here only if in add mode
      if( label.length() )
      {
        indent(level );
        webIO << NodeLabel( label );
      }
    }

    // DBG     webIO << qry << "<br>" << endl;
    if( rs.open( qry ) )
    {
      // add leading label here only if we have data and not in add mode
      if( addLink.length() == 0 && label.length() )
      {
        indent(level );
        webIO << NodeLabel( label ); 
      }
      bRet = true;
      string icoBase;
      do
      {
        string id = rs.getField(0).format();
        string tempLoc = location.replace("$ID", id.c_str());
        if( handlers.size() )
        {
          icoBase = "expand";
          if( pExpandedNodes && pExpandedNodes->find(id) != pExpandedNodes->end() )
            icoBase = "contract";
        }
        else
          icoBase="item";

        indent(level);
        emitLeaf(icoBase,rs);
 
        // are there child items?  Then recurse here
        if( handlers.size() )
        {
          indent(level);
          webIO << NodeStart( level, id.c_str() );
          for( int hi=0; hi<handlers.size(); ++hi )
          {
            bool doExpand = true;
            if(  pExpandedNodes )
            {
              // DBG webIO << "Finding " << id << "..." << endl;
              if( pExpandedNodes->find(id) == pExpandedNodes->end() )
              {
                // DBG      webIO << " nope! " << endl;
                doExpand = false;
              }
            }
            // Recursion
            if( handlers[hi] == this )
            {
              if( doExpand )
              {
                string tempPid = ParentId;
                handlers[hi]->ParentId = id;
                handlers[hi]->level++;
                handlers[hi]->emit(Filter(handlers[hi],rs));
                level--;
                ParentId = tempPid;
              }
            }
            else
            {
              if( doExpand )
              {
                handlers[hi]->ParentId = id;
                handlers[hi]->emit(Filter(handlers[hi],rs));
              }
            }
          }
          indent(level);
          webIO << treeNodeEnd;
        }
      } while( rs.next() );
    }
        // First node is for for adding new items in edit mode
    if( addLink.length() )
    {
      indent(level);
      webIO << addLink.replace("$ID",ParentId.c_str());
      if( count ) count--;
    }
    return bRet;
  }
};


// tree control
class ocTreeControl
{
  cgiTemplates treeTemplate;
  ostream & webIO;
  nodeHandlers handlers;

  // templatized sections for tree
  ocString treeTop;
  ocString treeEnd;
  ocString treeNodeLabel;
  
  string UID; // if set, triggers event recording for state of nodes -
             //   both for branches and selections

  
public:
  string Title;

  ocTreeControl( string templatePath, ostream & webIo )
  :webIO(webIo)
  {
    treeTemplate.load(templatePath.c_str());
    treeTop = treeTemplate.getParagraph("treeTop");
    treeEnd = treeTemplate.getParagraph("treeEnd");
    treeNodeLabel = treeTemplate.getParagraph("treeLabel");
  }
  virtual ~ ocTreeControl()
  {
    for( int hi=0; hi < handlers.size(); hi++ )
    {
      delete handlers[hi];
    }
  }
  
  // persit using a unique tree id (uid is overstating this)
  void persistantStates(string uid)
  {
    UID = uid;
  }
  
  ocTreeNode * addHandler( int nodeSequence, ocTreeNode * lastNode = 0 )
  {
    ocTreeNode * newNode = new ocTreeNode(treeTemplate, webIO );
    if( newNode )
    {
      newNode->level = 0;
      newNode->treeId = UID;
      if( lastNode )
      {
        newNode->level = lastNode->level + 1;
        lastNode->handlers.push_back( newNode );
      }
      else
      {
        handlers.push_back( newNode );
      }
    }
    return newNode;
  }
  void makeRecursive( int nodeSequence, ocTreeNode * lastNode = 0 )
  {
    if( lastNode )
    {
      lastNode->handlers.push_back( lastNode );
    }
  }
  bool emit( string filter = "" )
  {
    webIO << treeTop.replace("$UID",UID);
    Session_Obj sess;
    string expanded;
    if( UID.length() )
    {
      expanded = sess.GetData(UID);
    }
    if( Title.length() )
    {
      webIO << treeNodeLabel.replace("$label",Title) << endl;
    }
    
    // Start with the 0 level handler;
    for( int hi=0; hi < handlers.size(); hi++ )
    {
      ocTreeNode * theBase = dynamic_cast<ocTreeNode *>(handlers[0]);
      if( theBase )  theBase->emit(filter);
    }
    string script;
    if( expanded.length() )
    {
      script = " Expand these nodes: \n";
      script += "  expand('";
      script += UID;
      script += "',";
      script += expanded;
      script += ");";
    }
    webIO << treeEnd.replace("$expando",script);
  }
};

#endif

#ifdef IN_T2_TESTHARNESS
  {
    cgiTemplates templ;
    
    templ.load("wTree.html");

    script << templ.getParagraph("top");

    ocTreeControl tc( "wTree.html", script );
    tc.Title = "Site Tree - Test of tree control";
    ocTreeNode * theBase = tc.addHandler( 0 );
    
    if( theBase )
    {
      theBase->fields = "id, name";
      theBase->from = "sites";
      theBase->location = "http://devlinux/sys/site_ui.cgi?id=$ID";
      theBase->label = "<span style=\"font: bold 12pt arial;text-decoration: underline;\">SITES</span>";
    }
    ocTreeNode * theNext = tc.addHandler( 1, theBase );
    if( theNext )
    {
      theNext->fields = "id, name";
      theNext->from = "pages";
      theNext->location = "http://devlinux/sys/page_ui.cgi?id=$ID";
      theNext->childFilter = "  where site_id = $ID  ";
      theNext->label = "<span style=\"font: bold 9pt/10pt arial\">PAGES</span>";
    }
    ocTreeNode * theLast = tc.addHandler( 2, theNext);
    if( theLast )
    {
      theLast->fields = "id, name";
      theLast->from = "paragraphs";
      theLast->location = "http://devlinux/sys/paragraph_ui.cgi?id=$ID";
      theLast->childFilter = " where page_id = $ID ";
      theLast->label = "<span style=\"font: bold 8pt/10p arial\">PARAGRAPHS</span>";
    }
    theLast = tc.addHandler( 3, theNext);
    if( theLast )
    {
      theLast->fields = "pm.id, m.name";
      theLast->from = "page_menus pm inner join menus m on pm.menu_id = m.id";
      theLast->location = "http://devlinux/sys/page_menu_ui.cgi?id=$ID";
      theLast->childFilter = " where page_id = $ID ";
      theLast->label = "<span style=\"font: bold 8pt/10p arial\">MENUS</span>";
    }
    tc.emit();
    script << templ.getParagraph("end");
  }
#endif


