/*
  ocImportParser.h
  ================
  Imports delimited data into a vector of columns

  (c) 2002 David McCombs w3sys.com

*/

// Compile sentry
#ifndef OCIMPORTPARSER_H
#define OCIMPORTPARSER_H

struct ocFileCol
{
  string name;  
  string value;
  int iMapPos;
  ocFileCol():iMapPos(0L){;}
  ocFileCol( const ocFileCol & in )
  :name(in.name),value(in.value),iMapPos(in.iMapPos){;}
  ~ocFileCol(){;}
  ocFileCol & operator = (const ocFileCol & in )
  {
    name = in.name;
    value = in.value;
    iMapPos = in.iMapPos;
    return * this;
  }
};

typedef vector<ocFileCol> ocFileCols;

class ocFileParser
{
  string cDelim;
  string cField;
  int colCount;
  ocFileCols cols;
  ifstream inFile;
  ocString  lineTokens;
  bool bTrimQuote;
  bool bCheckQuote;
  bool notDone;
  
public:

  string attrSep;
  ocFileParser()
  :colCount(0),cDelim(","),cField("\""),cols(100)
  ,bTrimQuote(false),bCheckQuote(false)
  ,notDone(false),attrSep("~|~")
  {;}
  ~ocFileParser(){;}

  // clear values only, the name should stay set
  void clearData( void )
  {
    for( int i=0; i < cols.size(); i++ ) cols[i].value = "";
  }

  bool openFile( string filePath )
  {
    inFile.open(filePath.c_str());
    return( inFile.is_open() );
  }
  bool closeFile( void )
  {
    if( inFile.is_open() )
    {
      inFile.close();
      inFile.clear();
      cols.clear();
    }
    return true;
  }
  
  bool isEven( int cnt )
  {
    return cnt%2 == 0;
  }
  
  bool getLine ( void )
  {
    bool bret = false;
    lineTokens = "";
    bool eol = false;
    char ch;
    // if were quote checking, 
    // this should always be even at end of line
    int quoteCount = 0;


    if( inFile.is_open() )
    {
      while( inFile.eof() == false
             && eol == false )
      {
        ch = inFile.get();

        if( ch == EOF ) break;
        
        // increment if quote checking
        if( ch == '\"' &&  bCheckQuote )
        {
           ++quoteCount;           
        }

        lineTokens += ch;

        int len = lineTokens.length();
        if( len 
            &&  lineTokens[len-1] == '\n' 
            &&  isEven(quoteCount)
          )
        {          
          lineTokens.resize(len-1);
          eol = true;
          len = lineTokens.length();
          if( lineTokens[len-1] == '\r' ) // possible carraige ret from Windows files
          {
            lineTokens.resize(len-1);
          }
        }
      } // end while
    }
    if(lineTokens.length())
    {
      bret = true;
    }
    return bret;
  }
  bool parseLine( bool isLabel = false )
  {
    bool bret = false;
    if( getLine() )
    {
      bret = parse(isLabel);
    }
    return bret;
  }
  void trimQuote( string & tok )
  {
    int len = tok.length();
    if( len )
    {
      if( tok[0] == '\"' )
      {
        tok.erase(0,1);
      }
      len = tok.length();
      if( len )
      {
        if( tok[len-1] == '\"' )
        {
          tok.resize(len-1);
        }
      }
    }
  }
  // Use matched quotes trick to reveal true delimiters
  bool hasEvenQuotes( string & tok )
  {
    int cnt = 0;
    for( int i = 0; i < tok.length(); i++ )
    {
      if( tok[i] == '\"' ) ++cnt;
    }
    return isEven(cnt);
  }
  
  // """Hey Baby"",""Oh Baby"",""Oh!""",
  // return whether a text field
  bool checkQuote( string & tok )
  {
    int len = tok.length();
    bool evenQuotes =  hasEvenQuotes(tok);
    bool bret = false;
    if( len ) // the token has length
    {
      if( tok[0] == '\"' )  // the start has a quote, so it may be text
      {
        bret = true;
        if( len > 1 && tok[len-1] == '\"' && evenQuotes ) // true unless the delim is part of text
        {
          tok.erase(0,1);
          len = tok.length();
          tok.resize(len-1);
        }
        else // delim part of text, add it back
        {

          tok += cDelim; // add back the delimiter stolen from the field
          tok += lineTokens.parse(cDelim.c_str());
          // recurse
          checkQuote(tok);
        }
      }
    }
  }

  bool parse( bool isLabel = false )
  {
    lineTokens.parseInit();
    int colCount = 0;
    clearData();
        
    while(lineTokens.endOfParse() == false )
    {
      string tok = lineTokens.parse(cDelim.c_str());
      if( bTrimQuote )
      {
        trimQuote( tok );
      }
      else if ( bCheckQuote )
      {
        if( checkQuote( tok  ) ) trimQuote( tok );
      }
      ocFileCol & rfc = cols[colCount];
      if(isLabel)
      {
        rfc.name = tok;
      }
      else
      {
        // append values instead of assigning them in case of multiple mapping
        // Especially useful for attributes
        if(rfc.value.length())
        {
          rfc.value += attrSep;
        }
        rfc.value += tok;
      }
      rfc.iMapPos = colCount;
      colCount ++;
    }

    if( isLabel ) this->colCount = colCount;
    
    return true;
  }
  inline int getColCount(void)
  {
    return colCount;
  }
  inline void setColCount( int iCount )
  {
    colCount = iCount ;
  }
  inline ocFileCols & getCols( void )
  {
    return cols;
  }
  ocFileParser & setDelim(string in)
  {
    cDelim = in;
    return * this;
  }
  ocFileParser & setTrimQuote( bool in )
  {
    bTrimQuote = in;
    return * this;
  }
  ocFileParser & setCheckQuote( bool in )
  {
    bCheckQuote = in;
    return * this;
  }
  ocFileParser & setLineTokens( string in )
  {
    lineTokens = in;
    return * this;
  }
  string getLineTokens( void )
  {
    return lineTokens;
  }
};

#endif
