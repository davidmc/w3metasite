/*
  flexButtons.hpp

  More buttons - When am i going to get this right!

  Copyright (C) 2007 by David K. McCombs
  W3 Systems Design
  davidmc@w3sys.com

*/
#ifndef FlexButtons_Hpp
#define FlexButtons_Hpp

string flexButtons( long long key,
                    string iLabel="&nbsp;Insert&nbsp;",
                    string uLabel="&nbsp;Update&nbsp;", 
                    string dLabel="&nbsp;Delete&nbsp;",
                    string rLabel="&nbsp;New - Reset&nbsp;",
                    string nLabel="&nbsp;New - Keep Data&nbsp;",
                    bool doInsert = true,
                    bool doUpdate = true,
                    bool doDelete = true,
                    bool doReset = true,
                    bool doKeepData = true )
{
  string ret;
  if( key )       
  { // Appropriate for update/delete
    if( doUpdate )
    { 
      ret += "<input class='submitButtons' type='button' onclick='doUpdate()' name='action' class='action' value='";
      ret += uLabel;
      ret += "'>";
    }
    if( doDelete )
    {
      ret += "<input class='submitButtons' type='button' onclick='doDelete()' name='action' class='action' value='";
      ret += dLabel;
      ret += "'>";
    }
    if( doReset )
    {
      ret += "<inputclass='submitButtons'  type='button' onclick='doReset()' name='action' class='action' value='";
      ret += rLabel;
      ret += "'>";
    }
    if( doKeepData )
    {
      ret += "<input type='button' onclick='doNew()' name='action' class='action' value='";
      ret += nLabel;
      ret += "'>";
    }
  }
  else
  { // Appropriate for insert
    if( doInsert )
    {
      ret += "<input type='button' onclick='doInsert()' name='action' class='action' value='";
      ret += iLabel;
      ret += "'>";
    }
    else
    { // hide the fact your inserting, make it look like an update, you will never get here unless this is the intent
      ret += "<input type='button' onclick='doInsert()' name='action' class='action' value='";
      ret += uLabel;
      ret += "'>";
   }
  }
  return ret;
}

#endif
