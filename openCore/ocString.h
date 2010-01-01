#ifndef OC_STRING_H
#define OC_STRING_H

#include <string>
#include <algorithm>
#include <sstream>
#include <regex.h>

using namespace std;
/**
 External helper type conversion function template

*/
template<class T> string & ocAppend( string & inStr, T inVar )
{

  stringstream s;
  s.precision(12);
  s << inVar;
  inStr += s.str();  
  return inStr;
}

/**
  ocString
  new open string class
  ---------------------
  The beginning of a replacement for my legacy aString class
  Uses std::string functionality.

*/
class ocString: public string
{
private:
  string retVal;
  string::size_type idx;
  bool eop;
  int  regex_status;
public:

  ocString():string(),retVal(""),idx(0),eop(false),regex_status(0){;}
  ocString( const char * in ):string(in),retVal(""),idx(0),eop(false),regex_status(0){;}
  ocString( const string & in ):string(in),retVal(""),idx(0),eop(false),regex_status(0){;}
  
  // string overload methods    
  inline ocString & operator = ( const string & in ){ return operator=(in.c_str());}
  inline string & parse( const string & pattern ){ return parse(pattern.c_str());}
  inline ocString replace( const string & pattern, const string & replacement ){ return replace(pattern.c_str(),replacement.c_str()); }
  inline ocString replaceAll( const string & pattern, const string & replacement ){ return replaceAll(pattern.c_str(),replacement.c_str()); }
  inline ocString remove( const string & pattern ){ return remove( pattern.c_str()); }
    
  ocString & operator = ( const char * in )
  {
    string::operator = ( in );
    parseInit();
    return *this;
  }
 
  // append any type with a defined stream operator
  template<class T> ocString & append( T inVar )
  {
    ocAppend( *this, inVar );
    return *this;
  }
  virtual ~ocString(){;}
  /*
    Initializes the class for parsing
  */
  void parseInit( void )
  {
    retVal = "";
    idx=0;
    eop = false;
  }
  /**
    Parse the string into tokens based on the pattern
    This is how parse behaves:
    -------------------------------------------------------------
    If the pattern is NOT found:
      Return value is the whole string from the previous parse pos.
      End of parse (eop) is set to true.
      Parse pos is set to start of string.
    If the pattern IS found:
      Return value is the substring from the previous parse pos to the current found token.
      Parse position is set just beyond the found pattern.
    If eop set.
      Returns an empty string.
  */
  string & parse( const char * pattern )
  {
    string::size_type idxStart;
    retVal="";
    if( pattern && strlen( pattern ) && !eop )
    {
      idxStart = this->find( pattern, idx );
      if(idxStart != string::npos )
      {
        retVal = this->substr(idx,idxStart-idx);
        idx = idxStart+strlen(pattern); // move to the next position
      }
      else
      {
        retVal = this->substr(idx);
        idx=0; // start over
        eop=true;
      }
    }
    return retVal;
  }

  /**
    This one is like parse, 
    but it returns text delimited 
    by char tokens instead of strings
  */
  string & tokenParse( string tokens )
  {
    enum { before, inside, after, ended } cpos = before;
    retVal="";
    if( tokens.length()  && !eop )
    {
      string::size_type idxStart = idx, idxEnd = 0;
      while( cpos != ended && idx < size() )      
      {
        char test = this->operator[](idx);        
        if( cpos == before )
        {
          // check if no longer in tokens
          if( tokens.find(test) == string::npos )
          {
            cpos = inside;
            idxStart = idx;                         
          }
        }   
        else if( cpos == inside )
        {
          // check if back in tokens
          if( tokens.find(test) != string::npos )
          {
            cpos = after;
            idxEnd = idx;          
          }
        }
        else if( cpos==after )
        {
          // for parse compatibility, go to end of delimiters  
          if( tokens.find(test) == string::npos )
          {
            cpos = ended;
            break;
          }
        }        
        ++idx;        
      } // end while
      if( idxEnd > idxStart ) // did we find an end delimiter?       
      {    
        retVal = this->substr(idxStart,idxEnd-idxStart);              
      }
      else
      {   
        retVal = this->substr(idxStart);     
        idx=0; // start over
        eop=true;
      }  
    }
    return retVal;
  }

  /**
   remainder() get whats left after the last parse
  */
  string & remainder( void )
  {
    retVal = this->substr(idx);
    return retVal;
  }

  /**
    replace the first occurence of pattern with replacement
      return is a temporary ocString,
        so you can say strObj.replace().replace()...
        and have it do the expected thing.
  */
  ocString replace( const char * pattern, const char * replacement )
  {
    string::size_type idxPattern;
    retVal="";
    if( pattern )
    {
      size_t ptrnLen = strlen( pattern );
      if( ptrnLen > 0 )
      {
        idxPattern = this->find( pattern );
        if(idxPattern != string::npos )
        {
          retVal = this->substr(0, idxPattern );
          if( replacement && strlen(replacement) )
          {
            retVal += replacement;
          }
          retVal += this->substr(idxPattern+ptrnLen);
        }
      }
    }
    // set to this if no replacements made
    if( retVal.length() == 0 ) retVal = *this;
    return retVal;
  }

  /**
    replace all occurences of pattern with replacement
      return is a temporary ocString.
  */
  ocString replaceAll( const char * pattern, const char * replacement )
  {
    string::size_type idxPattern, idxFrom;
    retVal="";

    if( pattern )
    {
      size_t ptrnLen = strlen( pattern );
      //size_t replLen = strlen( replacement );
      if( ptrnLen > 0 )
      {
        idxFrom = 0;
        idxPattern = this->find( pattern );
        while(idxPattern != string::npos )
        {
          retVal += this->substr(idxFrom, idxPattern-idxFrom );
          if( replacement && strlen(replacement) )
          {
            retVal += replacement;
          }
          idxFrom = idxPattern+ptrnLen;
          idxPattern = this->find( pattern, idxFrom );
        }
        // any remainder
        retVal += this->substr(idxFrom);
      }
    }
    return retVal;
  }

  /**
    ocString remove(const char * pattern )
  
    remove the first occurence of pattern
      return is a temporary ocString.
  */
  ocString remove( const char * pattern )
  {
    string::size_type idxPattern;
    retVal=*this;
    if( pattern )
    {
      size_t ptrnLen = strlen( pattern );
      if( ptrnLen > 0 )
      {
        idxPattern = this->find( pattern );
        if(idxPattern != string::npos )
        {
          retVal.erase(idxPattern, ptrnLen);
        }
      }
    }
    return retVal;
  }

  /**
    string::size_type lastPos( void )
  
      Returns last parse position in string
  */
  string::size_type lastPos( void )
  {
    return idx;
  }
  /**
    bool endOfParse(void)
  
     end of parse is true whenever the parse detects there are no more longer have tokens to consume.  
  */
  bool endOfParse(void)
  {
    return eop;
  }
  /**
   ocString titleCase( void )
  
     return a copy with uppercased letter on every new word.
  */
  ocString titleCase( void )
  {
    string::iterator pos;
    retVal="";
    bool wasWhiteSpace = false;
    for( pos=begin(); pos!=end(); ++pos )
    {
      char ch = *pos;
      if(wasWhiteSpace || pos==begin() )
      {
        retVal += toupper(ch);
      }
      else
      {
        retVal += tolower(ch);
      }
      if( isalpha(ch) || ch=='\'' )
      {
        wasWhiteSpace = false;
      }
      else
      {
        wasWhiteSpace = true;
      }
    }
    return retVal;
  }
  /**
   bool regExMatch( const char *pattern )
  
     returns true of string matches the regulare expression pattern passed  
  */
  bool regExMatch( const char *pattern )
  {    
    const char *string = c_str();     
    regex_t    re;
    regex_status = regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB|RE_BACKSLASH_ESCAPE_IN_LISTS);
    if ( regex_status != 0) 
    {
        return(false);      /* report error */
    }
    regex_status = regexec(&re, string, (size_t) 0, NULL, 0);
    regfree(&re);
    if (regex_status != 0) 
    {
      return(false);      /* report error */
    }
    return(true);

  }
  int RegExStatus( void )
  {
    return regex_status;
  }
  
  unsigned long  makeHash( void )
  {
    unsigned long hash = 0;
    const char * str = this->c_str();
    int c;
    while ( (c = (unsigned int) (*str++)) )  hash = c + (hash << 6) + (hash << 16) - hash;
    return hash;
  }
};

#endif

#ifdef IN_T2_TESTHARNESS
  // unit testing
  cout << "=======================================" << endl;
  ocString thingy("This is \n\tSome parsable data");
  while( !thingy.endOfParse() )
  {
    cout << "Parsing: [" << thingy.tokenParse(" \t\n\r" ) << "]" << endl;
  }
  thingy.parseInit();
  cout << "=======================================" << endl;
  while( !thingy.endOfParse() )
  {
    cout << "Parsing: [" << thingy.parse(" ") << "]" << endl;
  }
  
  cout << "checking out reg exp matching" << endl;
  thingy = "david.mc@w3.sys.com";
  cout <<  thingy.regExMatch("^[a-z\\.A-Z1-9\\-]+@[a-z\\.A-Z1-9\\-]+$") << " should be true" << endl;
  thingy = "david-mc@w3-sys.com";
  cout <<  thingy.regExMatch("^[a-z\\.A-Z1-9\\-]+@[a-z\\.A-Z1-9\\-]+$") << " should be true" << endl;
  thingy = "david9mcw3.sys9.com";
  cout <<  thingy.regExMatch("^[a-z\\.A-Z1-9\\-]+@[a-z\\.A-Z1-9\\-]+$") << " should be false" << endl;
  thingy = "david.mcw@";
  cout <<  thingy.regExMatch("^[a-z\\.A-Z1-9\\-]+@[a-z\\.A-Z1-9\\-]+$") << " should be false" << endl;
  thingy = "Hutch0099@cs.com";
  cout <<  thingy.regExMatch("^[a-z\\.A-Z0-9\\-]+@[a-z\\.A-Z0-9\\-]+$") << " should be true" << endl;
  cout << thingy.RegExStatus() << endl;
  
  thingy = "something.cgi?isPDF=1";
  cout <<  thingy << " " << thingy.find("isPDF") << endl;
  cout <<  thingy << " " << thingy.regExMatch("isPDF") << endl;  
  
  /*
  cout <<  thingy.regExMatch("^this") << " should be false" << endl;
  cout <<  thingy.regExMatch("\n\tS") << " should be true" << endl;
  cout <<  thingy.regExMatch("^\n\tS") << " should be false" << endl;
  cout <<  thingy.regExMatch("data$") << " should be true" << endl;
  */

  cout << "String is 'gooby gotta go'" << endl;

  thingy = "gooby gotta go";

  cout << "Match pattern = '(go|do)'" << endl;

  cout <<  thingy.regExMatch("(go|do)") << " should be true" << endl;
  
  /* hash testing */
  
  ocString hashable = "Big Goofy String";
  cout << hashable <<  " hash value is " << hashable.makeHash() << endl;
  hashable = "Profound";
  cout << hashable <<  " hash value is " << hashable.makeHash() << endl;
  hashable = "The eclipse burdened the mood with gloom, even on the bright side, if there were such.";
  cout << hashable <<  " hash value is " << hashable.makeHash() << endl;

  ocString split = "davidmc@w3sys.com,davidmc@dalton.net,w3sysdesign@google.com";
  string current = split.parse(",");

  while( !split.endOfParse() )
  {
    cout << "Current is " << current << endl;
    current = split.parse(",");
  }

  cout << "testing complete!" << endl;


#endif
