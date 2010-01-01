/*
  read_write_base.hpp  
  
  base class for data access    
  
  Implementation file
  The classes in this file are used strictly as glue 
  between the business objects and the data objects.
  
                                   ****                                       
  [DB]<->[DB API]<->[DATA-OBJs]<->[Glue]<->[BUS-OBJs]<->[Glue]<->[GUI]
                    (polymorph)    ****
  
  Copyright © 2003 All rights reserved
  David K. McCombs - davidmc@w3sys.com
  W3 Systems Design - www.w3sys.com

*/

// includes
#include "read_write_base.hpp"

// defaults from connectstring include
read_write_base::read_write_base()  
  :DbMaker(PROVIDER,CONNECTSTRING),db(DbMaker.db())
  ,RsMaker(db),rs(RsMaker.rs())
  ,CmdMaker(db),cmd(CmdMaker.cmd()),m_name("")
  ,m_result(""),m_key(0LL),actDerived(false)
{
  ;
}

// override to other data sources... dbProvider,const char *
read_write_base::read_write_base( dbProvider Provider, const char * Connectstring )
  :DbMaker(Provider,Connectstring),db(DbMaker.db())
  ,RsMaker(db),rs(RsMaker.rs())
  ,CmdMaker(db),cmd(CmdMaker.cmd()),m_name("")
  ,m_result(""),m_key(0LL),actDerived(false)
{
  ;
}
read_write_base::~read_write_base()
{
  clearMaps(); 
} 
void read_write_base::clearMaps( void )
{
  for(int i=0; i<xfers.size(); ++i )
  {
    delete xfers[i];
  }
  xfers.clear();
}
bool read_write_base::db_action( string mode, changeMap & changes )
{
  bool breturn = false;
  m_result = "";
  switch( mode[0] )
  {
    case 'u': // update
      breturn = db_update(changes);
      get_data(); // get data anyway! but don't modify return status.
      if( breturn ) usupplemental(changes);       
    break;
    case 'i': // insert
      breturn = db_insert();
      if( breturn ) breturn = get_data();
      if( breturn ) isupplemental();       
    break;
    case 'd':
      breturn = db_delete();
      if( breturn ) dsupplemental(); 
    case 'r': // reset - no data, no key
    case 'n': // new - data, but no key
      key(0);
    break;
    case 's': // show
    default: // same as show 
      breturn = get_data();
      if( breturn ) ssupplemental();        
    break;   
  }
  return breturn;  
}
bool read_write_base::db_insert( void )
{
  bool breturn = false;
  if( key() && !actDerived )
  {
    m_result = "Reset key! ";
    key(0);
  }
  if( ivalidate() )
  {
    string sql = "insert into ";
    string colNames = " ( ", colVals = " values ( ";
    sql += data_name();
        

    // ALWAYS make the key be first, so we can skip it (unless this is a derived class)
    for( int i=actDerived?0:1; 
         i<xfers.size(); 
         ++i )
    {    
      colNames += xfers[i]->name();
      colVals += xfers[i]->toFieldVar();
      if( i<xfers.size()-1 )
      {
        colNames += ", ";
        colVals += ", ";
        
      }
    }
    colNames += " ) ";
    colVals += " ) ";
    sql += colNames;
    sql += colVals;    
    if( cmd.execute(sql) )
    {
      key( cmd.resultKey( xfers[0]->name() , data_name() ) );
      breturn = true;
      m_result = "Insert Successful\n";
      m_result += "<!-- ";
      m_result += sql;
      m_result += " Key Returned: ";
      m_result.append(key());   
      m_result += " -->\n";
    }
    else
    {  
      m_result = "Insert Command Failed: ";
      m_result += "<!-- ";
      m_result += sql;
      m_result += " -->";
      m_result += " - ";
      m_result += cmd.getErrors();
    }  
  }  
  return breturn;
}
bool read_write_base::db_update( changeMap & changes )
{
  bool breturn = false;
  bool foundChange = false;
  if( uvalidate(changes) )
  {
    bool cols=0;
    if( changes.size() )
    {
      ocString sql = "update ";  
      sql += data_name();
      sql += " set ";

      // ALWAYS make the key be first, so we can skip it
      for(int i=actDerived?0:1; 
          i<xfers.size(); 
          ++i )
      {  
        // ONLY modify columns the user has changed.
        if( changes.find(xfers[i]->name()) != changes.end() )
        { 
          if( cols )
          {
            sql += ", ";      
          }  
          sql += xfers[i]->name();
          sql += " = ";
          sql += xfers[i]->toFieldVar();     
          ++cols;
          foundChange = true; 
        }
      }  
      sql += " where ";
      sql += xfers[0]->name();
      sql += " = ";
      sql.append(key());
      if( !foundChange )
      {
        m_result = "No Changes Made";
        breturn = true;
      }
      else if( cmd.execute(sql) )
      {      
        m_result = "Update Successful\n<!-- ";
        m_result += sql;      
        m_result += " -->\n";    
        breturn = true;
      }
      else
      {  
        m_result = "Update Command Failed: ";
        m_result += "<!-- ";
        m_result += sql;
        m_result += " -->";
        m_result += " - ";
        m_result += cmd.getErrors();
      }  
    }
    else
    {
      m_result += " No Changes Made.";
    }
  }
  return breturn;
}
bool read_write_base::db_delete(  void )
{
  bool breturn = false;
  if(dvalidate())
  {
    ocString sql = "delete from ";  
    sql += data_name();
    sql +=  " where ";
    sql += xfers[0]->name(); // the name if the id column MUST be in position 0
    sql += " = ";
    sql.append(key());

    if( cmd.execute(sql) )
    {    
      breturn = true;
      m_result = "Delete Successful\n<!-- ";
      m_result += sql;    
      m_result += " -->\n"; 
    }
    else
    {  
      m_result = "Delete Command Failed: ";
      m_result += sql;
      m_result += " - ";
      m_result += cmd.getErrors();
    }                  
  }  
  return breturn;
}
bool read_write_base::get_data( void )
{
  bool breturn = false;
  if( key() == 0 )
  {
    return breturn;
  }
  int i;
  ocString sql = "select ";
  for(i=0; i<xfers.size(); ++i )
  {
    sql += xfers[i]->name();
    if( i<xfers.size()-1 )
    {
      sql += ", ";
    }
  }
  sql += " from ";
  sql += data_name();
  sql +=  " where ";
  sql += xfers[0]->name(); // the name if the id column MUST be in position 0
  sql += " = ";  
  sql.append(key());
  if( rs.open(sql) )
  {
    breturn=true;
    for(i=0; i<xfers.size(); ++i )
    {      
      xfers[i]->toClassVar( rs.getField(i) );      
    }
  }
  else
  {
     m_result += " Could not get data\n<!-- ";
     m_result += sql;
     m_result += "\n";
     m_result += rs.getErrors();
     m_result += " -->\n";
  }  
  return breturn;
}

// custom get_data based on where clause
bool read_write_base::get_data( string whereClause,  string orderClause  )
{
  bool breturn = false;
  int i;
  ocString sql = "select ";
  for(i=0; i<xfers.size(); ++i )
  {
    sql += xfers[i]->name();
    if( i<xfers.size()-1 )
    {
      sql += ", ";
    }
  }
  sql += " from ";
  sql += data_name();
  if( whereClause.length() )
  {
    sql +=  " where ";
    sql += whereClause;
  }

  if( orderClause.length() )
  {
    sql +=  " order by ";
    sql += orderClause;
  }
  
  if( rs.open(sql) )
  {
    breturn=true;
    
    // the key should be in the first position
    key( atoll(rs.getField(0).format().c_str()) );
    // set the class properties
    for(i=0; i<xfers.size(); ++i )
    {          
      xfers[i]->toClassVar( rs.getField(i) );      
    }
  }
  else
  {
    m_result += " Could not get data <!--";
     m_result += sql;
     m_result += " - ";
     m_result += rs.getErrors();
     m_result += " -->";
  }  
  return breturn;
}

/*
  allow forward iteration
*/
bool read_write_base::next( void )
{
  bool ret = rs.next(); 
  if( ret )
  {
    key( atoll(rs.getField(0).format().c_str()) );
    for(int i=0; i<xfers.size(); ++i )
    {          
      xfers[i]->toClassVar( rs.getField(i) );      
    }
  }
  return ret;  
}

// new for transasction capable dbs
bool read_write_base::commit( void )
{
  return cmd.commit();
}

/* 
  Add an item to the list that controls 
  the mapping of data fields to properties  
*/
bool read_write_base::addDXMap( dataXfer * xfer )
{
  bool breturn = true;
  xfers.push_back(xfer);
  return breturn;
}  

bool read_write_base::ivalidate( void  ) { return true; }
bool read_write_base::uvalidate( changeMap & changes  ) { return true; }
bool read_write_base::dvalidate( void ) { return true; }

bool read_write_base::isupplemental( void ){ return true; }
bool read_write_base::usupplemental( changeMap & changes ){ return true; }
bool read_write_base::dsupplemental( void ){ return true; }
bool read_write_base::ssupplemental( void ){ return true; } 




