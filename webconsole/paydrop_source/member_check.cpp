/*
  Member Check Form
  ===================================
  CGI User Interface for Pay Drop Member
*/

#include <iostream>
#include <iomanip>
#include "ocTypes.h"
#include "ocString.h"
#include "connectstring"
#include "read_write_base.hpp"
#include "forms_base.hpp"
#include "list_base.hpp"
#include "cgiTemplates.h"
#include "nav_menu.h"
#include "MemberPurchase.hpp"
#include "MemberCheck.hpp"
#include "Member.hpp"
#include "Store.hpp"
#include "Marketer.hpp"
#include "Config.hpp"

#include "ocFileSys.h"
#include "ocImportParser.h"

// for mail alerts
#include "mail_control.hpp"

typedef map<string, string> email_addresses;

class Member_Check_Form:  public read_write_base, public forms_base
{
public:

  ocString sqlQualPurchases, sqlUpdatePurchase;  

  // common objects  
  Config cfg;  
  
  // email addresses
  email_addresses accountLacksAlerts;
  email_addresses accountCreditedAlerts;
  
  // config data
  string DefaultId;
  string MinCreditDays;
  string MinCreditAmount;
  string LastCreditRun;
  
  bool run;
  int noQulified, noCredited, noSkipped;
  
  
  // Constructor
  Member_Check_Form(cgiScript & script)
  :read_write_base(),forms_base(script)
  ,run(false),noQulified(0), noCredited(0), noSkipped(0)
  {
    setKey(*this);
    sqlQualPurchases =  "SELECT MemberID, SUM(PurchaseDiscountAmount) AS PurchaseDiscountTotal " 
          "FROM MemberPurchase WHERE curdate() >= date_add(PurchaseDate, interval %iMinCreditDays day ) "
          "AND MemberID <> %DefaultMember AND MemberCheckID = 0 GROUP BY MemberID";
    sqlUpdatePurchase = "Update  MemberPurchase set MemberCheckID = %checkid "
          "WHERE MemberID = %memberid AND MemberCheckID = 0 AND "
          "curdate() >= date_add(PurchaseDate, interval %iMinCreditDays day )";     
  }  
  ~Member_Check_Form(){;}
  // Implement pure virtual methods of form base 
  void form_id_transfer( void ) {;}
  bool dbf_action( string, changeMap & ){ return true; }
  
  // transfer form vars to local vars
  void form_data_transfer( void )
  {
    stringFXfer( "DefaultId", DefaultId );   
  } 
 
  
  bool form_action( void )
  {
        
    // get the form data
    form_data_transfer();
    
    // should have the default id set if this is a form submission
    run = DefaultId.length() > 0;
    
    // get from the config table
    if( cfg.get_data(" ConfigVariable = 'Default Login Id' ") ) DefaultId = cfg.ConfigValue;
    if( cfg.get_data(" ConfigVariable = 'Minimum Credit Days' ") ) MinCreditDays = cfg.ConfigValue;
    if( cfg.get_data(" ConfigVariable = 'Minimum Credit Amount' ") ) MinCreditAmount = cfg.ConfigValue;
    if( cfg.get_data(" ConfigVariable = 'Last Credit Run' ") ) LastCreditRun = cfg.ConfigValue;
    if( run )
    {
      string sql = sqlQualPurchases.replace("%iMinCreditDays",MinCreditDays.c_str())                                   
                                   .replace("%DefaultMember",DefaultId.c_str());
      bool isOpen =  rs.open(sql.c_str());                           
      while( isOpen )
      {
        // for each qualifying member
        Member member;
        member.key( atoll(rs.getField(0).format().c_str()) );
        if( member.get_data() )
        {
          ocString ocs_MemberID;
          ocs_MemberID.append(member.MemberID);
          // see if the address is incomplete, if so add to map
          if( member.MemberPostalCode.length() && member.MemberAddress1.length() )
          {
            // good member, check if amount is high enough
            double credit = atof(rs.getField(1).format().c_str());
            double minCredit = atof( MinCreditAmount.c_str() );
            if( credit >= minCredit )
            {
              // create the check record
              MemberCheck mcheck;
              mcheck.MemberID = member.MemberID;
              mcheck.CheckAmount = credit;
              mcheck.db_insert();
              noCredited++;
              // update the payments to reflect the check id
              if( !updatePaymentRecords(mcheck) )
              {
                // Error! Roll back the check record!
                mcheck.db_delete();
                noCredited--;
              }
              else
              {
                accountCreditedAlerts[member.MemberEmail] = ocs_MemberID;
              } 
            }                       
          }
          else
          {
            // Lack member info - add to lacking member list
            accountLacksAlerts[member.MemberEmail] = ocs_MemberID;
            // DON'T takes these payments out of the running for future credits
            // updatePaymentRecords(member.MemberID);
            noSkipped++;
          }          
        } 
        noQulified++;     
        isOpen =  rs.next();
      }
      sendAlerts();
      return true;
    }    
    return false;  
  } 
  bool updatePaymentRecords(MemberCheck & mcheck)
  {
    ocString memid, memcheckid;
    memid.append(mcheck.MemberID);
    memcheckid.append(mcheck.key()); 
    string sql =  sqlUpdatePurchase
                  .replace("%checkid",memcheckid)
                  .replace("%memberid",memid)
                  .replace("%iMinCreditDays",MinCreditDays);
    // turn off debugging -- script << sql;               
    return cmd.execute(sql);               
  }
  /*
  bool updatePaymentRecords(long long MemberID)
  { 
    ocString memid;
    memid.append(MemberID);
    string sql =  sqlUpdatePurchase
                  .replace("%checkid","-1")
                  .replace("%memberid",memid)
                  .replace("%iMinCreditDays",MinCreditDays);
    // turn off debugging -- script << sql;              
    return cmd.execute(sql);      
  }
  */
  void sendAlerts( void )
  {
    string sql = "select ConfigValue from Config where ConfigVariable = 'Send Notices'";
    string sendNotices = "true";
    if( rs.open(sql) )
    {
      sendNotices = rs.getField(0).format();
      rs.close();
    }
    if( sendNotices != "false" )
    {
      email_addresses::iterator pos;

      // Send account lacks
      for( pos=accountLacksAlerts.begin();pos!=accountLacksAlerts.end();++pos)
      {
        sendMail( pos->first, "custcare@paydrop.com",
                  "",  // subject will come from meta_description 
                  "mail/missedrebates", script.ServerName(),  pos->second ); 
      }
      for( pos=accountCreditedAlerts.begin();pos!=accountCreditedAlerts.end();++pos)
      {
        sendMail( pos->first, "custcare@paydrop.com",
                  "", // subject will come from meta_description
                  "mail/monthlyrebates", script.ServerName(), pos->second ); 
      }
    }    
  }
  
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    ocString buttonGroup = formTemplate.getParagraph("button_group");
    script << makeTop( "member_check.cgi", 
                       "Member Rebate Check Generation"
                       "<p style='font-size:8pt; font-weight: 100;'>"
                       "Generate monthly check records here."
                       "</p>" )
           << makeStaticBox( "Member Default Id", "DefaultId", DefaultId, "25")
           << makeStaticBox( "Minimum Credit Days", "MinCreditDays", MinCreditDays, "25")
           << makeStaticBox( "Minimum Credit Amount", "MinCreditAmount", MinCreditAmount, "25")
           << makeStaticBox( "Last Credit Run", "LastCreditRun", LastCreditRun, "25")                                       
           << buttonGroup.replace("$buttons$","<input type='reset'> &nbsp; "
                                              "<input type='submit' value='Generate Credits'>")
           << makeBottom( m_result ) 
           << endl; 
    return breturn;
  }
  string checkRunList( void )
  {
    string ret;
    // show all runs within a month as a list that will allow dyna-creation of .csv dump.
    bool isOpen;
    string sql = "select distinct CheckDate "
                 "from MemberCheck where "
                 "curdate() <= date_add(CheckDate, interval 5 day )";
    for( isOpen=rs.open(sql); isOpen; isOpen=rs.next() )
    {
      basicField & rDate = rs.getField(0);
      string link = "<a href='checkrun.cgi/";            
      rDate.setFormatMask("%Y-%m-%d");
      link += rDate.format();
      link += ".csv'>";       
      rDate.setFormatMask("%m/%d/%Y"); 
      link += rDate.format();
      link += "</a><br>";
      ret += link;
    }             
    return ret;             
  }      
};


int main( int argc, char ** argv )
{  
  cgiScript scrpt("text/html",false); 
  Member_Check_Form form(scrpt);    
  if( oLogin.testLoginStatus() )
  {
    scrpt.closeHeader();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/adminPane.htmp");
    
    scrpt << pgTemplate.getParagraph("top");
    
    form.loadControlTemplates("Templates/multipart.htmp"); 
    
    form.form_action();
    form.form_display();
    
    // run ran? 
    if( form.run )
    {
      scrpt << "<hr><br>\n<h4>Results of check generation: </h4>\n" << endl;    
      scrpt << "&nbsp; Number of <b>qualified</b> members for credit: &nbsp; " 
            << form.noQulified 
            << "<br>\n &nbsp; Number of members <b>credited</b>: &nbsp; " 
            << form.noCredited 
            << "<br>\n &nbsp; Number of members <b>skipped</b>: &nbsp; " 
            << form.noSkipped << "<br>\n";
    }    
    scrpt << "<hr><br>\n<h4>Recent check runs: </h4>\n" << endl;  
    scrpt << form.checkRunList();
    scrpt << pgTemplate.getParagraph("bottom");
  }
  else
  {
    scrpt.Redirect("signIn.html"); 
  } 
};

// as an alternative to building more objects
#include "read_write_base.cpp"
#include "forms_base.cpp"



 
