/*
  codeMap.hpp
  ===========
  
*/

#ifndef CODEMAP_HPP
#define CODEMAP_HPP
#include <map>
#include <vector>
#include <fstream>

typedef map<string,string> xlation;

/* 
  Array of xlation for typical or specialized maps 
  of type to type (or type to code blocks)
*/
typedef map<string,xlation> type_xlation;

class field {
public:
  string name;
  string type;
  field(const field & in):name(in.name),type(in.type){;}
  field(){;}
  ~field(){;}
  field & operator = (const field & in )
  {
    name=in.name;
    type=in.type;
  }
};

// BY CONVENTION - THE FIRST FIELD IS ALWAYS THE PHYSICAL KEY!
typedef vector<field> field_array;

class codeMap
{
  // the result to return;
  string result;
  
  // Hold the field & types here
  field_array fields;
  
  // hold the 2d array of types here
  type_xlation typemaps;
  
  // keep track of the class name;
  string class_name;  
  
  // files to open
  ifstream itemplate;
  ifstream imap;
  
public:  
  
  void addTypes ( string typeline )
  {
    aString typeTokenizer = typeline;
    
    // make sure key is canonical
    string type = "[[";
    type += typeTokenizer.token("[]");
    type += "]]";
    
    // debug  result += "\nAdding Types: " + type;
    xlation & xlt = typemaps[type];
    while( imap )
    {
      ocString line;
      getline( imap, line );
            
      if( line.regExMatch("=") )
      {                
        aString lineTokenizer = line;        
        string type = lineTokenizer.token("=");
        // result += "\nAdding Type Xlation: " + type;
        
        string fartype =  lineTokenizer.remainder();
        
        xlt[type]=fartype;
        // debug     result += "\nType xlate added " + type + "=" + xlt[type];        
        
      } 
      else if( line.regExMatch("^\\[") )
      {
        // a little recursion
        addTypes ( line );
      }
      else
      {
        break;
      }
    }    
  }
  bool parseMapFile( void )
  {  
    bool ret = false;  
    string mapfile;
    getline( itemplate, mapfile );
    aString line = mapfile.c_str();
    mapfile = line.token(" =;"); // parse off TYPEMAP
    // test
    if( mapfile == "TYPEMAP" )
    {
      mapfile = line.token(" =;");
      // debug      result += "\n mapfile: " + mapfile;
      // try to open it
      imap.open( mapfile.c_str() );
      if( imap.good() )
      {
        ret = true;
        ocString line;
        getline( imap, line );
        // debug  result += "\n first line: " + line +"\n";                  
        if( line.regExMatch("^\\[") )
        {
          addTypes(line); 
        }
      }
    }
    return ret;
  }
  
  /*
    Valid tokens
    [[CLASSNAME]]
    [[FIELD-REPEATING]]
    [[TYPE]], [[TYPE2]] ... [[TYPE(n)]] 
    [[FIELD]]
    [[/FIELD-REPEATING]]
    [[NONKEY-REPEATING]]
    [[/NONKEY-REPEATING]]
    [[KEY]]
  
  */
  void doCodeMapping(string & class_name, string & fields)
  {
    this->class_name = class_name;
    // put the fields in their place
    if( ! addfields( fields )  )
    {
      result = "COULD NOT ADD FIELDs! ";
      result += fields;
      return;
    }  
    while( itemplate )
    {
      ocString line;
      getline(itemplate,line);

      // check for [[FIELD-REPEATING]]
      if( line.regExMatch("\\[\\[FIELD-REPEATING\\]\\]") )
      {
        fieldRepeatingCode();
      }
      // else check for [[NONKEY-REPEATING]]
      else if( line.regExMatch("\\[\\[NONKEY-REPEATING\\]\\]") )
      {
        fieldRepeatingCode(false);
      }      
      // else check for [[KEY]]
      else if( line.regExMatch("\\[\\[KEY\\]\\]") )
      {
        if( fields.size() )
        {
          field & fld = this->fields[0];
          typeReplacements( line, fld.type );
          result += line.replaceAll("[[CLASSNAME]]",class_name)
                        .replaceAll("[[KEY]]",fld.name); 
          result += "\n";
        }
      }
      //  Finally use straight replacement for  [[CLASSNAME]]
      else 
      {
        result += line.replaceAll("[[CLASSNAME]]",class_name);      
        result += "\n";
      }  
    }
  }
  bool addfields( ocString fields )
  {
    bool bRet = false;
    while( !fields.endOfParse() )
    {
      aString FieldPattern = fields.parse("|");
      char * tok = FieldPattern.token( " " );      
      field fld;
      while( tok )
      {
        string temp = tok;
        char * tok2 = FieldPattern.token( " " );
        if( tok2 )
        {
          // create canonical type
          if( fld.type.length() ) fld.type += " ";
          fld.type += temp;           
        }
        else
        {
          // the name is always the last word
          fld.name += temp; 
        }
        tok = tok2; 
      }
      this->fields.push_back(fld);
      bRet = true;
      // debug result += "\nField: " + fld.type + " " + fld.name + "; added"; 
    }
    return bRet;
  } 
  /*
    Valid tokens
    [[CLASSNAME]]
    [[TYPE]], [[TYPE2]] ... [[TYPE(n)]] + [[TYPEINIT]]
    [[FIELD]]    
  */
  void fieldRepeatingCode( bool includeKey=true )
  {
    ocString pattern;
    // get the whole multi-line pattern fomr the code template
    while( itemplate )
    {
      ocString line;
      bool isEnding = false;
      getline(itemplate,line);
      if( includeKey && line.regExMatch("\\[\\[/FIELD-REPEATING\\]\\]") ) 
      {
        line = line.remove( "[[/FIELD-REPEATING]]" );
        isEnding = true;
      }
      if( includeKey == false && line.regExMatch("\\[\\[/NONKEY-REPEATING\\]\\]") )
      {
        line = line.remove( "[[/NONKEY-REPEATING]]" );
        isEnding = true;
      }
      pattern += line;
      if( isEnding ) break;
      else pattern += "\n";
    } // end while
    // now replace the tokens on the pattern
    int i;    
    for( i=includeKey?0:1; i<fields.size(); i++ )
    {
      field & fld = fields[i];
      ocString codeChunk = pattern.replaceAll("[[CLASSNAME]]",class_name)
                                  .replaceAll("[[FIELD]]",fld.name);
      typeReplacements( codeChunk, fld.type );
      // last filter, check for conditional text based on end of field list
      terminalCheck( codeChunk, (i+1)==fields.size() );
      result += codeChunk;
    }  
  }
  /*
    Valid tokens  
    [[IFNEND]],[[/IFNEND]]
    
    YOU MUST HAVE BOTH on the same line
   */ 
  void terminalCheck( ocString & codeChunk, bool isEnd )
  {                                           // 1234567890  
    string::size_type position = codeChunk.find("[[IFNEND]]");    
    if(position != string::npos)
    {                                            //  12345678901
      string::size_type eposition = codeChunk.find("[[/IFNEND]]");
      if(eposition != string::npos)
      {
        ocString temp = codeChunk.substr(0,position);
        if( !isEnd )
        {
          string::size_type len = eposition - (position+10);
          temp += codeChunk.substr(position+10,len);                    
        }
        temp += codeChunk.substr(eposition+11);
        codeChunk = temp;                  
      }      
    }    
  }
  
  void typeReplacements( ocString & codeChunk, string & type )
  {
    type_xlation::iterator pos;
    for(pos=typemaps.begin();pos!=typemaps.end();++pos)
    {
      if( pos->second.find(type) != pos->second.end() )
      {
        codeChunk = codeChunk.replaceAll(pos->first.c_str(),
                                         pos->second[type].c_str()); 
      }
    }
  }
  
  string & genCode(string & class_name, string & fields, string & template_name )
  {
    string path = "./CodeTemplates/";
    path += template_name;    
    itemplate.open( path.c_str() );
    if( itemplate.good() )
    {      
      if( parseMapFile() )
      {
        // debug        result += "\nGOOD MAP FILE!";
        doCodeMapping(class_name,fields);         
      }
    }
    else
    {
      result = "Could not load: ";
      result +=  path;
      cerr << result;
    }
    return result;
  }
};

#endif
