/**
  audit_base.hpp
  base class for auditung data changes
  Definition file        
  The classes in this file are used strictly as glue 
  between the business objects and the data objects.  
                                   ****                                       
  [DB]<->[DB API]<->[DATA-OBJs]<->[Glue]<->[BUS-OBJs]<->[Glue]<->[GUI]
                    (polymorph)    ****

  The idea is to provide a differential audit of key data.
  Differential audits only record differences, ie, it asks 'what was the data before the change?'

  Key data is specified by calling the auditField(name) method.

  Copyright © 2006 All rights reserved
  David K. McCombs - davidmc@w3sys.com
  W3 Systems Design - www.w3sys.com  
*/

#ifndef Audit_Base_Hpp
#define Audit_Base_Hpp

// includes
#include "read_write_base.hpp"
#include "Deletion.hpp"
#include "FieldChange.hpp"
#include "AuditTable.hpp"
#include <set>

using namespace std;

// forward declaration
class ref_base;

/// for determining what data items to audit.
typedef set<string> auditSet;

/// for individual db instances
class audit_base : public read_write_base
{
protected:

  auditSet audit_fields;
  llong user_id;  // who did it
  string db_name;
  
  enum {
    audit_nil=0,
    audit_insert=1,
    audit_update=2,
    audit_delete=3
  } audit_type;
  
  bool getOldValues( changeMap & changes )
  {
    bool btrue = true;
    ocString select = "select ";
    auditSet::iterator pos;
    for( pos=audit_fields.begin(); pos != audit_fields.end();  )
    {
      select += *pos;
      ++pos;
      if( pos != audit_fields.end() )
        select += ", ";
    }
    select += " from ";
    select += data_name();
    select += " where Id = ";
    select.append(key());
    if( rs.open(select) )
    {
      for( pos=audit_fields.begin(); pos != audit_fields.end(); ++pos )
      {
        string name = *pos;
        string value = rs.getField(name.c_str()).format();
        // put them in the changes array,
        if(  audit_type == audit_update )
        {
          // if match (change case)
          if( changes.find(name)!=changes.end() || audit_type == audit_delete )
          {
            changes[name] = value;
          }
        }
        else if( audit_type == audit_delete )
        {
          // just add (deleting case)
          changes[name] = value;
        }
      }
      rs.close();
    }
    return btrue;
  }
  bool changeAudits( changeMap & changes, AuditTable_Obj & audt)
  {
    changeMap::iterator pos;
    for( pos=changes.begin(); pos != changes.end(); ++pos )
    {
      auditSet::iterator spos = audit_fields.find( pos->first );
      if( spos != audit_fields.end() )
      {
        FieldChange_Obj fcng;
        fcng.auditTable = audt.key();
        fcng.Name = pos->first;
        fcng.Value = pos->second;
        fcng.db_insert();
      }
    }
    return true;
  }
  bool deleteAudits( changeMap & changes, AuditTable_Obj & audt )
  {
    auditSet::iterator pos;
    // DBG cout << "<div>auditing deletes</div>" << endl;
    // first get the values
    get_data();
    for( pos=audit_fields.begin(); pos != audit_fields.end(); ++pos )
    {
      Deletion_Obj fdel;
      fdel.auditTable = audt.key();
      fdel.Name = *pos;
      // DBG cout << "<div>Add: " << fdel.Name << "</div>" << endl;
      fdel.Value = rs.getField((*pos).c_str()).format();
      fdel.db_insert();
    }
    return true;
  }
  bool performAudit( changeMap & changes )
  {
    if( !( audit_type==audit_update && changes.size()==0 ) )
    {
      AuditTable_Obj audt;
      audt.Actor = user_id;
      audt.aDatabase = db_name;
      audt.aTable = data_name();
      audt.auditType = static_cast<int>( audit_type );
      audt.aRow = key();
      audt.db_insert();
      switch( audit_type )
      {
      case audit_update:
        getOldValues( changes );
        changeAudits( changes, audt );
        break;
      case audit_delete:
        getOldValues( changes );
        deleteAudits( changes, audt );
        break;
      default:
        break;
      }
    }
    return true;
  }
  
public:
  audit_base():read_write_base(),user_id(0),audit_type(audit_nil){;}
  virtual ~audit_base(){;}

  bool db_insert( void )
  {
    bool bRet = false;
    changeMap changes;
    audit_type = audit_insert;
    bRet = read_write_base::db_insert();
    performAudit( changes ); // just add a table entry for insert, but after insert so we have the key
    return bRet;
  }
  bool db_update( changeMap & changes )
  {
    audit_type = audit_update;
    performAudit( changes ); // add a table entry and changed field values before change
    return read_write_base::db_update(changes);
  }
  bool db_delete(  void )
  {
    changeMap changes;
    audit_type = audit_delete;
    performAudit( changes ); // add a table entry and deleted field values before delete
    return read_write_base::db_delete();
  }

  // user sets to determine scope of the audit
  audit_base & setUser( llong id )
  {
    user_id = id;
    return *this;
  }
  // If you have many databases represented in your audit db, this should be set
  audit_base & setDbName( string name )
  {
    db_name = name;
    return *this;
  }
  // to put a field in a table under audit.
  audit_base & auditField( string name )
  {
    audit_fields.insert(name);
    return *this;
  }
  // To audit all the fields in the recordset
  audit_base & auditAll( void )
  {
    int count = xfers.size();
    for( int ifc = 1; ifc<count; ++ifc )  // don't audit the id
    {
      //DBG  cout << "<div>audit: " <<  xfers[ifc]->name() << "</div>" << endl;
      audit_fields.insert(xfers[ifc]->name());
    }
    return *this;
  }
};

#endif
