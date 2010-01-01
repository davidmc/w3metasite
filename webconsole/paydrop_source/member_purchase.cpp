/*
  Member Purhase Form
  ===================================
  CGI User Interface for Pay Drop Member
*/
#include "connectstring"
#include <iostream>
#include <iomanip>
#include "ocTypes.h"
#include "ocString.h"
#include "read_write_base.hpp"
#include "forms_base.hpp"
#include "list_base.hpp"
#include "cgiTemplates.h"
#include "nav_menu.h"
#include "MemberPurchase.hpp"
#include "Member.hpp"
#include "Store.hpp"
#include "Marketer.hpp"
#include "ocFileSys.h"
#include "ocImportParser.h"

#include "Purchase_Import_Results.hpp"
// for mail alerts
#include "mail_control.hpp"

// use the logger for debugging
#define DO_OPEN_LOGGING
#include "openLogger.h"

typedef map<string, string> email_addresses;

class Member_Purchase_Form:  public read_write_base, public forms_base
{
public:

  long long MarketerID;
  string marketerFile;
  
  // Common Objects
  ocFileSys fs;
  Marketer marketer;
  
  email_addresses accountChangeAlerts;
  email_addresses accountLacksAlerts;   
  
  // Constructor
  Member_Purchase_Form(cgiScript & script)
  :read_write_base(),forms_base(script),MarketerID(0)
  {setKey(*this);}  
  ~Member_Purchase_Form(){;}
  // Implement pure virtual methods of form base 
  void form_id_transfer( void ) {;}
  bool dbf_action( string, changeMap & ){ return true; }
  
  // transfer form vars to local vars
  void form_data_transfer( void )
  {
    llongFXfer( "MarketerID", MarketerID );
    stringFXfer( "marketerFile", marketerFile );   
  } 
  // import member payments
  bool importRecords ( string & filepath )
  {
    // Create a file parser of the appropriate type
    // value="1" csv: value="2" tab delimited: value="3" pipe delimited
    ocFileParser parser;
    switch( marketer.FileType )
    {
      case 1:
        parser.setDelim(",").setCheckQuote(true);
        m_result += "<br>\nCsv file";    
        break;
      case 2:
        parser.setDelim("\t");
        m_result += "<br>\nTab delimited file";     
        break;
      case 3:
        parser.setDelim("|");
        m_result += "<br>\nPipe delimited file";     
        break;
      default:
        m_result += "<br>\nBad file type in marketer record! ";
        m_result.append(marketer.FileType);
        return false;
    }
    
    // open
    bool isOK = parser.openFile(filepath);
         
    // move off all of the header lines    
    for(int i=0; i < marketer.HeaderLines; i++)
    {
      parser.getLine();
      m_result += "<br>\n| &nbsp; ";
      m_result += parser.getLineTokens();
    }

    m_result += "<br>\nMoved off the header - traversed ";
    m_result.append(marketer.HeaderLines);
    m_result += " lines.";
    
    // next line should be data, get the line, parse as header for col count, then as data for values
    isOK = parser.getLine();
    if(isOK) isOK = parser.parse(true);
    
    m_result += "<br>\nData columns in File: ";
    m_result.append(parser.getColCount());    
    do
    {
      isOK = parser.parse();
      
      // make a new pristine Purchase_Import_Results...
      Purchase_Import_Results pir;
      
      // Map values, validating for validity, and minimum completeness
      if( pir.Map( marketer, parser.getCols(), parser.getColCount() ) )
      {
        // Insert transactions, check for duplicates
        checkTransaction(pir);
        // insert the record;      
        pir.db_insert();
      }
      else
      {
        m_result += pir.last_result();
        return false;
      }      
      isOK = parser.getLine();
      
    } while (isOK);    
    // Maps the fields to the Purchase_Import_Results        
  }
  void checkTransaction(Purchase_Import_Results & pir)
  {
    // no need to do nothing if pir is failed - excuse bad grammer
    if( pir.Status != statii[efail] )
    {
      // create a member purchase
      MemberPurchase purchase;
      // check if the purchase is already in the system
      ocString clause = "PurchaseDate = '";
      clause += dynamic_cast<time_date&>(pir.PurchaseDate).format("%Y-%m-%d");
      clause += "' and MemberID = ";
      clause.append(pir.MemberID);
      clause += " and StoreID = ";
      clause.append(pir.StoreID);
      clause += " and PurchaseAmount = ";
      clause.append( pir.PurchaseAmount.amount() );
      // if there is an order number, 
      //   use it as criteria as well
      if(  pir.PurchaseOrderNumber.length() )
      {
         clause += " and PurchaseOrderNumber = '";
         clause += pir.PurchaseOrderNumber;
         clause += "'"; 
      }

      // debugging
      writelog2( "Check Purchase Date:", clause ); 
      
      if( purchase.get_data( clause ) )
      {
        writelog( "checkTransaction Found Duplicate!");
        // IT's A DUP!
        pir.Status = statii[edup];        
      }
      else
      {
        // insert it
        purchase.MemberID = pir.MemberID;
        purchase.StoreID = pir.StoreID ;
        purchase.PurchaseDate = pir.PurchaseDate;
        purchase.PurchaseOrderNumber = pir.PurchaseOrderNumber ;
        purchase.PurchaseSkuNumber = pir.PurchaseSkuNumber ;
        purchase.PurchaseAmount.amount() = pir.PurchaseAmount.amount() ;
        purchase.PurchaseDiscountAmount = pir.CalculatedDiscountAmount.amount();
        if( !purchase.db_insert() )
        { 
          m_result += "MemberPurchase insert failure";
          writelog2( "checkTransaction", m_result ); 
        }
        else
        {
          ocString ocs_MemberID;
          ocs_MemberID.append(pir.MemberID);
          // see if we should send email
          if( pir.MemberIsOrganization == false)
          {
            // determine what kind of email to send...
            if(pir.isIncompleteRegistration)
            { writelog2( "Added to Incomplete Registration Notification: ", pir.trueEmailAddress );
              accountLacksAlerts[pir.trueEmailAddress] = ocs_MemberID;            
            }  
            else 
            { writelog2( "Added to Account Change Notification: ", pir.trueEmailAddress );
              accountChangeAlerts[pir.trueEmailAddress] = ocs_MemberID;
            }  
          }
          else
          {
            writelog( "checkTransaction bypassed notification"
                      "Cause detected organization" );
          }  
        }  
      }            
    }
  }
  bool form_action( void )
  {
    form_data_transfer();
    
    // This is not an error, happens on GET method, nothing to do
    if( MarketerID == 0 )
    {
      m_result = "I'm waiting for you to do something...";
      return true;
    }
    
    // Have to be able to get the marketer
    marketer.key(MarketerID);  
    if( marketer.get_data() == false )
    {
      m_result = "I couln't get a marketer with id = ";
      m_result.append(MarketerID);
      return false;
    }    
    // see if there is a file
    if( marketerFile.length() )
    {
      cgiInput & args = script.ClientArguments();
      string filepath = "import/";
      filepath += marketerFile;       
      if( fs.is( filepath ) && fs.fileSize(filepath) > 0)
      {
        m_result = "OK<br>\nMarketer: ";
        m_result += marketer.MarketerName;
        m_result += "<br>\nTransaction file: ";
        m_result += filepath;
        // Kill the data in the transaction file
        string sql = "delete from Purchase_Import_Results";
        if( cmd.execute(sql) )
        {
          if( importRecords( filepath ) )
          {
            sendAlerts();
            return true;
          }
        }
        else
        {
          m_result = "I couldn't delete the temporary import results!<br>\n";
          m_result += sql;
        }
      }
      else
      {
        m_result = "I can't open the  Marketer Transaction File ";
        m_result += filepath;        
      }      
    }
    else
    {
      m_result = "I need a Marketer Transaction File";
    }
    return false;  
  } 
  
  void sendAlerts( void )
  {
    string sql = "select ConfigValue from Config where ConfigVariable = 'Send Notices'";
    string sendNotices = "true";
    if( rs.open(sql) )
    {
      sendNotices = rs.getField(0).format();
      rs.close();
      writelog2( "Send Notices?", sendNotices );
    }
    if( sendNotices != "false" )
    {
      email_addresses::iterator pos;
      // first send account updates
      for( pos=accountChangeAlerts.begin();pos!=accountChangeAlerts.end();++pos )
      { writelog2( "Sending account change mail to: ", pos->first );
        sendMail( pos->first, "custcare@paydrop.com",  
                  "", "mail/accountchange", script.ServerName(), pos->second ); 
      }    
      // next send account lacks
      for( pos=accountLacksAlerts.begin();pos!=accountLacksAlerts.end();++pos )
      { writelog2( "Sending account incomplete mail to: ", pos->first );
        sendMail( pos->first, "custcare@paydrop.com", 
                  "", "mail/updateaccount", script.ServerName(), pos->second ); 
      }
    } // end if should send mail notices   
  }
  
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    ocString buttonGroup = formTemplate.getParagraph("button_group");
    script << makeTop( "member_purchase.cgi", "Upload Member Purchases"
                                              "<p style='font-size:8pt; font-weight: 100;'>"
                                              "Upload member purchases from marketers here.</p>" )
           << makeComboBox( "Marketer*:","MarketerID",MarketerID,
                            "select MarketerID, MarketerName from Marketer" )
           << makeFileBox( "Transaction File", "marketerFile", marketerFile, "45")
           << buttonGroup.replace("$buttons$","<input type='reset'> &nbsp; <input type='submit' value='Import Transactions'>")
           << makeBottom( m_result ) 
           << endl; 
    return breturn;
  }      
};

class Member_Purchase_List:  public list_base
{
  ocString versatile,fixed;
  string tr,end_tr;
  string current_status, current_refund;
  int count;
  double sum, dsum;
public: 
  // Constructor
  Member_Purchase_List(cgiScript&sc):list_base(sc),count(0),sum(0.0),dsum(0.0)
  {
    ;                   
  }  
  ~Member_Purchase_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;
    editLink = "$col$";    
    versatile  = listTemplate.getParagraph("multi");
    fixed = listTemplate.getParagraph("th"); 
    tr = listTemplate.getParagraph("tr"); 
    end_tr = listTemplate.getParagraph("end_tr"); 
    emitHeadings("Universal<br>ID|Marketer<br>Store<br>ID|Purchase<br>Date|"
                 "Purchase<br>Amount|Calculated<br>Discount<br>Amount|Is<br>Return|Status|"
                 "Member<br>ID|Store<br>ID|Purchase<br>Order<br>Number|Purchase<br>Sku<br>Number" );             
    
    if( getData("select UniversalID,MarketerStoreID,PurchaseDate,"
                "PurchaseAmount,CalculatedDiscountAmount,"
                "case IsReturn when 1 then 'Yes' else 'No' end,"
                "Status,"
                "MemberID,StoreID,PurchaseOrderNumber,PurchaseSkuNumber "
                "from Purchase_Import_Results order by IsReturn asc,Status desc"
               ) )
    {
      setColFormat( "%.2f", 3 );   
      setColFormat( "%.2f", 4 );                            
      emitData(); 
    }    
    emitEnd();
    return breturn;
  }
  string SalesOrRefund( string isRefund )
  {
    return isRefund=="No"?"Sales":"Refunds";
  }
  void derived_commence_event( void )
  {
 //     webIO << listTemplate.getParagraph("tr") << 
 //                 versatile.replace("$cols$","11").replace("$data$","Sales - Good Records") << 
 //                 listTemplate.getParagraph("end_tr"); 
     current_refund = "No";
     current_status = "Good";
  }
  bool derived_predata_event( void )
  {

    if( rs.getField(6).format() != current_status || 
        rs.getField(5).format() !=  current_refund  ) 
    {       
      // reheader by forcing complete event
      derived_complete_event();
        
      // get latest groupt data           
      current_status = rs.getField(6).format();
      current_refund = rs.getField(5).format();
      count=0;sum=0.0;dsum=0.0;                                         
    }
    return true;    
  }
  void derived_complete_event( void )
  {
    // reheader   
    ocString topic = SalesOrRefund ( current_refund ); 
    topic += " &nbsp; - &nbsp; ";
    topic += current_status;
    topic += " Records &nbsp; - &nbsp; Count: ";
    topic.append(count);
    topic += " &nbsp; - &nbsp; Total: ";
    topic.append(sum);
    topic += " &nbsp; - &nbsp; Discount Total: ";
    topic.append(dsum);      
    webIO << listTemplate.getParagraph("tr") << 
            versatile.replace("$cols$","11").replace("$data$",topic.c_str()) << 
            listTemplate.getParagraph("end_tr");
               
  }
  
  void derived_data_event( void )
  {  
    count++;
    double test = atof(rs.getField(3).format().c_str());
    if( isnan(test) == 0 ) sum += test;
    test = atof(rs.getField(4).format().c_str());
    if( isnan(test) == 0 ) dsum += test;
  }
          
};

int main( int argc, char ** argv )
{  
  cgiScript scrpt("text/html",  // mime type
                  false,        // don't close the header yet
                  "import");    // file upload path
                  
  Member_Purchase_Form form(scrpt);
  Member_Purchase_List mylist(scrpt); // so you can set cookies 
  if( oLogin.testLoginStatus() )
  {
    scrpt.closeHeader();
    cgiTemplates pgTemplate;    
    pgTemplate.load("Templates/adminPane.htmp");
    // change the pick routine
    ocString top = pgTemplate.getParagraph("top");
    scrpt << top.replaceAll("filePick.cgi","fileImportPick.cgi");
    form.loadControlTemplates("Templates/form.htmp");
        
    form.form_action();
    form.form_display();
    
    scrpt << "<hr><br>\n<h4>Result of last import: </h4>" << endl;
    
    mylist.loadListTemplates("Templates/reportlist.htmp");      
    mylist.list_display();
    
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



 
