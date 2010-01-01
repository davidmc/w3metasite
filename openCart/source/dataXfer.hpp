/*
  dataXfer.hpp

  The classes in this file are used strictly as glue 
  between the business objects and the data objects.
  
                                   ****                                       
  [DB]<->[DB API]<->[DATA-OBJs]<->[Glue]<->[BUS-OBJs]<->[Glue]<->[GUI]
                    (polymorph)    ****
  
  Copyright © 2003 All rights reserved
  David K. McCombs - davidmc@w3sys.com
  W3 Systems Design - www.w3sys.com

*/

// this is for key fields.
typedef long long identifier;

// easy to type long long
typedef long long llong;
/*
  class dataXfer
  Base class for transfering data between basicField(s) and class data    
*/
class dataXfer
{
protected:  
  string m_name;
  void * ptrVar;
public:  
  dataXfer(string in, void * ptrIn):m_name(in),ptrVar(ptrIn){;}  
  virtual ~dataXfer(){;}
  virtual void toClassVar( basicField & rFld ) = 0;
  virtual string toFieldVar( void ) = 0; 
  const string & name( void ) const
  { 
    return m_name;
  }
  void * getPtr(void) const
  {
    return ptrVar;  
  }
};

class longXfer : public dataXfer
{

public:
  longXfer(string  in, void * ptrIn):dataXfer(in,ptrIn){;}  
  virtual ~longXfer(){;}
  void toClassVar( basicField & rFld  )
  {
    long & val = *static_cast<long*>(ptrVar);    
    longField & rlF = dynamic_cast<longField&>(rFld);
    val = rlF.get();  
  }
  string toFieldVar( void )
  {
    ocString ret;
    long & val = *static_cast<long*>(ptrVar); 
    ret.append(val);
    return ret;
  }
};

class llongXfer : public dataXfer
{

public:
  llongXfer(string  in, void * ptrIn):dataXfer(in,ptrIn){;}  
  virtual ~llongXfer(){;}
  void toClassVar( basicField & rFld  )
  {
    llong & val = *static_cast<llong*>(ptrVar);    
    longlongField & rllF = dynamic_cast<longlongField&>(rFld);
    val = rllF.get();  
  }
  string toFieldVar( void )
  {
    ocString ret;
    llong & val = *static_cast<llong*>(ptrVar); 
    ret.append(val);
    return ret;
  }
};

class boolXfer : public dataXfer
{
public:
  boolXfer(string  in, void * ptrIn):dataXfer(in,ptrIn){;}  
  virtual ~boolXfer(){;}
  void toClassVar( basicField & rFld  )
  {
    bool & val = *static_cast<bool*>(ptrVar);
    switch( rFld.getType() )
    {
      case shortType:
      case longType:
      case longlongType:
        val = atol( rFld.format().c_str() ) != 0L;
      break;
      case stringType:
      {
        char chVal = rFld.format()[0];
        val =  ::toupper(chVal) == 'T' || 
               ::toupper(chVal) == 'Y' ||
               chVal == '1'; 
      }             
      break;
        
      case boolType:  
        val = dynamic_cast<boolField &>(rFld).get();
      break;
      default:
        val=false;
      break;   
    } 
  }
  string toFieldVar( void )
  {
    ocString ret("'");
    bool & val = *static_cast<bool*>(ptrVar); 
    ret.append(val?1:0);
    ret += "'";
    return ret;
  }
};

class moneyXfer : public dataXfer
{
public:
  moneyXfer(string  in, void * ptrIn):dataXfer(in,ptrIn){;}  
  virtual ~moneyXfer(){;}
  void toClassVar( basicField & rFld  )
  {
    money & val = *static_cast<money*>(ptrVar);    
    // Should be one of two types
    switch( rFld.getType())
    {
      case doubleType:
        {          
          doubleField & rdF = dynamic_cast<doubleField&>(rFld);          
          val = rdF.get();          
        }
        break;
      case currencyType:
        {          
          currencyField & rcF = dynamic_cast<currencyField&>(rFld);
          val = rcF.get().amount;
        }
        break;
      default:
        break;       
    }  
  }
  string toFieldVar( void )
  {
    ocString ret;
    money & val = *static_cast<money*>(ptrVar); 
    ret.append(val.whole());
    ret+=".";
    ret.append(val.fractional());
    return ret;
  }
};

class stringXfer : public dataXfer
{
public:
  stringXfer(string in, void * ptrIn):dataXfer(in,ptrIn){;}
  virtual ~stringXfer(){;}
  void toClassVar( basicField & rFld )
  {
    string & val = *static_cast<string*>(ptrVar);    
    stringField & rstringF = dynamic_cast<stringField&>(rFld);
    val = rstringF.get();
  }
  string toFieldVar( void  )
  {
    string ret = "'";        
    string & val = *static_cast<string*>(ptrVar); 
    ocString fixup = val;
    ret += fixup.replaceAll("'","''");
    ret += "'";
    return ret;
  }
};

class doubleXfer : public dataXfer
{
public:
  doubleXfer(string  in, void * ptrIn):dataXfer(in,ptrIn){;}  
  virtual ~doubleXfer(){;}
  void toClassVar( basicField & rFld  )
  {
    double & val = *static_cast<double*>(ptrVar); 
    switch( rFld.getType())
    {
      case doubleType:
        {          
          doubleField & rdF = dynamic_cast<doubleField&>(rFld);          
          val = rdF.get();          
        }
        break;
      case floatType:
        {          
          floatField & rcF = dynamic_cast<floatField&>(rFld);
          val = rcF.get();
        }
        break;
      default:
        break;       
    }                     
  }
  string toFieldVar( void )
  {
    ocString ret;
    double & val = *static_cast<double*>(ptrVar); 
    ret.append(val);
    return ret;
  }
};
class time_dateXfer : public dataXfer
{
public:
  time_dateXfer(string in, void * ptrIn):dataXfer(in,ptrIn){;}
  virtual ~time_dateXfer(){;}
  void toClassVar( basicField & rFld )
  {
    time_date & val = *static_cast<time_date*>(ptrVar);    
    dateTimeField & rdtF = dynamic_cast<dateTimeField&>(rFld);
    val = rdtF.get();
  }
  string toFieldVar( void  )
  {
    string ret = "'";        
    time_date & val = *static_cast<time_date*>(ptrVar); 
    ret += val.format(iDateTimeMyFMT);
    ret += "'";
    return ret;
  }
};
typedef vector<dataXfer*> dataXfers;

