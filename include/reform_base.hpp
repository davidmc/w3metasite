/**
  reform_base.hpp

  ==============  
  Reporting Base Class  
  davidmc@w3sys.com 
  copyright (C) 2006

 it came up more that once that I was usgin the report generator to fill out a form.
 It really was not intended for this, but could do it, but it took too much time.
 This is the basis for an alternative.

 This will take an html template as form input.
 It will still use report data for instruction
 The template semantics will have page replacement tags called markup.

Only one report form tag called 'markup'.
Two types of markup 'field' and 'repeatarea'

Very simple syntax:

form fields:=
<markup type='field' colname='string'>Example Text for viewing template</markup>

repeating area :=
<markup type='repeatarea' subrep='report_id'>
Example text to be replaced by sub report data
</markup>

:end of syntax description! that's it!

*/

#ifndef reform_base_HPP
#define reform_base_HPP

#include <iostream>
#include <iomanip>
#include "ocTypes.h"
#include "ocString.h"
#include "read_base.hpp"
#include "cgiClass.h"
#include <fstream>
#include "Report.hpp"
#include "Report_Params_ui.h"
#include "col_handling_rep.hpp"

using namespace std;

class reform_base;
void doSubReport ( reform_base & parent, llong repId );

/*
  generic report form class

*/
class reform_base: public read_base
{
public:
  ocString formTemplate;
  string cachedSQL;
  ostream & webIO;
  bool opened;
  bool more_data;
  long row;
  /*
     This constructor must be called!
  */
  reform_base(ostream&sc):read_base(),webIO(sc),
  opened(false),more_data(false),row(0)
  {
  }
   
  virtual ~reform_base(){;}
  
  // load template for pieces
  bool loadTemplate( string & path )
  {
    bool bRet = false;
    formTemplate = "";
    ifstream templateFile( path.c_str() );
    if( templateFile )
    {
      bRet = true;
      char buffer[1024];
      templateFile.getline( buffer, sizeof(buffer) );
      while( templateFile.rdstate() == ios::goodbit )
      {
        if( strlen(buffer) )
        {
          formTemplate += buffer;
          formTemplate += "\n";
        }
        templateFile.getline( buffer, sizeof(buffer) );
      }
    }
    return bRet;
  }
  
  bool getData( string sql )
  {
    cachedSQL = sql;
    iGetData(sql);
    return opened;
  }

  bool iGetData( string sql )
  {
    opened = rs.open(sql);
    return opened;
  }

  // though typically only one pass, does not preclude multiples
  reform_base & emitData( void )
  {
    more_data = opened;
    int fieldCount;
    if( opened ) fieldCount = rs.getFieldCount();
    derived_commence_event();
    while( more_data )
    {
      derived_prerow_event();

      // parse through the template, performing markup tag replecements
      parseAndReplace();
      derived_postrow_event();
      more_data = rs.next();
      if( more_data ) row++;
    }
    derived_complete_event();
    return * this;
  }
  void parseAndReplace( void )
  {
    string::size_type start = 0;
    string::size_type found = 0;
    string attrDelim =  "'\"";
    do
    {
      found = formTemplate.find("<markup",start);
      if( found != string::npos )
      {
        webIO << formTemplate.substr(start,found-start) << endl;
        // what is it
        string::size_type typePos =  formTemplate.find("type",found );
        if(  typePos != string::npos )
        {               // 123456
          typePos += 6; // type='
          string::size_type typeEnd = formTemplate.find_first_of(attrDelim,typePos);
          if( typeEnd != string::npos )
          {
            //make the type string
            string theType = formTemplate.substr( typePos,  typeEnd-typePos);
            //webIO << "<h2>type: " << theType << "</h2>" << endl;
            if( theType == "field" )
            {
              // find the colname marker
              string::size_type colNamePos =  formTemplate.find("colname",found);
              if( colNamePos != string::npos )
              {                  //  123456789
                colNamePos += 9; //  colname='
                found = colNamePos;
                string::size_type colNameEnd = formTemplate.find_first_of(attrDelim,colNamePos);
                if( colNameEnd != string::npos )
                {
                  found = colNameEnd;
                  string colName = formTemplate.substr( colNamePos,  colNameEnd-colNamePos);
                  //webIO << "<h2>colname: " << colName << "</h2>" << endl;
                  sendField( colName );
                }
              }
            }
            else if( theType == "repeatarea" )
            {
              // get the report id
              llong repId = 0;
              
              // get all between markup tags
              // find the colname marker
              string::size_type ridPos =  formTemplate.find("subrep",found);
              if( ridPos != string::npos )
              {              //  12345678
                ridPos += 8; //  subrep='
                found = ridPos;
                string::size_type ridEnd = formTemplate.find_first_of(attrDelim,ridPos);
                if( ridEnd != string::npos )
                {
                  found = ridEnd;
                  string strId = formTemplate.substr( ridPos,  ridEnd-ridPos);
                  if(strId.length() )
                  {
                    repId = atoll(strId.c_str()); 
                    // invoke sub-report
                    doSubReport( repId );
                  }
                }
              }
            }
          }
          else
          {
            break;
          }
          //webIO << "<h2>found: " << found << "</h2>" << endl;
        }
        if( found != string::npos )
        {
          // go to the end;
          start = formTemplate.find("</markup>", found);
          if( start !=  string::npos )
          {
                        // 123456789
            start += 9; // </markup>
            //webIO << "<h2>start: " << start << "</h2>" << endl;
          } 
          else
          {
            webIO << "<h1>Bad Stuff: couldn't find end markup marker</h1>" << endl;
            break; 
          }
        }
      }
    } while( found != string::npos );
    // emit the remainder
    webIO << formTemplate.substr(start);
  }
  
  // To be able to override each field
  virtual void sendField( string & fldName )
  {
    webIO << rs.getField(fldName.c_str()).format();
  }

  virtual void doSubReport( llong Id );

  // events
  virtual void derived_commence_event( void )
  {
    ;
  }
  virtual void derived_prerow_event( void )
  {
    ;
  }
  virtual void derived_postrow_event( void )
  {
    ;
  }
  virtual void derived_complete_event( void )
  {
    ;
  }
  void columnTagFixup( ocString & tgt )
  {
    for( int i =0; i < rs.getFieldCount(); i++ )
    {
      ocString repTag = "{{";
      repTag.append(i);
      repTag += "}}";
      tgt = tgt.replaceAll(repTag, rs.getField(i).format());
    }
  }
};


void reform_base::doSubReport ( llong repId )
{
  Report_Obj report;
  report.key( repId );
  report.Id = repId;
  if( report.get_data() )
  {
    string path = "reportTemplates/" + report.getTemplatePath();
    ocString sql = report.getCompositeQuery();
    columnTagFixup(sql);

    col_handling_rep rpt(webIO);
    rpt.Formatting_Modulus = report.Formatting_Modulus;
    rpt. Items_Per_Page = report.Items_Per_Page;
    rpt.loadTemplates(path);
        // now lets fix the top header, with elements from the current data
    ocString top = rpt.part("top");
    columnTagFixup( top );
    rpt.reportParts.getParagraph("top") = top;
    // also fixup page break headers
    ocString pgbrk = rpt.part("pagebreak");
    if( pgbrk.length() )
    {
      columnTagFixup(pgbrk);
      rpt.reportParts.getParagraph("pagebreak") = pgbrk;
    }
    rpt.addColumnHandlers( report.Id );
        
    if( rpt.getData(sql) )
    {
      rpt.emitTop(report.Name);
      rpt.emitHeader();
      rpt.emitData();
      rpt.emitEnd();
    }
    else
    {
      webIO << "<!-- " <<  sql << " -->" << endl;
    }
  }
}



#endif
