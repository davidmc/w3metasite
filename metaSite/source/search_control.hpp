#include "page_control.hpp"
/*

  Search Control:
  =============
  control tags
  
  search_form - a template section for the search
  -----------
  attr:
      -----
      item := the template section
    
  The form must reflect back to the current page and have a search input named 'q'
  form will have replacement $q$ to re-display last search value
  
  results - a template section for listing results of the search
  ------
      attr:
      -----
      item := the template section
      no_results := section to show if no results encountered
      words := number of words for summary
  --------
  results will have replacements on
  
  $title$ - the page title
  $location$ - the page url
  $content$ - a sample of the page content as summary, posssibly around find
      
*/


class form_functor: public base_functor
{
  cgiScript & script;
public:
  
  form_functor(page & ipg,cgiScript & script):base_functor(ipg),script(script){;}  
  virtual ~form_functor(){;} 
  
  virtual  bool operator()( xmlNode & node )
  {
    bool bRet = false;    
    string item;
    
    node_attr::iterator x = node.attr.find("item"); // detail for what slide?
    if( x!=node.attr.end() ) item = x->second;
    x = node.attr.find("item"); // new default suppresses show on admin pane
    if( x!=node.attr.end() )
    {
      item=x->second;
      ocString frm = pg.paragraph_string( item );
      // get the last search for replacement of 
      script << pg.paragraph_string( item ); 
    } 
    else
    {
      script << "no content items" << endl;
    }
    
    return bRet;    
  }
};


class results_functor: public base_functor
{
  cgiScript & script;
public:
  results_functor(page & ipg,cgiScript & script):base_functor(ipg),script(script){;}
  virtual ~results_functor(){;}
  
  show_page & showpg(void) { return dynamic_cast<show_page &>(this->pg); }
  
  bool shouldShow(xmlNode & node )
  {
    bool ret=true; // assume we should
    if( isMutex(node) )
    {
      if( script.ClientArguments().count("s") )
        ret=false; // since we're mutually exclusive, and we have a detail, don't show
    }
    return ret;
  }
  
  virtual  bool operator()( xmlNode & node )
  {
    bool bRet = false;
    
    if( shouldShow( node ) )
    {
    
      string items, replace_tag;        
      node_attr::iterator x = node.attr.find("items");
      if( x!=node.attr.end() )
      {

        items=x->second;
        x = node.attr.find("replace_tag");  // preferred  
        if( x==node.attr.end() ) x = node.attr.find("replace-tag"); // backward compat
        if( x!=node.attr.end())
        {
          replace_tag=x->second;
          bRet = showpg().emitSlides( items, replace_tag );        
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
    }
    return bRet;    
  }
};
