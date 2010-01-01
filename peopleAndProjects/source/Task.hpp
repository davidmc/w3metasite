/* 
  Task.hpp

  Object Definition and Implementation 
  for Task.
  
  Copyright (C) 2006 by D.K. McCombs.
  W3 Systems Design  

*/  
#ifndef Task_HPP
#define Task_HPP

#include "read_write_base.hpp"
#include "Status_Enumerations.h"

class Task_Obj: public read_write_base
{
public:
  identifier Id;
  llong Project;
  string Name;
  double Estimated_Time;
  double Actual_Time;
  string Description;
  time_date Start;
  long Task_Order;
  long Hierarchy;
  llong Status;
  long Percent_Complete;
  llong Parent_Task;
  money Estimate;
  llong Requestor;
  oc_date Requested;
  oc_date Estimated_End;
  oc_date Actual_End;
  
  Task_Obj():read_write_base()
  ,Id(0LL)
  ,Project(0LL)
  ,Name("")
  ,Estimated_Time(0.0)
  ,Actual_Time(0.0)
  ,Description("")
  ,Start( )
  ,Task_Order(1000L)
  ,Hierarchy(0L)
  ,Status(0LL)
  ,Percent_Complete(0L)
  ,Parent_Task(0LL)
  ,Estimate(0.0)
  ,Requestor(0LL)
  ,Requested( )
  ,Estimated_End( )
  ,Actual_End( )
  {
    // set name
    data_name("Task");

    // Defaults: add 2000 years to the end times
    Estimated_End.addYears(2000);
    Actual_End.addYears(2000);
    
    // add fields
    addDXMap( new llongXfer("Id", &Id ));
    addDXMap( new llongXfer("Project", &Project ));
    addDXMap( new stringXfer("Name", &Name ));
    addDXMap( new doubleXfer("Estimated_Time", &Estimated_Time ));
    addDXMap( new doubleXfer("Actual_Time", &Actual_Time ));
    addDXMap( new stringXfer("Description", &Description ));
    addDXMap( new time_dateXfer("Start", &Start ));
    addDXMap( new longXfer("Task_Order", &Task_Order ));
    addDXMap( new longXfer("Hierarchy", &Hierarchy ));
    addDXMap( new llongXfer("Status", &Status ));
    addDXMap( new longXfer("Percent_Complete", &Percent_Complete ));
    addDXMap( new llongXfer("Parent_Task", &Parent_Task ));
    addDXMap( new moneyXfer("Estimate", &Estimate ));
    addDXMap( new llongXfer("Requestor", &Requestor ));
    addDXMap( new oc_dateXfer("Requested", &Requested ));
    addDXMap( new oc_dateXfer("Estimated_End", &Estimated_End ));
    addDXMap( new oc_dateXfer("Actual_End", &Actual_End )); 
  }
  // MANY TO MANY FIELDS
  ocString Task_Assignment;
  virtual bool setTask_Assignment( void )
  {
    bool bRet = false;
    if( Task_Assignment.length() )
    {
      ocString sql;
      ocString sqlStart = "insert into Task_Assignment ("
          "Task,"
          "User"
          ") values (";
      sqlStart.append(key());
      sqlStart += ",";
      Task_Assignment.parseInit();
      while( !Task_Assignment.endOfParse() )
      {
        sql = sqlStart;
        sql += Task_Assignment.parse("|");
        sql += ")";
        bRet = cmd.execute(sql);
        if(!bRet) break;
      }
    }
    
    return bRet;
  }
  // on Event of form submission, delete
  virtual bool deleteTask_Assignment( void )
  {
    bool bRet = false;
    ocString sql = "delete from Task_Assignment where Task = ";
    sql.append(key());
    bRet = cmd.execute(sql);
    return bRet;
  }
  // on event of getting data
  virtual bool retrieveTask_Assignment( void )
  {
    bool bRet = false;
    Task_Assignment = "";
    ocString sql = "select User from Task_Assignment where Task = ";
    sql.append(key());
    if( rs.open(sql) )
    {
      bRet = true;
      bool moreData = true;
      do
      {
        Task_Assignment += rs.getField(0).format();
        moreData = rs.next();
        if( moreData ) Task_Assignment += "|";
      }
      while( moreData );
    }
    return bRet;
  }
  bool isupplemental( void )
  {
    return setTask_Assignment();
  }

   // event fired after update
  bool usupplemental( changeMap & changes )
  {
    bool bRet = deleteTask_Assignment();
    if(bRet)  bRet = setTask_Assignment();
    return bRet;
  }
  
  // event fired after delete
  bool dsupplemental( void )
  {
    bool bRet = deleteTask_Assignment();
    return bRet;
  }
  // event fired after gettring data
  bool ssupplemental( void )
  {
    return retrieveTask_Assignment();
  }
 

  double sumTimes ( void )
  {
    double sumOfTime = 0;
    // Get the sum of all the times
    ocString sql = "select sum( Hours ) from Time where Task = ";
    sql.append(Id);
    if( rs.open(sql) )
    {
      sumOfTime = atof( rs.getField(0).format().c_str() );
    }
    // round to quarter hour
    sumOfTime *= 4;
    sumOfTime = round(sumOfTime);
    sumOfTime /= 4;
    return  sumOfTime;
  }
  virtual bool uvalidate( changeMap & changes  )
  {
    // Save the current estimate
    double Hours = Actual_Time;
    Actual_Time = sumTimes();
    if( Hours != Actual_Time)
    {
      changes["Actual_Time"]="Actual_Time";
    }
    if(  Percent_Complete == 100  )
    {
      if( l_Status_Enum(Status) < Task_Complete )
      {
        Status = ll_Status_Id( Task_Complete );
        changes["Status"]="Status";
      }
      // Get it off the front of the list
      Task_Order += 1000000;
      changes["Task_Order"]="Task_Order";
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



