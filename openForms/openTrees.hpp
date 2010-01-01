/*

  openTrees.hpp
  ------------------
  Extensible framework for building trees.

  Need some javascript on the client to expand and collapse.
  

*/

#ifndef oPenTreeS_hPp
#define oPenTreeS_hPp

#include <iostream>
#include <iomanip>
#include <uuid/uuid.h>

#include "ocTypes.h"
#include "ocString.h"
#include "read_base.hpp"
#include "cgiTemplates.h"

// base type tree node
class baseTreeNode: public read_base
{
  protected:
  ostream & webIO;
  cgiTemplates & treeTemplate;

  // Temparary data to fill out the node;
  ocString treeStem;
  ocString treeSpacer;
  ocString nodeBranch;
  ocString nodeExpand;
  ocString nodeLeaf;
  ocString nodeLastBranch;
  ocString nodeElement;
  ocString nodeLeafEnd;
  ocString vizDiv;
  ocString vizDivEnd;
  int count;
  bool isLastNode;

  public:
    baseTreeNode( cgiTemplates & templateIn, ostream & webIo )
  : webIO( webIo ),treeTemplate( templateIn ),count(0),level(0),isLastNode(false)
    {
      treeStem = treeTemplate.getParagraph("treeStem");
      treeSpacer = treeTemplate.getParagraph("treeSpacer");

      nodeBranch = treeTemplate.getParagraph("nodeBranch");
      nodeExpand = treeTemplate.getParagraph("nodeExpand");
      nodeLeaf = treeTemplate.getParagraph("nodeLeaf");
      nodeLastBranch = treeTemplate.getParagraph("nodeLastBranch");
      nodeElement = treeTemplate.getParagraph("nodeElement");
      nodeLeafEnd = treeTemplate.getParagraph("nodeLeafEnd");
      vizDiv = treeTemplate.getParagraph("vizDiv");
      vizDivEnd = treeTemplate.getParagraph("vizDivEnd");
    }
    virtual ~baseTreeNode() {;}
    virtual bool emit( string filter = "" ){;}
    bool Count( void )
    {
      return count;
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
// handlers for each level
typedef map<int,baseTreeNode*> nodeHandlers;

class ocTreeNode: public baseTreeNode
{
  nodeHandlers & handlers;
public:
  ocTreeNode( cgiTemplates & templateIn, ostream & webIo, nodeHandlers & inHandlers )
  :baseTreeNode( templateIn, webIo ),handlers(inHandlers)
  {
    
  }
  virtual ~ocTreeNode(){;}
  virtual void emitStem( int stemLevel )
  {
    ocTreeNode * thatNode = 0;
    if( handlers.find(stemLevel) != handlers.end() )
    {
      thatNode = dynamic_cast<ocTreeNode*>(handlers[stemLevel]);
      if( thatNode->isLastNode )
      {
        webIO << treeSpacer; 
      }
      else
      {
        webIO << treeStem;
      }
    }
  }
  // gives each leaf a unique id
  virtual string NodeLeaf( int level, string id )
  {
    ocString ID;
    ID.append(level);
    ID += "_";
    ID += id;
    return nodeLeaf.replace("$ID",ID.c_str());
  }
  virtual bool emit( string filter = "" )
  {
    bool bRet = false;
    isLastNode = false;
    int cursor = 0;
    string qry = "select " + fields;
    qry += " from " + from;
    qry += filter;
    
    if( label.length() )
    {
      webIO << NodeLeaf(level,"LBL");
      for( int iLev = 0; iLev < level; iLev++ )
      {
        emitStem(iLev);
      }
      webIO << label << nodeLeafEnd << endl;
    }
    if( addLink.length() )
    {
      webIO << NodeLeaf(level,"0");
      for( int iLev = 0; iLev < level; iLev++ )
      {
        emitStem(iLev);
      }
      if( count == 1 )
      {
        webIO << nodeLastBranch;
      }
      else
      {
        webIO << nodeBranch;
      }
      webIO << addLink.replace("$ID",ParentId.c_str());
      webIO << nodeLeafEnd << endl;
      if( count ) count--;
    }
    if( rs.open( qry ) )
    {
      bRet = true;

      do {
        cursor++;
        isLastNode = (cursor == count);
        string tempLoc = location.replace("$ID", rs.getField(0).format().c_str());
        webIO << NodeLeaf(level,rs.getField(0).format());
        for( int iLev = 0; iLev < level; iLev++ )
        {
          emitStem(iLev);
        }
        if( isLastNode )
        {
          webIO << nodeLastBranch;
        }
        else
        {
          webIO << nodeBranch;
        }
        // are there child items?
        int childCount = 0;
        ocTreeNode * theNext = 0;
        string filter = childFilter.replace("$ID", rs.getField(0).format().c_str() );
        if( handlers.find(level+1) != handlers.end() )
        {
          theNext = dynamic_cast<ocTreeNode*>(handlers[level+1]);
          childCount = theNext->Count(filter);
          if( childCount > 0 )
          {
            // put the plus icon
            webIO <<  nodeExpand;
          }
        }
        // convenience for multiple fields in recordset
        string label = "";
        for( int lx=1; lx<rs.getFieldCount(); lx++ )
        {
          if( label.length() && rs.getField(lx).format().length() ) label += " - ";
          label += rs.getField(lx).format();
        }
        webIO << nodeElement.replace("$location", tempLoc)
                            .replace("$label", label );
        if(  theNext && childCount > 0 )
        {
          webIO << vizDiv;
          theNext->ParentId = rs.getField(0).format(); // for 'add'
          theNext->emit(filter);
          webIO << vizDivEnd;
        }
        webIO << nodeLeafEnd << endl;

      } while( rs.next() );
    }
    return bRet;
  }
  bool Count( string filter = ""  )
  {
    count = 0;
    string qry = "select count(*) from " + from;
    qry += filter;
    if( rs.open( qry ) )
    {
      count = atoi( rs.getField(0).format().c_str() );
    }
    if( addLink.length() ) count++;
    // webIO << "COUNT" << count << endl;
    return count;
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
  string UUID; // if set, triggers event recording for state of nodes branshes and selectsion

  
public:

  ocTreeControl( string templatePath, ostream & webIo )
  :webIO(webIo)
  {
    treeTemplate.load(templatePath.c_str());
    treeTop = treeTemplate.getParagraph("treetop");
    treeEnd = treeTemplate.getParagraph("treeEnd");

  }
  virtual ~ ocTreeControl()
  {
    nodeHandlers::iterator it = handlers.begin();
    while( it != handlers.end() )
    {
      if( it->second ) delete it->second;
      ++it;
    }
    handlers.clear();
  }
  void persitantStates(void)
  {
    uuid_t uuid;
    char uuid_val[37];
    memset(uuid,'\0',sizeof(uuid));
    uuid_generate(uuid);
    uuid_unparse(uuid, uuid_val);
    ocString uuid_work(uuid_val);
    UUID = uuid_work.replaceAll("-","");
  }
  ocTreeNode * addHandler( int nodeLevel )
  {
    ocTreeNode * newNode = new ocTreeNode(treeTemplate, webIO, handlers);
    if( newNode )
    {
      newNode->level = nodeLevel;
      handlers.insert( make_pair( nodeLevel, newNode ) );
    }
    return newNode;
  }
  bool emit( string filter = "" )
  {
    webIO << treeTop;
    
    // Start with the 0 level handler;
    if( handlers.find(0) != handlers.end() )
    {
      ocTreeNode * theBase = dynamic_cast<ocTreeNode*>(handlers[0]);
      theBase->Count();
      theBase->emit(filter);
    }
    webIO << treeEnd;
  }
};

#endif

#ifdef IN_T2_TESTHARNESS
  {
    cgiHead head;
    {
      cgiCan style("style", "type=\"text/css\"");
      style <<
        "div.viz, div.branch, div.leaf { font: normal 8pt/10pt san-serif; clear: left; }" << endl;
      style <<  "div.viz { margin: 0px; display:none; } "
        "img{ float: left; } "
        "a { display: block; float: left; margin-left: 4px; "
        "color: #059; text-decoration: none; }"<< endl;
      
    }
    {
      cgiCan script("script"," type=\"text/javascript\" src=\"openTree.js\" ");
    }
  }
  {
    cgiBody body;
    body << "<h1>tree testing</h1>" << endl;

    ocTreeControl tc( "tree.htmp", script );
    ocTreeNode * theBase = tc.addHandler( 0 );
    if( theBase )
    {
      theBase->fields = "id, name";
      theBase->from = "sites";
      theBase->location = "http://devlinux/sys/site_ui.cgi?id==$ID";
      theBase->childFilter = " where site_id = $ID ";
      theBase->label = "<span style=\"font: bold 12pt arial;text-decoration: underline;\">SITES</span>";
    }
    ocTreeNode * theNext = tc.addHandler( 1 );
    if( theNext )
    {
      theNext->fields = "id, name";
      theNext->from = "pages";
      theNext->location = "http://devlinux/sys/page_ui.cgi?id=$ID";
      theNext->childFilter = " where page_id = $ID ";
      theNext->label = "<span style=\"font: bold 9pt/10pt arial\">PAGES</span>";
    }
    ocTreeNode * theLast = tc.addHandler( 2 );
    if( theNext )
    {
      theLast->fields = "id, name";
      theLast->from = "paragraphs";
      theLast->location = "http://devlinux/sys/paragraph_ui.cgi?id==$ID";
      theLast->label = "<span style=\"font: bold 8pt/10p arial\">PARAGRAPHS</span>";
    }
    tc.emit();
  }
#endif


