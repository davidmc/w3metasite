/**
  reform_base.hpp

  ==============  
  Reporting Base Class  
  davidmc@w3sys.com 
  copyright (C) 2006

 it came up more that once that I was using the report generator to fill out a form.
 It really was not intended for this, but could do it, but it took too much time.
 This is the basis for an alternative.

 This will take an html template as form input.
 It will still use report data for instruction
 The template semantics will have page replacement tags called markup.

Only one report form tag called 'markup'.
Two types of markup 'field' and 'repeatarea'

Very simple syntax:

form fields:=
<markup type="field" colname="string">Example Text for viewing template</markup>
use as template:
<markup type="field" colname=""> </markup>
repeating area :=
<markup type="repeatarea" subrep="report_id">
Example text to be replaced by sub report data
</markup>

:end of syntax description! that's it!
(BUT MAKE SURE YOU USE DOUBLE QUOTES!)

Extra Info:
  This also fully supports sub reports, at which point the functionality is
   turned over to the default report generator code.

For reports that may have multiple data items, these tags are now supported:

<!--@begin--> blah blah <!--/@begin-->
<!--@middle-->   blah blah blah (with markup) <!--/@middle-->
<!--@pagebreak--> blah <!--/@pagebreak-->
<!--@end-->   blah blah    <!--/@end-->

*/

#ifndef reform_base_HPP
#define reform_base_HPP

#include <iostream>
#include <iomanip>
#include "ocTypes.h"
#include "ocString.h"
#include "read_base.hpp"
#include "cgiClass.h"
#include "cgiTemplates.h"
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
  string beginDoc;
  ocString formTemplate;
  string endDoc;
  string pageBreak;
  
  string cachedSQL;
  ostream & webIO;
  bool opened;
  bool more_data;
  long row;

  formatMap     colFormats;
  void addColumnHandlers( long long Report_Id )
  {
    ocString handlerSQL = "select Id, Col, Format from Report_Conditioning where Report_Id = ";
    handlerSQL.append( Report_Id );
    for( bool bopen = rs.open(handlerSQL); bopen; bopen=rs.next() )
    {
      addFormat();
    }
    rs.close();    
  }
  void addFormat( void )
  {
     int col = atol(rs.getField("Col").format().c_str());
     string Col_Format = rs.getField("Format").format();
     // DBG     cout << " Add "  << Col_Format << " at " <<  col << endl;
     if(Col_Format=="CURRENCY")
     { 
       colFormats.insert(make_pair(col, new formatCurrency));
     }
     else if(Col_Format=="DATE")
     { 
       colFormats.insert(make_pair(col, new formatDate));
     }   
     else if(Col_Format=="DECIMAL")
     {
       colFormats.insert(make_pair(col, new formatDecimal));
     }
     else if(Col_Format=="INTEGER")
     {
       colFormats.insert(make_pair(col, new formatInteger));
     }
  }
  int fieldPosition( string & fieldName )
  {
    int row=0;
    int rowCount = rs.getFieldCount();
    for(row=0;row<rowCount;row++)
    {
       if( rs.getField(row).getName() == fieldName ) break;
    }
    return row;
  }
  string formatField( string & fieldName )
  {
    int iField = fieldPosition(fieldName);
    string val = rs.getField(iField).format();    
    formatMap::iterator pos = colFormats.find(iField); 
    if( pos != colFormats.end() )
    {  
      val = (*(pos->second))(val);
    }    
    return val;    
  }
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
    cgiTemplates templateFile;
    if( templateFile.load( path.c_str() ) )

    {
      bRet = true;
      // figure out what mode, begining and ending and middle with page break or all in one.
      paragraphMap & parags = templateFile.getParagraphs();
      paragraphMap::iterator  pos = parags.find("begin");
      if( pos != parags.end() )
      {
         beginDoc = pos->second;
         endDoc = parags["end"];
         formTemplate = parags["middle"];
         pageBreak = parags["pagebreak"];
      }
      else
      {
        formTemplate = templateFile.getUnparsedHtml();
        
        
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

    // see if it is a mutipart
    if( beginDoc.length() ) webIO << beginDoc;
    
    derived_commence_event();
    while( more_data )
    {
      derived_prerow_event();

      // parse through the template, performing markup tag replecements
      parseAndReplace();
      derived_postrow_event();
      
      if( pageBreak.length() ) webIO << pageBreak;
      
      more_data = rs.next();
      if( more_data ) row++;
    }
    derived_complete_event();
    if( endDoc.length() ) webIO << endDoc;
     
    return * this;
  }
  void parseAndReplace( void )
  {
    string::size_type start = 0;
    string::size_type found = 0;
    string attrDelim =  "'\"";
    do
    {
      found = formTemplate.find("<markup",start);   // <markup

      if( found != string::npos )
      {

        webIO << formTemplate.substr(start,found-start);
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
            // DBG            webIO << "<h2>type: " << theType << "</h2>" << endl;
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
                  // DBG                  webIO << "<h2>colname: " << colName << "</h2>" << endl;
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
                    webIO << "<!-- rep Id " << repId << " --> ";
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
          // DBG          webIO << "<h2>found: " << found << "</h2>" << endl;
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
            webIO << "<h1>Couldn't find end markup marker</h1>" << endl;
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
    if( rs.hasField(fldName.c_str()) )
    {
      webIO << formatField(fldName);//rs.getField(fldName.c_str()).format();
    }
    else
    {
      webIO << "<b>NO SUCH FIELD " <<  fldName << " IN DATASET</b>" << endl;
    }
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
