/*
   global_content.hpp


*/
#ifndef Global_Content_Hpp
#define Global_Content_Hpp
class global_content
{
private:

protected:
  long long m_id;
  long long m_site_id;
  string m_name;
  string m_content;

public:
  global_content & operator = ( const global_content & in )
  {
    id( in.id() );
    site_id( in.site_id() );
    name( in.name() );
    content( in.content() );
    return * this;
  }
  global_content()
  :m_id(0),m_site_id(0)
  {
    ;
  }
  virtual ~global_content()
  {
    ;
  }

  // Property Access Methods
  // Gets
  long long id( void ) const {return m_id;}
  long long site_id( void ) const {return  m_site_id;}
  string    name( void ) const {return  m_name;}
  string    content( void ) const {return  m_content;}

  // Sets
  void id( long long in ){m_id=in;}
  void site_id( long long in ){m_site_id=in;}
  void name( string in ){m_name=in;}
  void content( string in ){m_content=in;}
};

typedef map<string,global_content> global_content_map;

class global_contents : public global_content_map, protected read_base
{
private:
  string getSiteSql( long long site_id )
  {
    // load page from lookup url
    ocString sql = "select id, site_id, name, content "
                   "from metasite.global_content "
                   "where site_id = ";
    sql.append(site_id);    
    return sql;                
  }
  void propset( global_content & gc )
  {
    gc.id(atoll(rs.getField(0).format().c_str()));
    gc.site_id(atoll(rs.getField(1).format().c_str()));    
    gc.name(rs.getField(2).format());
    gc.content(rs.getField(3).format());
  }  
public:
  global_contents():read_base()
  {
    ;
  }
  ~global_contents()
  {
    ;
  } 
  
  bool load( long long site_id )
  {
    bool breturn=false;
        
    // load the template info by id from the db.
    for( bool ok=rs.open(getSiteSql(site_id)); ok; ok=rs.next() )
    {  
      global_content  gc;    
      propset(gc);
      insert( make_pair(gc.name(),gc) );      
    }
    rs.close();
    
    // return result of load
    return breturn;
  }
};
#endif
