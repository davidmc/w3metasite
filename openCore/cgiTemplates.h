/*

  The cgiTemplates class
  ----------------------
  The rationale behind this class
  is the separation of content
  and style from function.

  Content and Style stay in a standard html (or XML)
  page. After a page is written using a WYSIWYG
  editor (or whatever) it is marked up with SGML
  comments to mark the begining and ending of paragraphs.

  Comment Format:
  ========================================

  paragraph beginning: <!--@label-->

  paragraph ending: <!--/@label-->

  ========================================
  The label can be any alpha-numeric
  sequence of characters

  Function comes from the cgi program.
  The cgi program calls this class to load and parse
  the html/xml into paragraphs.

  The program can then intersperse dynamically
  generated content with the static paragraphs.

  User defined argument markers can be placed in the
  paragraphs so as to be replaced by program variables.
  Replacement is most easily accomplished by using the
  ocString methods replace and replaceAll.  For type conversion
  to string there is a function template called ocAppend( string, var ).
  ocAppend and ocString are included in the "ocString.h" file

  Having worked in some of the scripted environments, I believe this
  method is superior in terms of performance, flexibility and reliability.

  All code is compiled, the parsing is fast, and is done against
  a well defined set of tokens.

  The user can completely change the visual interface
  without effecting the function (so long as the SGML comments and
  argument markers are preserved.)

  All code checks are done at compile time rather
  that run time - this means the user is less likely to be exposed
  to some un-anticipated error.

  Copyright 2002 - Open Core Class Library
  Author David McCombs

*/
#ifndef CGI_PARAGRAPHS_H
#define CGI_PARAGRAPHS_H

#include <map>
#include <string>
#include <fstream>
#include <iomanip>
#include "ocString.h"

typedef map < string, string > paragraphMap;

class cgiTemplates
{
private:
  ocString unparsed;
  paragraphMap parsed;
  bool comments;
  bool parse ( void );

public:
  // default and only constructor
  cgiTemplates();

  // non virtual destructor
  ~cgiTemplates();

  // load and parse the file */
  bool load ( const char * filename );
  cgiTemplates & operator = ( const cgiTemplates & in );

  // return the paragraph labeled by 'key
  string & getParagraph( const string & key );
  string & getParagraph( const char * key );

  // to have the tag name appear as an SGML comment around inserted paragraphs...
  void commentsOn(void);
  void commentsOff(void); // the default

  // return the content of the html file unparsed
  string & getUnparsedHtml( void );

  // return the whole array of parsed paragraphs
  inline paragraphMap & getParagraphs( void ) { return  parsed; }
};

#endif
