/*
  diaButtons.h
  I forget what I meant by the prefix dia.
  This file is here to make form button groups more flexible

  David Mc. 2006

*/

string diaButtons( long long key,
                   string iLabel="&nbsp;Insert&nbsp;",
                   string uLabel="&nbsp;Update&nbsp;",
                   string dLabel="&nbsp;Delete&nbsp;",
                   string rLabel="&nbsp;Reset New&nbsp;",
                   string kLabel="&nbsp;Keep New&nbsp;"
                 )
{
  string ret;
  if( key )       
  { // Appropriate for update/delete
    if(uLabel.length())
    { 
      ret += "<input type='button' onclick='doUpdate()' name='action' class='action' value='";
      ret += uLabel;
      ret += "'>&nbsp;&nbsp;&nbsp;";
    }
    if( dLabel.length() )
    {
      ret += "<input type='button' onclick='doDelete()' name='action' class='action' value='";
      ret += dLabel;
      ret += "'>&nbsp;&nbsp;&nbsp;\n";
    }
    if( rLabel.length() )
    {
      ret += "<input type='button' onclick='doReset()' name='action' class='action' value='";
      ret += rLabel;
      ret += "'>&nbsp;&nbsp;&nbsp;";
    }
    if( kLabel.length() )
    {
      ret += "<input type='button' onclick='doNew()' name='action' class='action' value='";
      ret += kLabel;
      ret += "'>";
    }       
  }
  else
  { // Appropriate for insert
    if( iLabel.length() )
    {
      ret += "<input type='button' onclick='doInsert()' name='action' class='action' value='";
      ret += iLabel;
      ret += "'>";
    }
    else if( uLabel.length() )
    {
      // Hide the fact you inserting, make it look like an update, you will never get here unless this is the intent 'KISS'
      ret += "<input type='button' onclick='doInsert()' name='action' class='action' value='";
      ret += uLabel;
      ret += "'>";
    }
  }
  return ret;
}
