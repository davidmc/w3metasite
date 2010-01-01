/*
  documents.cpp
  main file template for rad generated intranet pages.    
*/

#include <string>
//#define OPEN_LOGGING_FILE
#include "openLogger.h"
using namespace std;


#include "w3intranet.h"
#include "page_enhancements.hpp"
#include "Document.hpp"

class document_list_functor: public base_functor
{
  cgiScript & script;
  bool CanEdit;
  bool IsSignedIn;
  bool Secured;
  Document_Obj document;
public:
  document_list_functor(page & ipg,cgiScript & script, bool CanEdit, bool IsSignedIn)
  :base_functor(ipg),script(script),CanEdit(CanEdit),IsSignedIn(IsSignedIn),Secured(false)
  {;}
  virtual ~document_list_functor()
  {;}
  
  virtual  bool operator()( xmlNode & node )
  {
    bool bRet = false;

    node_attr::iterator x = node.attr.find("secured");
    if( x!=node.attr.end() &&  x->second == "true") Secured = true;
    
    paragraphMap & paras  = pg.get_template().getParagraphs();
    ocString category, entries;

    if( IsSignedIn == true || Secured == false )
    {
      x = node.attr.find("category");
      if( x!=node.attr.end() )
      {
        category = paras[x->second];
      }
      else
      {
        // get the parent folder information
        script << "can't find document_list 'category' tag " << endl;
      }
      x = node.attr.find("entries");
      if( x!=node.attr.end() )
      {
        entries  = paras[x->second];

        // Loop Filter
        string clause = "Visible > 0";

        if( document.get_data(clause, "Sequence, Category, Name" ) )
        {
          string lastCat;
          do {
            if ( lastCat != document.Category )
            {
              lastCat = document.Category;
              script << category.replace("$category$",lastCat);
            }
            ocString documentId;
            documentId.append(document.Id);
            editIcon( paras, node, documentId );
            script << entries
                .replaceAll("$id$", documentId)
                .replace("$location$",document.Filename)
                .replaceAll("$label$",document.Name)
                .replace("$description$",document.Description);
            
          } while(document.next());
        }
        else
        {
           script << document.last_result();
        }
      }
      else
      {
        script << "can't find document_list 'entries' tag " << endl;
      }
      addIcon( paras, node );
    }

    return bRet;
  }
  
  void editIcon( paragraphMap & paras, xmlNode & node, string Id )
  {
    if( CanEdit )
    {
      ocString editLink;
      node_attr::iterator x = node.attr.find("editlink");
      if( x!=node.attr.end() )
      {
        editLink = paras[x->second];
        script << editLink
            .replaceAll("$id$", Id );
      }
      else
      {
        script << "can't find document_list 'editLink' tag " << endl;
      }
    }
  }
  void addIcon( paragraphMap & paras, xmlNode & node )
  {
    if( CanEdit )
    {
      ocString addLink;
      node_attr::iterator x = node.attr.find("addlink");
      if( x!=node.attr.end() )
      {
        addLink = paras[x->second];
        script << addLink;
      }
      else
      {
        script << "can't find document_list 'addlink' tag " << endl;
      }
    }
  }

};

bool intraMain(page & pg ,cgiScript & script)
{
  return true;
}

// Standard main function
int main(int argc, char ** argv)
{  
  baseUrl = "intranet.meta";  // this is my compiled name!
  // because we are checking
  cgiScript script("text/html",false);
  isSignedOn = check(script);
  script.closeHeader();
  
  page pg(script);  
  
  writelog( "load page" );  
  if( pg.load() )
  {    
    writelog( "instance of page control" );
    page_control ctrl(pg);     
    ctrl.addOp ( "servicelist",  new servicelist_functor(pg,script,isSignedOn) );
    ctrl.addOp ( "userinterface",  new form_functor(pg,script,isSignedOn) );
    ctrl.addOp ( "qrycontent", new qrystr_content_functor(pg,script) );
    ctrl.addOp ( "documentlist", new document_list_functor(pg,script,oLogin.SiteAdmin(), isSignedOn) );
    writelog( "page control emit" );
    ctrl.emit();    
  }
  else
  {
    script << "<html><head><title>Page Unavailable</title></head>" << endl
           << "<body><h1>Sorry</h1>" << endl
           << "<p>The page you requested is not available</p></body></html>";
  } 
  return 0;
}

// compile implementations here
#include "read_write_base.cpp"
