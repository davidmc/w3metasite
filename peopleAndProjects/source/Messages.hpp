/*
  Messages.hpp

  Object Definition and Implementation 
  for Messages.
  
  Copyright (C) 2006 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef Messages_HPP
#define Messages_HPP

#include "read_write_base.hpp"

class Messages_Obj: public read_write_base
{
public:
  identifier Id;
  
  string Subject;
  
  llong From_Id;
  llong To_Id;
  
  string From_String; // for external email
  string To_String;   // for external email
  
  int Importance;
  string Content;

  llong Company; // the message body
  llong Project;
  llong Task;
  
  llong Author;
  time_date Created;
  
  Messages_Obj():read_write_base()
  ,Id(0LL)
  ,Subject("")
  ,From_Id(0LL)
  ,To_Id(0LL)
  ,Importance(0)
  ,Content("")
  ,From_String("")
  ,To_String("")
  ,Company(0LL)
  ,Project(0LL)
  ,Task(0LL)
  ,Author(0LL)
  ,Created( )
  {
    // set name
    data_name("Messages");
    // add fields
    addDXMap( new llongXfer("Id", &Id ));
    addDXMap( new stringXfer("Subject", &Subject ));
    addDXMap( new llongXfer("From_Id", &From_Id ));
    addDXMap( new llongXfer("To_Id", &To_Id ));
    addDXMap( new intXfer("Importance", &Importance ));
    addDXMap( new stringXfer("Content", &Content ));
    addDXMap( new stringXfer("From_String", &From_String ));
    addDXMap( new stringXfer("To_String", &To_String ));
    addDXMap( new llongXfer("Company", &Company ));
    addDXMap( new llongXfer("Project", &Project ));
    addDXMap( new llongXfer("Task", &Task ));
    addDXMap( new llongXfer("Author", &Author ));
    addDXMap( new time_dateXfer("Created", &Created ));
  }
  virtual bool ivalidate( void  )
  {
    Author = From_Id;
    if( To_Id < 1 )
    {
      m_result = "Must specify a recipient!";
      return false;
    }
    if( Subject.length() == 0 )
    {
      m_result = "Must include the Subject line!";
      return false;
    }
    return true;
  }
  virtual bool uvalidate( changeMap & changes  )
  {
    if( !Author && changes.find("Author") == changes.end() )
    {
       Author = From_Id;
       changes["Author"] = "Author";
    }
    if( To_Id < 1 )
    {
      m_result = "Must specify a recipient!";
      return false;
    }
    if( Subject.length() == 0 )
    {
      m_result = "Must include the Subject line!";
      return false;
    }
    return true;
  }
  /*
  TODO: Add Event hooks for business logic
  =========================
  // Done before any action - a false return prevents action
  virtual bool ivalidate( void  ) { return true; }
  virtual bool uvalidate( changeMap & changes  ) { return true; }
  virtual bool dvalidate( void ) { return true; }

  // Done after any action - a false return sets a warning
  virtual bool isupplemental( void ){ return true; }
  virtual bool usupplemental( changeMap & changes ){ return true; }
  virtual bool dsupplemental( void ){ return true; }
  virtual bool ssupplemental( void ){ return true; } 
  */
};
#endif



