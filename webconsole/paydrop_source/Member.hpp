/*
  Member.hpp
  
  Business abstraction of member/patron of paydrop.
  
  base classes:  
  read_write_base
  PayDrop Mamber Business Object 
*/
 
#include <uuid/uuid.h>
// need to link libuuid


#ifndef PD_Member
#define PD_Menber

class  Member :  public read_write_base
{
public:
  identifier MemberID;
  string UniversalID;
  string MemberFirstName;
  string MemberLastName;
  string MemberAddress1;
  string MemberAddress2;
  string MemberCity;
  string MemberState;
  string MemberProvince;
  string MemberPostalCode;
  string MemberCountry;
  string MemberEmail;
  string MemberPassword;
  time_date MemberSince;
  time_date MemberLastLogin; 
  bool MemberWantsNewsletter; 
  bool MemberIsOrganization;
  string MemberOrganizationSiteURL;
  string MemberContact;
  string MemberPhone;
  string MemberMobile;

public:
  Member(): read_write_base(), MemberID(0LL), 
  MemberCountry("USA"),MemberWantsNewsletter(true),MemberIsOrganization(false)
  {
    makeUUID();   
    data_name("Member");
    // ALWAYS make the key be first, so we can skip it in insert logic
    addDXMap( new llongXfer("MemberID", &MemberID) );
    addDXMap( new stringXfer("UniversalID", &UniversalID) );
    addDXMap( new stringXfer("MemberFirstName", &MemberFirstName) );
    addDXMap( new stringXfer("MemberLastName", &MemberLastName) );
    addDXMap( new stringXfer("MemberAddress1", &MemberAddress1) );
    addDXMap( new stringXfer("MemberAddress2", &MemberAddress2) );
    addDXMap( new stringXfer("MemberCity", &MemberCity) );
    addDXMap( new stringXfer("MemberState", &MemberState) );
    addDXMap( new stringXfer("MemberProvince", &MemberProvince) );
    addDXMap( new stringXfer("MemberPostalCode", &MemberPostalCode) );
    addDXMap( new stringXfer("MemberCountry", &MemberCountry) );
    addDXMap( new stringXfer("MemberEmail", &MemberEmail) );
    addDXMap( new stringXfer("MemberPassword", &MemberPassword) );
    addDXMap( new time_dateXfer("MemberSince", &MemberSince) );
    addDXMap( new time_dateXfer("MemberLastLogin", &MemberLastLogin) );
    addDXMap( new boolXfer("MemberWantsNewsletter", &MemberWantsNewsletter) );
    addDXMap( new boolXfer("MemberIsOrganization", &MemberIsOrganization) );
    addDXMap( new stringXfer("MemberOrganizationSiteURL", &MemberOrganizationSiteURL) );
    addDXMap( new stringXfer("MemberContact", &MemberContact) );
    addDXMap( new stringXfer("MemberPhone", &MemberPhone) );
    addDXMap( new stringXfer("MemberMobile", &MemberMobile) );                                                                                          
  }
  
  void makeUUID(void)
  {
    uuid_t uuid;
    char uuid_val[37];
    memset(uuid,'\0',sizeof(uuid));
    uuid_generate(uuid);
    uuid_unparse(uuid, uuid_val);    
    ocString uuid_work(uuid_val);    
    UniversalID = uuid_work.replaceAll("-","");    
  }
  bool ivalidate( void  ) 
  {
    string qry = "select count(*) from Member ";
#ifdef IS_FUNDRAISER
    qry += "where MemberOrganizationSiteURL = '";
    qry += MemberOrganizationSiteURL;
#else
    qry += "where UniversalID = '";
    qry += UniversalID;
    qry += "' or MemberEmail = '";
    qry += MemberEmail;
#endif    
    qry += "'";

    if( rs.open(qry) )
    {
      long count = atol(rs.getField(0).format().c_str());
      rs.close();
      if( count > 0 )
      {
        m_result = "The data element already exists";
        return false;
      }       
    }
    else 
    {
      m_result = "could not verify due to data retrieval error - ";
      m_result = qry;
      return false;
    }     
    return true; 
  }
  virtual ~Member()
  {  
  }   
};

#endif
