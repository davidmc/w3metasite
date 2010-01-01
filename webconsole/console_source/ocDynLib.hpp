/*
  ocDynLib.hpp
  =================================
  class of dynamic library loader
  
*/
extern "C" { 
#include  <dlfcn.h>
}
class ocDynamicLibrary
{
private:
  void * theLib;      // Handle to shared lib file 
  void * theFunction; // Address of the loaded routine 
  string theError;    // Pointer to the error string 
  bool isLibLoaded; 
  bool isFunctionLoaded; 

public:  
  /** 
    Constructor
    
    This is the only constructor you should use.
    
  */
  ocDynamicLibrary( string path )
  :theLib(NULL),theFunction(NULL)
  ,isLibLoaded(false),isFunctionLoaded(false)
  {
    theLib = dlopen(path.c_str(),RTLD_LAZY);
    if( theLib ) isLibLoaded = true;
    else
    {
      theError = dlerror(); 
    }
  }
  
  virtual ~ocDynamicLibrary()
  {
    if( theLib ) dlclose(theLib);
  }
  /*
    Find the symbol and load the function
  */  
  bool loadFunction(string symbolname )
  {
    theFunction    = dlsym( theLib, symbolname.c_str() );
    if( theFunction ) isFunctionLoaded = true;
    else
    {
      isFunctionLoaded = false;
      theError = dlerror(); 
    }
  }
  /*
    Return function address for calling...
     
    cast the return appropriately - example casted calls:
    ((void (*)())cls.function())();
    int rc = ((int (*)(int))cls.function())(22); 
    bool isGood = ((bool (*)(char *))cls.function())("hey baby!");    
  */
  void * function( void )
  {
    return theFunction;
  }
  
  /*
    Check status ...
  */
  bool IsLibraryLoaded( void )
  {
    return isLibLoaded;
  }
  bool IsFunctionLoaded( void )
  {
    return isFunctionLoaded;
  }  
  string LastError( void )
  {
    return theError;
  }
  
};   
