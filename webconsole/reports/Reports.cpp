/*
  Reports.cpp

  User Interface Definition and Implementation 
  for a Report Run.
  
  Copyright (C) 2005 by D.K. McCombs.
  W3 Systems Design www.w3sys.com    
  
*/

// interface class (list) 
#define NO_INSERT_BUTTON  
#define NO_DELETE_BUTTON

#include "Reports.hpp"

#include "openLogin.h" // open-login has own connectstring

openLogin oLogin;

int main( int argcount, char ** args )
{
    
  if( oLogin.testLoginStatus() )
  {
    cgiEnvironment cenv;
    
    cgiInput & args = cenv.ClientArguments();    
    cgiTemplates pgTemplate; 
    if( args.count("Id")==0 )   
    {
      cgiScript script( "text/html", false );
      Report_List mylist(script,oLogin.Session());
      script.closeHeader();
      pgTemplate.load("Templates/adminPane.htmp");

      script << ocString(pgTemplate.getParagraph("top"))
                      .replaceAll("$heading$","Report");

      mylist.loadListTemplates("Templates/list.htmp");  
      mylist.list_display();

      ocString end = pgTemplate.getParagraph("bottom");
      // dummy form to satisfy client scripts
      script << "<form id='uiForm' name='uiForm'><input type='hidden' id='xmldirections'></form>" << endl;    
      
      script << end;
      
    }
    else
    {      
      Report_Obj report;
      report.key( atoll(args["Id"].c_str()) );
      report.Id = report.key();
      if( report.get_data() )
      {          
        cgiScript script(report.getMediaType().c_str());
        // copy the map to get any post info from cenv
        script.ClientArguments().TheMap() = args.TheMap();
        // New report parameters Part 1
        Report_Parameters_form param_form(script);
        param_form.Report_Id = report.Id;
        param_form.get();
        if( param_form.haveFormValues() )
        { 
          // End New report parameters Part 1          
          // build a path
          string path = "reportTemplates/" + report.getTemplatePath();
          
          string sql = report.getQuery();

          // fix the query with the parameters
          sql = param_form.paramFilters( sql );

          
          // the column handling report
          col_handling_rep rpt(script);          

          rpt.loadTemplates(path); 
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
            cgiHtml html;
            { cgiHead head;
              head << "<style>"
                "body {color:darkred;background:#ddd;font-size:12pt;border:8pt solid #fff;margin:0pt;padding:30pt;text-align:center;} "
                "h2 {color:#00a;background:transparent;}"
                "</style>" << endl;
            }
            { cgiBody body(" onLoad='window.focus()'");
              script << "<h2>Sorry!</h2>There is no data for the criteria that you have selected.<br>"
                "Please try again with different criteria <!--" << report.getQuery() << "-->" << endl;
            }
          }
        // New report parameters Part 2
        } // have params?
        else
        {
          pgTemplate.load("Templates/adminPane.htmp");

          script << ocString(pgTemplate.getParagraph("top"))
                          .replaceAll("$heading$","Report");

          param_form.loadControlTemplates("Templates/childdivform.htmp"); 
          param_form.form_display();
          ocString end = pgTemplate.getParagraph("bottom");
          script << end;          

        }
        // End new report parameters Part 2        
      }      
    }
  }
  else
  {
    cgiScriptLite script;
    script.Redirect("signIn.html"); 
  } 
}
// compile implementations here
#include "read_write_base.cpp"
#include "forms_base.cpp"

