/*
  Reports.hpp

  MUST HAVE TEMPLATES:

  For Parameters:
       Templates/reportPane.htmp
       Templates/repform.htmp

  User Interface Class Definition 
  for Report.

  Copyright (C) 2004 by D.K. McCombs.
  W3 Systems Design
*/
#ifndef REPORTS_HPP
#define REPORTS_HPP

#include <iostream>
#include <iomanip>
#include "cgiTemplates.h"
#include "connectstring"
#include "forms.h"
#include "ocTypes.h"
#include "ocString.h"
#include "cgiTemplates.h"
#include "openLogin.h"

#include "Report.hpp"
#include "sublist_base.hpp"


// new report parameter capability
#include "Report_Params_ui.h"

#include "col_handling_rep.hpp"
#include "siteLimit.h"

using namespace std;

class Report_List:  public sublist_base
{
  string lastCat;
  cgiScript & scr;

public: 
  // Constructor
  Report_List(cgiScript&sc):sublist_base(sc), scr(sc)
  {
    ;
  }  
  ~Report_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;
  
    hotCol=-2; 
    skipCol=0;
      
    editLink = listTemplate.getParagraph("hotcolumn");
    
    
    string sql = "select r.Id, r.Category, r.Name, t.File_Extension, r.External_Program from Report r "
                "left join Report_Type t on t.Id = r.Media_Type where r.Show_In_List > 0 ";
    if( addedCriteria.length() )
    {
      sql +=    " and ";
      sql += addedCriteria;
    }
    sql +=    " order by r.Category, r.Name";
    webIO << "<!-- " << sql << "-->" << endl;
    if( getData( sql ) == false )
    {
       webIO << "<!-- " << sql << "-->" << endl;
    }
    
    webIO << listTemplate.getParagraph("list_begin"); 
    
    emitData();
             
    emitEnd();
    return breturn;
  }
  virtual void sendHotField( int iField, ocString & td )
  {
    string cat;    
    ocString th = listTemplate.getParagraph("th");
    ocString editLink2;
    string program = rs.getField("External_Program").format();
    if(  program.length() == 0 ) program =  "Reports.cgi";
    string key;
    string extension;
    string qString = scr.QueryString().c_str();
    switch(iField)
    {
    case 1:
      cat = rs.getField(iField).format();
      if( cat != lastCat )
        webIO <<  th.replace( "$data$", cat.c_str() );     
      else
      webIO <<  th.replace( "$data$", "" );   
      lastCat =  cat;  
    break;
    
    case 2:    
      extension= rs.getField(3).format();
      if( extension.length() > 0 )
      {
        key += "/" + rs.getField(iField).format() + "." + extension;
      }
      key +=  "?Id="  + rs.getField(0).format();
      if( qString.length() )
      {
        key +=  "&" + qString;
      }
      editLink2 = editLink.replace("$prog$",program);
      webIO <<  td.replace( "$data$",
                      editLink2.replaceAll( "?Id=$key$",key.c_str())
                               .replaceAll( "$col$",rs.getField(iField).format().c_str()).c_str());
    break;
    }
  }
    
};

class ReportEmitter
{
  cgiEnvironment cenv;
  ocString qs;
  bool isPDF;
  ocString scriptName;
  cgiInput & args;
  cgiTemplates pgTemplate;
  string mediaType;
  Report_Obj report;
  ocString sql;
  bool addPDFLink;
  public:
    ReportEmitter():cenv(),qs(cenv.QueryString().c_str()),args(cenv.ClientArguments()),addPDFLink(false)
    {
      isPDF = qs.regExMatch("isPDF");
      scriptName = cenv.ScriptName().c_str();
#ifdef REPORTDOESPDF
      addPDFLink = true;
#endif
    }
    ~ReportEmitter(){;}
    void run( void )
    {  
      if( oLogin.testLoginStatus() || isPDF )
      {
        if( args.count("Id")==0 )
        {
          /// They didn't ask for any particular report, so show them a list
          /// This is really the job of ReportListing.meta and is atypical but handled
          listReports();
        }
        else
        {
          report.key( atoll(args["Id"].c_str()) );
          report.Id = report.key();

          if( report.get_data() )
          {
            mediaType = report.getMediaType();
            cgiScript script(mediaType.c_str(),false,"",true);
            // copy the map to get any post info from cenv
            script.ClientArguments().TheMap() = args.TheMap();
            Report_Parameters_form param_form(script);
            param_form.Report_Id = report.Id;
            param_form.get();
            // Do we have enough parameters to satisfy report criteria
            if( param_form.haveFormValues() == false )
            {
              paramForm(script,param_form);
            }
            else
            {
              if( mediaType == "application/pdf" && ! isPDF )
              {
                redirectToHTMLDoc();
              }
              else
              {
                script.openHeader();
                script.closeHeader();
                // show the report if all parameters have been satisfied
                genReport(script,param_form);
              }
            }
          }
          else
          {
          // This is a very unusual case, it couldn't find a report to run.
            noData();
          }
        }
      }
      else
      {
      // no authorization to run this report
        invalidLogin();
      }
    } // end of run()
  
  private:
      
    void genReport(cgiScript & script, Report_Parameters_form &param_form)
    {
    /// build a path
      string path = "reportTemplates/" + report.getTemplatePath();
      string sql = report.getCompositeQuery();

    /// fix the query with the parameters
      sql = param_form.paramFilters( sql );

    /// the column handling report
      col_handling_rep rpt(script);
      rpt.Formatting_Modulus = report.Formatting_Modulus;
      rpt. Items_Per_Page = report.Items_Per_Page;
      rpt.loadTemplates(path);

      rpt.addColumnHandlers( report.Id );
      if(  args.count("doParamFilter") )
        rpt.doParamFilter = true;
      if( rpt.getData(sql) )
      {
        rpt.emitTop(report.Name);
        rpt.emitHeader();
        rpt.emitData();
        // show the formatted list of parameters
        ocString extra = param_form.filterList(path);
      // end of report stuff for back-link and pdfdoc
        if( addPDFLink )
        {
          bool isParameterized = false;
          if( !isPDF )
          {
            extra += "<a href='/htmldoc.cgi"  + scriptName;
            if(  args.TheMap().size() > 0 )
            {
              extra += "?";
              for(  queryStringMap::iterator pos = args.TheMap().begin();
                    pos != args.TheMap().end();
                 )
              {
                extra += pos->first.c_str();
                extra += "=";
                extra += pos->second.c_str();
                ++pos;
                if( pos != args.TheMap().end() )
                {
                  ocString paramTest = pos->first;
                  if( paramTest == "xmldirections" )
                  {
                    isParameterized = true;
                    break;
                  }
                  extra += "&";
                }
              }
            }
            extra += "&isPDF=1'>[As PDF]</a>";
            if( isParameterized )
            {
              extra += " &nbsp; <a href='" + scriptName
                  + "?"  + qs
                  + "'>[BACK]</a>";
            }

            rpt.emitEnd(extra);
          }
          else
          {
          // normal end of report
            rpt.emitEnd(extra);
          }
        }
        else
        {
          rpt.emitEnd(extra);
        }
      }
      else
      {
        rpt.emitTop(report.Name);
        rpt.emitEmptySet();
        rpt.emitEnd("<a href='javascript:window.close()'>[Quit]</a>");
      }
    }
    void paramForm(cgiScript & script, Report_Parameters_form & param_form)
    {
      script.openHeader("text/html");
      script.closeHeader();

      if( ! pgTemplate.load("Templates/reportPane.htmp") )
      {
        script << "<h2>could not load Templates/reportPane.htmp</h2>" << endl;
      }
  
      script << ocString(pgTemplate.getParagraph("top"))
          .replaceAll("$heading$","Reports");
  
      if( !param_form.loadControlTemplates("Templates/repform.htmp") )
      {
        {
          script << "<h2>could not load Templates/repform.htmp</h2>" << endl;
        }
      }
      param_form.reportName = report.Name;
      param_form.form_display();
      ocString end = pgTemplate.getParagraph("bottom");
      script << end;
    }

  
    void noData( bool needScript = true )
    {
      if( needScript )
      {
        cgiScript script;
      }
      cgiHtml html;
      {
        cgiHead head;
        head << "<style>"
            "body {color:darkred;background:#ddd;font-size:12pt;border:"
            "8pt solid #fff;margin:0pt;padding:30pt;text-align:center;} "
            "h2 {color:#00a;background:transparent;}"
            "</style>" << endl;
      }
      {
        cgiBody body(" onLoad='window.focus()'");
        body << "<h2>Sorry!</h2>"
            "There is no data for the criteria that you have selected.<br>"
            "Please try again with different criteria <!--"
            << sql << "-->" << endl;
      }
    }
    void invalidLogin( void )
    {
      cgiScript script;
      cgiHtml html;
      {
        cgiBody bod;
        bod << "Invalid Login Detected!"  << endl;
        bod << oLogin.getLastError() << endl;
      }
    }
    
    // if the mimetype is application/pdf then ...
    void redirectToHTMLDoc( void )
    {
      cgiScript script( "text/html", false );
        // build the redirect string
      string redir = "/htmldoc.cgi";
      redir += scriptName;
      redir += "?isPDF=1";
      if(  args.TheMap().size() > 0 )
      {
            
        for(  queryStringMap::iterator pos = args.TheMap().begin();
              pos != args.TheMap().end();
           )
        {
          redir += "&";
          redir += pos->first;
          redir += "=";
          redir += pos->second.c_str();
          ++pos;
          if( pos != args.TheMap().end() )
          {
            ocString paramTest = pos->first;
            if( paramTest == "xmldirections" )
            {
              break;
            }
          }
        }
      }
      script.Redirect( redir.c_str() );
      script.closeHeader();
    }
  /**
    a filter over the list of viewable reports found in
    the logins service list, xmlparam1 which is in group.service xmlparams1
    or the role service xmlparam, depending on security model (lite or not).
    Filters Reports by comma delimited ids the XML 'view' tag
   **/
    void addListCriteria( Report_List & repList )
    {
      if( oLogin.fetchServices() )
      {
        serviceMap::iterator pos = oLogin.Services().find("ReportListing.meta");
        if( pos != oLogin.Services().end() )
        {
          openService & service = pos->second;
          string::size_type idx = service.xml_param1.find("<view>");
          if( idx != string::npos )
          {
            idx += 6; // strlen of <view>
            string::size_type idx2=service.xml_param1.find("</view>");
            if( idx2 != string::npos && idx2 > idx )
            {
              string reps = service.xml_param1.substr(idx, idx2-idx);
              repList.addedCriteria = " where r.Id in ( " + reps + " ) ";
            }
          }
        }
      }
    }
    void listReports( void )
    {
      cgiScript script( "text/html", false );
      Report_List mylist(script);
      script.closeHeader();
      pgTemplate.load("Templates/adminPane.htmp");

      script << ocString(pgTemplate.getParagraph("top"))
          .replaceAll("$heading$","Report");

      mylist.loadListTemplates("Templates/list.htmp");
      
      // contrain view of reports to what is allowed
      addListCriteria(mylist);
      
      mylist.list_display();

      ocString end = pgTemplate.getParagraph("bottom");
      // Dummy form to satisfy client scripts
      script << "<form id='uiForm' name='uiForm'><input type='hidden' id='xmldirections'></form>" << endl;
      script << end;
    }
};

#endif



