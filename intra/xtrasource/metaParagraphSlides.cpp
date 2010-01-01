/**
  metaParagraphSlides.cpp
  User Interface Definition and Implementation  for meta_paragraphs.
  Copyright $copy; 2009 by D.K. McCombs. W3Sys.com
*/

#include <iostream>
#include <iomanip>
#include "cgiTemplates.h"
#include "connectstring"
#include "ocTypes.h"
#include "ocString.h"
#include "read_write_base.hpp"
#include "paragraphs.hpp"
#include "cgiClass.h"
#include "cgiTemplates.h"
using namespace std;

class metaParagraphs:  public paragraphs_Obj
{
protected:
  cgiScript & script;
  cgiTemplates & theTemplate;
  cgiInput & args;
  
public:
  string currentSlideId;
  string initialSlideParagraph;
  
  metaParagraphs(cgiScript & script, cgiTemplates & theTemplate)
  :paragraphs_Obj(), script(script),theTemplate(theTemplate),args(script.ClientArguments())
  {  
  }

  virtual ~metaParagraphs(){;}

  bool load_detail()
  {
    bool got=false;
    initialSlideParagraph = args["p"].c_str();
    id = atoll(initialSlideParagraph.c_str());
    key(id);       
    got = get_data();
    return got;
    
  }

  string detail_display()
  {
      ocString out = theTemplate.getParagraph("detail");
      ocString strPage;
      strPage.append(page_id);
      ocString strParagraph;
      strParagraph.append(id);
      out = out.replaceAll( "$page$"  ,strPage)
                    .replaceAll( "$id$"    ,strParagraph)
                    .replaceAll( "$content$" ,content)
                    .replaceAll( "$label$" , name );
      return out;
  }

  string getWhere( string page, string section )
  {
    string where = "page_id="+page+" and section='"+section+"'";
    if( args.count("paraSrchCrtria") )
    {
      string search = args["paraSrchCrtria"].c_str();
      where = "( ( content like '%" + search + "%' ";
      where += " or name like '%" + search +  "%' ) ";
      where += " and section = 'slides') ";
    }//  DBG   script << where << endl;
    return where;
  }

  void full_slide_display(string page, string section)
  {
    bool iStillGotIt = get_data(getWhere(page, section), "place_order");
    int order = 0;
    while( iStillGotIt)
    {
      ocString out = theTemplate.getParagraph("detail");
      ocString strPage;
      strPage.append(page_id);
      ocString strParagraph;
      strParagraph.append(id);
      ocString strPlaceOrder;
      strPlaceOrder.append(order);
      script <<  out.replaceAll( "$page$"  ,strPage)
                    .replaceAll( "$id$"    ,strParagraph)
                    .replaceAll( "$slide$" ,strPlaceOrder )
                    .replaceAll( "$content$" ,content)
                    .replaceAll( "$label$" , name );
      order++;
      iStillGotIt = next();
    }
  }


  // Implement pure virtual of form display
  void slide_display( string page, string section )
  {
    bool iStillGotIt = get_data(getWhere(page, section), "place_order");
    int order = 0;
    while( iStillGotIt)
    {
      ocString out = theTemplate.getParagraph("slide");
      ocString strPage;
      strPage.append(page_id);
      ocString strParagraph;
      strParagraph.append(id);
      ocString strPlaceOrder;
      strPlaceOrder.append(order);
      script <<  out.replaceAll( "$page$"  ,strPage)
                    .replaceAll( "$id$"    ,strParagraph)
                    .replaceAll( "$slide$" ,strPlaceOrder )
                    .replaceAll( "$image$" ,slideImage(content))
                    .replaceAll( "$label$" , ocString(name).replaceAll("\"","'" ) );
      if( initialSlideParagraph == strParagraph )
      {
        currentSlideId = strPlaceOrder;
      }
      order++;
      iStillGotIt = next();
    }
  }

  ocString slideImage( ocString parseContent )
  {
    ocString image = theTemplate.getParagraph("no-image");

    // find an image tag
    if( parseContent.find("<img") != string::npos ) parseContent.parse("<img");
    else parseContent.parse("<IMG");
    if( !parseContent.endOfParse() ) // found one!
    {  // w3sys images are always in images or small_images directories
       parseContent.parse("images/");
       ocString worker =  parseContent.parse(">");
       worker = worker.replaceAll("'","\"");
       image = worker.parse("\"");
    }
    return image;
  }
};


int main( int argcount, char ** args )
{
  cgiScript script( "text/html" );
  
  ocString page, section;
  cgiTemplates pgTemplate;
  pgTemplate.load("Templates/slidePane.htmp");
  
  metaParagraphs detail(script, pgTemplate);
  if( detail.load_detail() )
  {
    ocString page, section;
    page.append(detail.page_id);
    section.append(detail.section);
    
    script << pgTemplate.getParagraph("top");
    ocString detailDisplay = detail.detail_display();   // the selected item, or the first item
    
    metaParagraphs content(script, pgTemplate);
    content.initialSlideParagraph = detail.initialSlideParagraph;
    content.slide_display(page, section);
    
    script << pgTemplate.getParagraph("mid");
    script << detailDisplay.replaceAll( "$slide$" ,content.currentSlideId );
    script << ocString(pgTemplate.getParagraph("lomid")).replace("$id$",content.currentSlideId);
    content.full_slide_display(page, section);
    
    script << pgTemplate.getParagraph("end");
  }
}

// compile implementations here
#include "read_write_base.cpp"


