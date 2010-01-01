/*
  formsExt.h
  
  Extra alternative controls
  
*/

#include "ocControls.h"
#include "read_base.hpp"

// This function Always gets All data
ocCheckBoxes * ckboxesEntry( string name, string sql,
                             string defaulted = "",
                             string attr = "" )
{  
  // instance of the combo box
  string attributes = attr;
  
  ocCheckBoxes * checks = new ocCheckBoxes(name,attributes);
  
  // fill the combo box
  if(  defaulted.length() > 0 )
  {
    checks->addOption( defaulted, "-1" );
  }
  if( sql.length() )
  {
    quickQuery qq;
    openRS & rs = qq.getData(sql);    
    for( bool result=qq.opened; result; result=rs.next() )
    {
      attributes = " name='";
      attributes += name;
      attributes += "' onChange='ctrlChanged(this)' ";

      checks->addOption( rs.getField(1).format(),
                         rs.getField(0).format(), 
                         attributes );
    }
  }
  return checks;
}

// This function Always gets All data
ocRadio * radioEntry( string name, string sql,
                             string defaulted = "",
                             string attr = "" )
{
  // instance of the combo box
  string attributes = attr;
  
  ocRadio * radios = new ocRadio(name,attributes);
  
  // fill the combo box
  if(  defaulted.length() > 0 )
  {
    radios->addOption( defaulted, "-1" );
  }
  if( sql.length() )
  {
    quickQuery qq;
    openRS & rs = qq.getData(sql);
    for( bool result=qq.opened; result; result=rs.next() )
    {
      attributes = " name='";
      attributes += name;
      attributes += "' onChange='ctrlChanged(this)' ";

      radios->addOption( rs.getField(1).format(),
                         rs.getField(0).format(),
                         attributes );
    }
  }
  return radios;
}

