/* METASITE EXPORT */
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "ocString.h"
#include "ocFileSys.h"
#include "connectstring"
#include "openDB.hpp"

#define DO_OPEN_LOGGING
#include "openLogger.h"
#include "ocImportParser.h" 
#include "read_write_base.hpp"
#include "SysImportFixup.h"
#include "../../intra/xtrasource/global_content_obj.hpp"
#include "../../intra/xtrasource/templates.hpp"
#define SUPPRESS_LINK_CREATION
#include "../../intra/xtrasource/page_obj.hpp"
#include "link.hpp"
#include "../../intra/xtrasource/menu.hpp"
#include "../../intra/xtrasource/menu_item.hpp"
#include "page_menus.hpp"
#include "../../intra/xtrasource/paragraphs.hpp"
using namespace std;

string condition( ocString input )
{
  return input.replaceAll("\"","\"\"");
}

void exportGlobalContent( long long site_id )
{
  global_content_Obj bobj;
  ocString whereClause = " site_id = ";
  whereClause.append(site_id);
  cout << " global_content get with " <<  whereClause << endl;
  if( bobj.get_data(whereClause))
  {
    cout << "opening Global.csv"  << endl;
    // Open a file stream
    ofstream outFile( "Global.csv" );
    do
    { cout << ".";
      outFile << bobj.id << ",\""
              << condition(bobj.name) << "\",\""
              << condition(bobj.content) << "\"" << endl;  
    }while(bobj.next()) ;
    outFile.close();
    cout << "done" << endl;
  }
  else
  {
    cout << bobj.last_result();
  }
}
void exportTemplates( long long site_id )
{
  Template bobj;
  ocString whereClause = " site_id = ";
  whereClause.append(site_id);
  cout << " templates get with " <<  whereClause << endl;
  if( bobj.get_data(whereClause))
  {
    cout << "opening Templates.csv"  << endl;
    // Open a file stream
    ofstream outFile( "Templates.csv" );
    do
    { cout << ".";
      outFile << bobj.id << ",\""
              << condition(bobj.name) << "\",\""
              << condition(bobj.path) << "\"" << endl;
    }while(bobj.next()) ;
    outFile.close();
    cout << "done" << endl;
  }
  else
  {
    cout << bobj.last_result();
  }  
}
void exportPages( long long site_id )
{
  page bobj;
  ocString whereClause = " site_id = ";
  whereClause.append(site_id);
  cout << " page get with " <<  whereClause << endl;
  if( bobj.get_data(whereClause))
  {
    cout << "opening Page.csv"  << endl;
    // Open a file stream
    ofstream outFile( "Page.csv" );
    do
    { cout << ".";
      /*
        identifier id;
        long long site_id;
        long long template_id;
        string name;
        string url;
        string meta_description;
      */
      outFile << bobj.id << ","
              << bobj.template_id << ",\""
              << condition(bobj.name) << "\",\""
              << condition(bobj.url) << "\",\"" 
              << condition(bobj.meta_description) << "\"" << endl;
    }while(bobj.next()) ;
    outFile.close();
    cout << "done" << endl;
  }
  else
  {
    cout << bobj.last_result();
  }
}

void exportLinks( long long site_id )
{
  links_Obj bobj;
  ocString whereClause = " site_id = ";
  whereClause.append(site_id);
  cout << " link get with " <<  whereClause << endl;
  if( bobj.get_data(whereClause))
  {
    cout << "opening Link.csv"  << endl;
    // Open a file stream
    ofstream outFile( "Link.csv" );
    do
    { cout << ".";
      outFile << bobj.id <<  ",\""
              << condition(bobj.name) << "\",\""
              << condition(bobj.url) << "\",\"" 
              << condition(bobj.target) << "\"" << endl;
    }while(bobj.next()) ;
    outFile.close();
    cout << "done" << endl;
  }
  else
  {
    cout << bobj.last_result();
  }
}
void exportMenus( long long site_id )
{
  menu bobj;
  ocString whereClause = " site_id = ";
  whereClause.append(site_id);
  cout << " menu get with " <<  whereClause << endl;
  if( bobj.get_data(whereClause))
  {
    cout << "opening Menu.csv"  << endl;
    // Open a file stream
    ofstream outFile( "Menu.csv" );
    do
    { cout << ".";
      outFile << bobj.id << ",\""
              << condition(bobj.name) <<  "\",\""
              << condition(bobj.reference_template) <<  "\",\""
              << condition(bobj.template_open_tag) <<  "\",\""
              << condition(bobj.template_close_tag) <<  "\""
              << endl;
    }while(bobj.next()) ;
    outFile.close();
    cout << "done" << endl;
  }
  else
  {
    cout << bobj.last_result();
  }
}
void exportMenuItems( long long site_id )
{
  menu_item bobj;
  ocString whereClause = " site_id = ";
  whereClause.append(site_id);
  cout << " menu_item get with " <<  whereClause << endl;
  if( bobj.get_data(whereClause))
  {
    cout << "opening MenuItem.csv"  << endl;
    // Open a file stream
    ofstream outFile( "MenuItem.csv" );
    do
    { cout << ".";
      outFile << bobj.id << ","
              << bobj.menu_id << ","
              << bobj.link_id << ",\""
              << condition(bobj.template_tag) << "\",\""
              << condition(bobj.locate_tag) << "\",\"" 
              << condition(bobj.target_tag) << "\",\"" 
              << condition(bobj.label_tag) << "\","                            
              << bobj.place_order << ","
              << bobj.place_level << endl;
    }while(bobj.next()) ;
    outFile.close();
    cout << "done" << endl;
  }
  else
  {
    cout << bobj.last_result();
  }
}
void exportPageMenus( long long site_id )
{
  page_menus_Obj bobj;
  ocString whereClause = " site_id = ";
  whereClause.append(site_id);
  cout << " page_menu get with " <<  whereClause << endl;
  if( bobj.get_data(whereClause))
  {
    cout << "opening PageMenu.csv"  << endl;
    // Open a file stream
    ofstream outFile( "PageMenu.csv" );
    do
    { cout << ".";
      outFile << bobj.id << ","
              << bobj.menu_id << ","
              << bobj.page_id << ","                  
              << bobj.place_order << ","
              << bobj.collapsible << endl;
    }while(bobj.next()) ;
    outFile.close();
    cout << "done" << endl;
  }
  else
  {
    cout << bobj.last_result();
  }
}

void exportParagraphs( long long site_id )
{
  paragraphs_Obj bobj;
  ocString whereClause = " site_id = ";
  whereClause.append(site_id);
  cout << " paragraph get with " <<  whereClause << endl;
  if( bobj.get_data(whereClause))
  {
    cout << "opening Paragraph.csv"  << endl;
    // Open a file stream
    ofstream outFile( "Paragraph.csv" );
    do
    { cout << ".";

      outFile << bobj.id << ","
              << bobj.page_id << ","
              << bobj.place_order << ",\""
              << condition(bobj.template_tag) << "\",\""
              << condition(bobj.replace_tag) << "\",\"" 
              << condition(bobj.name) << "\",\""              
              << condition(bobj.content)  << "\",\""
              << condition(bobj.section)  << "\","
              << bobj.edit_type  <<  endl;
    }while(bobj.next()) ;
    outFile.close();
    cout << "done" << endl;
  }
  else
  {
    cout << bobj.last_result();
  }
}

int main( int argc, char** argv )
{
  /*
    Must have one argument, the site id for the site
    set up in THIS system.
    It should check to see if such a site exists
  */
  if( argc > 1 )
  {  
    // Check site key
    long long site_id = atoll(argv[1]);
    // export globals
    exportGlobalContent( site_id );
    // export templates    
    exportTemplates( site_id );
    // export pages
    exportPages( site_id );
    // export links
    exportLinks( site_id );
    // export menus
    exportMenus( site_id );
    // export menu_items
    exportMenuItems( site_id );
    // export page_menus
    exportPageMenus( site_id );
    // export paragraphs
    exportParagraphs( site_id );
  }



  return 0;
}

#include "read_write_base.cpp"
