/*
  Copyright (c) 2003
  metatagPick.cpp 
  Author David McCombs -- www.w3sys.com   
*/
#include "connectstring"
#include "openLogin.h"
#include "cgiTemplates.h"
#include "openLogin.h"
#include "ocFileSys.h"
// #define DO_OPEN_LOGGING
#include "openLogger.h"
#include "siteLimit.h"
#include "ocXML.h" // implement context sensitive tag listings

using namespace std;
openLogin oLogin;
const string csPath="Templates";
void  templateList(cgiScript & script, openLogin & login)
{
  string context;
  if( script.ClientArguments().count("context") ) context = script.ClientArguments()["context"].c_str();
  cgiTemplates htmlDoc;
  htmlDoc.load("Templates/listMetatags.htmp");
  char * Rows[2] = { "oddcell", "evencell" };
  script << ocString(htmlDoc.getParagraph("top"))
            .replaceAll( "$TITLE_GOES_HERE",
                      "File Choices:" )
         << endl; 
         
  ocString link = "<a href='metatagPick.cgi?template_path=$path&site_id=$id&context=$context'>"
                  "$path</a>";
                  
  ocString headitem = htmlDoc.getParagraph("headitem");
  string rowsep = htmlDoc.getParagraph("rowsep");
  script << headitem.replace("__hcell_label","Pick a file first:");
                                     
  quickQuery qqry;
  openRS & rs = qqry.getRS();
  
  string siteLimitation = siteFocus(login);
  string sql = "select path, site_id from metasite.templates";
  if( siteLimitation.length() )
  {
    sql += " where site_id ";
    sql += siteLimitation;
  }
  int iRow = 0;
  for( bool b = rs.open(sql);
       b;
       b= rs.next(), ++iRow )
  {
    ocString row = htmlDoc.getParagraph(Rows[ iRow % 2 ] );
    script << rowsep;
    script << row.replace( "$cell_data",
                           link.replaceAll( "$path",
                                            rs.getField(0).format().c_str()
                                           )
                               .replace( "$id",
                                         rs.getField(1).format().c_str() )  
                               .replace( "$context", context.c_str()   ).c_str() );
    script << endl;   
  }           
}
/* 
  New contextual 
  filtering of metatag lists 
*/
string contextualFilter( ocString context, string controlXML )
{  
  string ret;
  writelog3("Entered contextualFilter( )", context, controlXML );      
  if( context.length() && controlXML.length() )
  {
    xmlParser ctrlParse(controlXML);
    ctrlParse.parse();  
    node_vector & node_vect = ctrlParse.nodeList();
    // Need to build a regexp filter for items like '(w1|w2|w3|..)'
    // context will be made of node attribure pairs ex:
    // menu items, content items, menu open, menu close
    string tk = context.parse(" ");

    string attr = context.parse(" ");
    writelog2("Node Attribute: ", attr );
    
    node_map::iterator node_pos = ctrlParse.states.nodemap.lower_bound(tk);
    if( node_pos != ctrlParse.states.nodemap.upper_bound(tk) ) ret = "(";
    for( ; node_pos != ctrlParse.states.nodemap.upper_bound(tk); ++ node_pos )    
    {  
      if( ret.length() > 1 )  ret += "|";   
      xmlNode & node = node_vect[node_pos->second];      
      if( attr.length() ) 
      {        
        ocString filt = node.attr[attr];
        if( filt.length() )
        {         
          ret += filt.replaceAll(",","|");
        }
      }
    }
    if( ret[0] == '(' ) ret += ")";

  }
  writelog2("Exited contextualFilter( ) returning: ", ret );  
  return ret;
}   
 
void  tagList(cgiScript & script, string path, string site_id )
{
  cgiTemplates htmlDoc, chosenTemplate;
  htmlDoc.load("Templates/listMetatags.htmp");
  script << ocString(htmlDoc.getParagraph("top"))
              .replaceAll( "$TITLE_GOES_HERE", 
                           "Tag Choices:" ) << endl;
                           
  string sql = "select path from metasite.sites where id = ";
  sql += site_id;
  quickQuery qqry;
  openRS & rs = qqry.getRS();
  
  if( rs.open(sql) )
  {
    string iPath = rs.getField(0).format().c_str();
    iPath += "/";
    iPath += csPath;
    iPath += "/";
    iPath += path; 
    
    chosenTemplate.load(iPath.c_str());

    char * Rows[2] = { "oddcell", "evencell" };   

    ocString headitem = htmlDoc.getParagraph("headitem");
    string rowsep = htmlDoc.getParagraph("rowsep");

    writelog("Writing headers");
    script << headitem.replace("__hcell_label","Pick a tag:");

    ocString link = "<a href='javascript:pickTag(\"$tag\")'>"
                    "$tag</a>";  
    paragraphMap::iterator pos;
    int iRow = 0;
    paragraphMap & pmap = const_cast<paragraphMap &>(chosenTemplate.getParagraphs());
    // filter here
    string context;
    if( script.ClientArguments().count("context") ) context = script.ClientArguments()["context"].c_str();
    writelog2("Got context", context)
    string filt = contextualFilter( context, chosenTemplate.getParagraph("control") );
    for(pos = pmap.begin(); pos != pmap.end(); ++pos )
    {
      ocString test_it =  pos->first;       
        
      // these are all reserved words
      if( ( !filt.length() || test_it.regExMatch(filt.c_str()) ) && 
          pos->first != "edits" &&
          pos->first != "top" &&
          pos->first != "end" &&
          pos->first != "help" &&
          pos->first != "control" )
      {writelog3("Tags: ", pos->first, pos->second);
                
        ocString row = htmlDoc.getParagraph(Rows[ iRow % 2 ] );
        ++iRow;
        script << rowsep << 
          row.replace( "$cell_data",
                       link.replaceAll( "$tag",
                                         pos->first.c_str()
                                       ).c_str() ) << endl;        
      }
    }
  }
  else
  {
    script << sql << endl;
  }
  script << htmlDoc.getParagraph("bottom") << endl;
}

int main( int argcount, char ** args )
{
  // An instance of the script
  // must be created before any other output.
  // the last param automatically save images to the path  

  cgiScript script("text/html", false);
  writelog("OK - script made");
  // attempt to get cookie
  if ( oLogin.testLoginStatus() )
  {
    script.closeHeader();
    if( script.ClientArguments().count("template_path") )
    {
      tagList( script,
               script.ClientArguments()["template_path"].c_str(),
               currentSite( script.ClientArguments() ) ); 
    }
    else
    {
      templateList(script,oLogin);    
    }
    writelog("Done");
  }
  else
  {
    script.Redirect("signIn.html");
    script.closeHeader();
    writelog("INVALID ACCESS ATTEMPT");      
  }
  writelog("OK - All Done");
  return(0);
}
#include "read_write_base.cpp"

