/*
  base classes:
  read_write_base
    
  PayDrop 
  Member Check Business Object
*/
#include <uuid/uuid.h>
// need to link libuuid

#ifndef PD_Member_Check
#define PD_Menber_Check

class  MemberCheck :  public read_write_base
{
public:

 identifier MemberCheckID;
 long long MemberID;
 oc_date CheckDate;
 money CheckAmount; 
public:
  MemberCheck():read_write_base(),MemberCheckID(0LL),MemberID(0LL)
  {
    data_name("MemberCheck");
    addDXMap( new llongXfer("MemberCheckID", &MemberCheckID  ) );
    addDXMap( new llongXfer("MemberID", & MemberID ) );
    addDXMap( new oc_dateXfer("CheckDate", &CheckDate  ) );
    addDXMap( new moneyXfer("CheckAmount", &CheckAmount) );
  }
  virtual ~MemberCheck(){;} 
};




#endif

