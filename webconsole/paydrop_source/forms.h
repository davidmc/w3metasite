#ifndef OPEN_CORE_FORMS_H
#define OPEN_CORE_FORMS_H 123456
#include "ocControls.h"
#include "read_base.hpp"

string formButtons(void)
{
  string ret;
  ret += "<input type='button' onclick='doInsert()' name='action' value='&nbsp;Insert&nbsp;'>";
  ret += "&nbsp;&nbsp;&nbsp;";
  ret += "<input type='button' onclick='doUpdate()' name='action' value='&nbsp;Update&nbsp;'>";
  ret += "&nbsp;&nbsp;&nbsp;";
  ret += "<input type='button' onclick='doDelete()' name='action' value='&nbsp;Delete&nbsp;'>";
  return ret;
}
string formButtons( long long key )
{
  string ret;
  if( key )       
  { // Appropriate for update/delete
    ret += "<input type='button' onclick='doUpdate()' name='action' value='&nbsp;Update&nbsp;'>";
    ret += "&nbsp;&nbsp;&nbsp;";
    ret += "<input type='button' onclick='doDelete()' name='action' value='&nbsp;Delete&nbsp;'>";
    ret += "&nbsp;&nbsp;&nbsp;\n";
    ret += "<input type='button' onclick='doReset()' name='action' value='&nbsp;New - Reset Form&nbsp;'>";
    ret += "&nbsp;&nbsp;&nbsp;";
    ret += "<input type='button' onclick='doNew()' name='action' value='&nbsp;New - Keep Data&nbsp;'>";    
  }
  else
  { // Appropriate for insert
    ret += "<input type='button' onclick='doInsert()' name='action' value='&nbsp;Insert&nbsp;'>";
  }
  return ret;
}

/*
  textEntry & checkEntry
*/
ocTextbox * textEntry( string name, string size, string length = "" )
{
  string attr = "onChange='ctrlChanged(this)' id='";
  attr += name;
  attr += "'";
  string len = length.length()?length:size;
  ocTextbox * box = new ocTextbox( name, len, size, attr );
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
  attr += "'";
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
  if( sql.length() )
  {
    quickQuery qq;
    openRS & rs = qq.getData(sql);
    for( bool result=qq.opened; result; result=rs.next() )
    {
      combo->addOption( rs.getField(1).format(),
                        rs.getField(0).format() );
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
      
      combo->addOption( pair.parse("="),
                        pair.remainder() );
    }
  }
  return combo;
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
  string attr = "onChange='ctrlChanged(this)' id='";
  attr += id;
  attr += "'";
  ocBoolbox * bbox = new ocBoolbox( name , attr, "1", "1" );
  return bbox;
}

#endif

