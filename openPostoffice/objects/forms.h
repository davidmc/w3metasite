#ifndef OPEN_CONNECTIONS_FORMS_H
#define OPEN_CONNECTIONS_FORMS_H 123456

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
string formButtons(cgiInput & args)
{
  string ret;
  if( args.count("id") && args["id"].length()  ) // Appropriate for update/delete
  {
    ret += "<input type='button' onclick='doUpdate()' name='action' value='&nbsp;Update&nbsp;'>";
    ret += "&nbsp;&nbsp;&nbsp;";
    ret += "<input type='button' onclick='doDelete()' name='action' value='&nbsp;Delete&nbsp;'>";
  }
  else
  {
    ret += "<input type='button' onclick='doInsert()' name='action' value='&nbsp;Insert&nbsp;'>";
  }
  return ret;
}

/*
  textEntry & checkEntry
*/
ocBase * textEntry( string name, string size, string length = "" )
{
  string len = length.length()?length:size;
  ocTextbox * box = new ocTextbox( name, len, size, "onChange='ctrlChanged(this)'" );
  return box;
}
ocBase * staticEntry( string name, string size, string length = "" )
{
  string len = length.length()?length:size;
  ocTextbox * box = new ocTextbox( name, len, size, "readonly class='static'" );
  return box;
}
ocBase * hiddenEntry( string name )
{
  ocHidden * box = new ocHidden( name );
  return box;
}
ocBase * dateEntry( string name, string size )
{
  ocTextbox * box = new ocTextbox( name, size, size, "onChange='validDate(this,true)'" );
  return box;
}
ocBase * emailEntry( string name, string size, string length = ""  )
{
  string len = length.length()?length:size; // max length
  ocTextbox * box = new ocTextbox( name, len, size, "onChange='isEmail(this,true)'" );
  return box;
}
ocBase * ssnEntry( string name, string size )
{
  ocTextbox * box = new ocTextbox( name, size, size, "onChange='validSSN(this,true)'" );
  return box;
}
ocBase * zipEntry( string name, string size )
{
  ocTextbox * box = new ocTextbox( name, size, size, "onChange='isZip(this,true)'" );
  return box;
}
ocBase * pswdEntry( string name, string size )
{
  ocPassword * box = new ocPassword( name, size, size, "onChange='ctrlChanged(this)'" );
  return box;
}

// This function Always gets All data
ocBase * cmboEntry( string name, string sql, string defaulted = "" )
{
  postgresDB db(CONNECTSTRING);
  postgresRS rs(db);
  // instance of the combo box
  string attributes = " name='";
  attributes += name;
  attributes += "'";
  attributes += " onChange='ctrlChanged(this)'";
  ocSelect * combo = new ocSelect(attributes);

  // fill the combo box
  if(  defaulted.length() > 0 )
  {
    combo->addOption( defaulted, "-1" );
  }
  for( bool result=rs.open( sql ); result; result=rs.next() )
  {
    combo->addOption( rs.getField(1).format(),
                      rs.getField(0).format() );
  }
  return combo;
}

ocBase * areaEntry( string name )
{
  string attr = " name='";
  attr += name;
  attr += "', rows='8' cols='50'";
  attr += " onChange='ctrlChanged(this)'";
  ocTextarea * ta = new ocTextarea(attr);
  return ta;
}

ocBase * boolBox( string name )
{
  ocBoolbox * bbox = new ocBoolbox( name , "onChange='ctrlChanged(this)'" );
  return bbox;
}

#endif
