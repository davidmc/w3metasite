#ifndef OPEN_CORE_FORMS_H
#define OPEN_CORE_FORMS_H 123456
#include "ocControls.h"
#include "read_base.hpp"

#ifdef WEB_USER_RESTRICTED
// version for external internet web users
string formButtons( long long key,
                    string, string, string ) // unused parameters for consistent function signature
{
  string ret;
  if( key )       
  { // Appropriate for update/delete
    ret += "<input type='button' onclick='doUpdate()' name='action' class='action' value='&nbsp;Update&nbsp;'>";
  }
  else
  { // Appropriate for insert
    ret += "<input type='button' onclick='doInsert()' name='action' class='action' value='&nbsp;Continue&nbsp;'>";
  }
  return ret;
}
#else
// version for intranets / extranets 
string formButtons( string iLabel="&nbsp;Insert&nbsp;",
                    string uLabel="&nbsp;Update&nbsp;", 
                    string dLabel="&nbsp;Delete&nbsp;")
{
  string ret;
#ifndef NO_INSERT_BUTTON  
  ret += "<input type='button' onclick='doInsert()' name='action' class='action' value='";
  ret += iLabel;
  ret += "'>&nbsp;&nbsp;&nbsp;";
#endif
#ifndef NO_UPDATE_BUTTON 

  ret += "<input type='button' onclick='doUpdate()' name='action' class='action' value='";
  ret += uLabel;
  ret += "'>&nbsp;&nbsp;&nbsp;";
#endif  
#ifndef NO_DELETE_BUTTON  
  ret += "<input type='button' onclick='doDelete()' name='action' class='action' class='action' value='";
  ret += dLabel;
  ret += "'>";
#endif  
  return ret;
}
string formButtons( long long key,
                    string iLabel="&nbsp;Insert&nbsp;",
                    string uLabel="&nbsp;Update&nbsp;", 
                    string dLabel="&nbsp;Delete&nbsp;" )
{
  string ret;
  if( key )
  { // Appropriate for update/delete
#ifndef NO_UPDATE_BUTTON    
    ret += "<input type='button' onclick='doUpdate()' name='action' class='action' value='";
    ret += uLabel;
    ret += "'>&nbsp;&nbsp;&nbsp;";
#endif 
#ifndef NO_DELETE_BUTTON       
    ret += "<input type='button' onclick='doDelete()' name='action' class='action' value='";
    ret += dLabel;
    ret += "'>&nbsp;&nbsp;&nbsp;\n";
#endif    
#ifndef NO_INSERT_BUTTON     
    ret += "<input type='button' onclick='doReset()' name='action' class='action' value='&nbsp;New - Reset Form&nbsp;'>";
    ret += "&nbsp;&nbsp;&nbsp;";
    ret += "<input type='button' onclick='doNew()' name='action' class='action' value='&nbsp;New - Keep Data&nbsp;'>";
#endif       
  }
  else
  { // Appropriate for insert
#ifndef NO_INSERT_BUTTON       
    ret += "<input type='button' onclick='doInsert()' name='action' class='action' value='";
    ret += iLabel;
    ret += "'>";
#else // hide the fact you inserting, make it look like an update, you will never get here unless this is the intent
   ret += "<input type='button' onclick='doInsert()' name='action' class='action' value='";
   ret += uLabel;
   ret += "'>";
#endif    
  }
  return ret;
}
#endif

/*
  textEntry & checkEntry
*/

ocTextbox * textEntry( string name, string size, string length = "", string attr = "onChange='ctrlChanged(this)'" )
{
  attr += " id='";
  attr += name;
  attr += "''";
  string len = length.length()?length:size;
  ocTextbox * box = new ocTextbox( name, len, size, attr );
  return box;
}
ocFilebox * fileEntry( string name, string size, string length = "", string xtra_attr = "" )
{
  string attr = "onChange='ctrlChanged(this)' id='";
  attr += name;
  attr += "'";
  if(xtra_attr.length())
  {
    attr += " ";
    attr += xtra_attr; 
  }
  string len = length.length()?length:size;
  ocFilebox * box = new ocFilebox( name, len, size, attr );
  return box;
}


ocTextbox * staticEntry( string name, string size, string length = "" )
{
  string attr = "readonly class='static' id='";
  attr += name;
  attr += "'";
  string len = length.length()?length:size;
  ocTextbox * box = new ocTextbox( name, len, size, attr );
  return box;
}
ocHidden * hiddenEntry( string name )
{
  ocHidden * box = new ocHidden( name );
  return box;
}
ocTextbox * dateEntry( string name, string size )
{
  string attr = "onChange='validDate(this,true)' id='";
  attr += name;
  attr += "' class='dateChoice'";
  ocTextbox * box = new ocTextbox( name, size, size, attr );
  return box;
}
ocTextbox * timeEntry( string name, string size )
{
  string attr = "onChange='validTime(this,true)' id='";
  attr += name;
  attr += "' class='timeChoice'";
  ocTextbox * box = new ocTextbox( name, size, size, attr );
  return box;
}
ocTextbox * emailEntry( string name, string size, string length = ""  )
{
  string attr = "onChange='isEmail(this,true)' id='";
  attr += name;
  attr += "'";
  string len = length.length()?length:size; // max length
  ocTextbox * box = new ocTextbox( name, len, size, attr );
  return box;
}
ocTextbox * ssnEntry( string name, string size )
{
  string attr = "onChange='validSSN(this,true)' id='";
  attr += name;
  attr += "'";
  ocTextbox * box = new ocTextbox( name, size, size, attr );
  return box;
}
ocTextbox * zipEntry( string name, string size )
{
  string attr = "onChange='isZip(this,true)' id='";
  attr += name;
  attr += "'";
  ocTextbox * box = new ocTextbox( name, size, size, attr );
  return box;
}
ocTextbox * phoneEntry( string name, string size )
{
  string attr = "onChange='isPhoneNumber(this,true)' id='";
  attr += name;
  attr += "'";
  ocTextbox * box = new ocTextbox( name, size, size, attr );
  return box;
}
ocTextbox * longNumberEntry( string name, string size )
{
  string attr = "onChange='hasFormattedNumericData(this,true)' id='";
  attr += name;
  attr += "'";
  ocTextbox * box = new ocTextbox( name, size, size, attr );
  return box;
}
ocTextbox * shortNumberEntry( string name, string size )
{
  string attr = "onChange='hasNumericData(this,true)' id='";
  attr += name;
  attr += "'";
  ocTextbox * box = new ocTextbox( name, size, size, attr );
  return box;
}
ocPassword * pswdEntry( string name, string size )
{
  string attr = "onChange='ctrlChanged(this)' id='";
  attr += name;
  attr += "'";
  ocPassword * box = new ocPassword( name, size, size, attr );
  return box;
}

// This function Always gets All data
ocSelect * cmboEntry( string name, string sql,
                      ocString defaulted = "",
                      string multiple = "" )
{  
  // instance of the combo box
  string attributes = " name='";
  attributes += name;
  attributes += "' id='";
  attributes += name;
  attributes += "'";
  attributes += " onChange='ctrlChanged(this)' ";
  attributes += multiple;
  ocSelect * combo = new ocSelect(attributes);
  // fill the combo box
  if(  defaulted.length() > 0 )
  {
    if(defaulted.find("=") == string::npos) // just a label 
    {
      combo->addOption( defaulted, "-1" );
    }
    else // a label with a specific value
    {
      string lbl =  defaulted.parse("=");
      string vlu =   defaulted.parse("="); 
      combo->addOption( lbl,  vlu ); 
    }
  }
  if( sql.length() )
  {
    quickQuery qq;
    openRS & rs = qq.getData(sql);
    for( bool result=qq.opened; result; result=rs.next() )
    {
      string label = "";
      for( int lx=1; lx<rs.getFieldCount(); lx++ )
      {
        if( label.length() && rs.getField(lx).format().length() ) label += " - ";
        label += rs.getField(lx).format();
      }
      combo->addOption( label, rs.getField(0).format() );
    }
  }
  return combo;
}

/* 
  keyVals must be in form "label1=key1,label2=key2,...,labeln=keyn"
  CAVEAT:
  Since keyVals uses a comma "," to delimit pairs and 
  an equal sign "=" to separate the value from the label, 
  those values should be avoided in the key or the label.
*/  
ocSelect * cmboManualEntry( string name,
                            ocString keyVals = "",
                            string defaulted = "",
                            string multiple = "" )
{
  // instance of the combo box
  string attributes = " name='";
  attributes += name;
  attributes += "' id='";
  attributes += name;
  attributes += "'";
  attributes += " onChange='ctrlChanged(this)' ";
  attributes += multiple;
  ocSelect * combo = new ocSelect(attributes);
  // fill the combo box
  if(  defaulted.length() > 0 )
  {
    combo->addOption( defaulted, "-1" );
  }
  if( keyVals.length() )
  {    
    while( keyVals.endOfParse() == false )
    {
      ocString pair = keyVals.parse(",");
      string val = pair.parse("=");
      string label = pair.parse("=");
      combo->addOption( val, label );
    }
  }
  return combo;
}
/* 
  keyVals must be in form "label1=key1,label2=key2,...,labeln=keyn"
  CAVEAT:
  Since keyVals uses a comma "," to delimit pairs and
  an equal sign "=" to separate the value from the label,
  those values should be avoided in the key or the label.
*/
ocCheckBoxes * checksEntry( string name,
                            ocString keyVals = "",
                            string defaulted = "",
                            string moreattributes = "" )
{
  // instance of the combo box
  string attributes = " name='";
  attributes += name;
  attributes += "' id='";
  attributes += name;
  attributes += "' ";
  attributes += moreattributes;
  ocCheckBoxes * chks = new ocCheckBoxes(name, attributes);
  // fill the combo box
  if(  defaulted.length() > 0 )
  {
    chks->addOption( defaulted, "-1" );
  }
  if( keyVals.length() )
  {
    while( keyVals.endOfParse() == false )
    {
      ocString pair = keyVals.parse(",");
      string val = pair.parse("=");
      string label = pair.parse("=");
      chks->addOption( val, label, " onChange='ctrlChanged(this)' " );
    }
  }
  return chks;
}
ocRadio * radioEntry( string name,
                      ocString keyVals = "",
                      string defaulted = "",
                      string moreattributes = ""  )
{
  // instance of the combo box
  string attributes = " name='";
  attributes += name;
  attributes += "' id='";
  attributes += name;
  attributes += "' ";
  attributes += moreattributes;
  ocRadio * rads = new ocRadio(name, attributes);
  // fill the combo box
  if(  defaulted.length() > 0 )
  {
    rads->addOption( defaulted, "-1" );
  }
  if( keyVals.length() )
  {
    while( keyVals.endOfParse() == false )
    {
      ocString pair = keyVals.parse(",");
      string val = pair.parse("=");
      string label = pair.parse("=");
      rads->addOption( val, label," onChange='ctrlChanged(this)' " );
    }
  }
  return rads;
}
/* 
  Present combo of dates in MM/DD/YYYY format
*/
ocHidden * ctrlDateGroup( string name,
                          int yearStart = 2003,
                          int yearEnd = 2015                     
                        )
{
  // instance of the combo box
  string mattributes = " name='m_" + name + "' id='m_" + name + "' onChange='m_Changed(this)' ";
  string dattributes = " name='d_" + name + "' id='d_" + name + "' onChange='d_Changed(this)' ";
  string yattributes = " name='y_" + name + "' id='y_" + name + "' onChange='y_Changed(this)' ";
  
  ocHidden * box = new ocHidden( name );
  ocSelect * mcombo = new ocSelect(mattributes);
  ocSelect * dcombo = new ocSelect(dattributes);
  ocSelect * ycombo = new ocSelect(yattributes);
  if( box && mcombo && dcombo && ycombo )
  {
    // link the ctrl list
    box->addNext(mcombo);
    mcombo->addNext(dcombo);
    dcombo->addNext(ycombo);
    
    // fill the combo boxes 
    int idt;
    ycombo->addOption("YYYY","YYYY");
    for( idt=yearStart; idt <=yearEnd; idt++ )
    {  
      ocString year;
      year.append(idt);    
      ycombo->addOption( year, year );
    }
    mcombo->addOption("MM","MM");
    for( idt=1; idt<=12; idt++ )
    {  
      ocString month;
      if( idt < 10 ) month="0";     
      month.append(idt);    
      mcombo->addOption( month, month );
    }
    dcombo->addOption("DD","DD");
    for( idt=1; idt <=31; idt++ )
    {  
      ocString day;
      if( idt < 10 ) day = "0";
      day.append(idt);    
      dcombo->addOption( day, day );
    }
  }
  return box;
}

ocTextarea * areaEntry( string name, string rows="12", string cols="80" )
{
  string attr = " name='";
  attr += name;
  attr += "' id='";
  attr += name;
  attr += "' rows='";
  attr += rows;
  attr += "' cols='";
  attr += cols;
  attr += "'";
  attr += " onChange='ctrlChanged(this)'";
  ocTextarea * ta = new ocTextarea(attr);
  return ta;
}

ocBoolbox * boolBox( string name, string id )
{
  string attr = "class='checkbox' onChange='ctrlChanged(this)' id='";
  attr += id;
  attr += "'";
  ocBoolbox * bbox = new ocBoolbox( name , attr, "1", "1" );
  return bbox;
}





#endif
#ifdef IN_T2_TESTHARNESS
cout << textEntry(  "textEntry", "12", "12", "onchange='ajxCtrlChanged(this)'" )->getHtml() << endl;
cout << fileEntry( "fileEntry", "20", "60" )->getHtml() << endl;
cout << dateEntry( "dateEntry", "12" )->getHtml() << endl;
cout << timeEntry( "timeEntry", "12" )->getHtml() << endl;
cout << emailEntry( "emailEntry", "42", "42" )->getHtml() << endl;
cout << ssnEntry( "ssnEntry", "12" )->getHtml() << endl;
cout << zipEntry( "zipEntry", "10" )->getHtml() << endl;
cout << phoneEntry( "phoneEntry", "14" )->getHtml() << endl;
cout << longNumberEntry( "longNumberEntry", "10" )->getHtml() << endl;
cout << shortNumberEntry( "shortNumberEntry", "15" )->getHtml() << endl;
cout << pswdEntry( "pswdEntry", "15" )->getHtml() << endl;


cout <<  cmboEntry( "cmboEntry", "select * from user", "1" )->getHtml() << endl;

cout <<  cmboManualEntry( "cmboManualEntry","a=1,b=2,c=3", "4", "multiple" )->getHtml() << endl;

cout <<  checksEntry( "checksEntry","a=1,b=2,c=3", "4" )->getHtml() << endl;
cout <<  radioEntry( "radioEntry","a=1,b=2,c=3", "4"  )->getHtml() << endl;
cout <<  areaEntry( "areaEntry" )->getHtml() << endl;
cout <<  boolBox( "boolBox", "1" )->getHtml() << endl;
#endif
