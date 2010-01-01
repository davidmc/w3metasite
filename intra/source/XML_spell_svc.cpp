/*
  XML_spell_svc.cpp
  (c) 2007 David McCombs
  davidmc@w3sys.com

*/

#include <iostream>
#include <iomanip>
#include "cgiClass.h"
#include "AspellWrapperClass.hpp"

using namespace std;

int main( int argc, char ** argv )
{
  cgiScript script("text/xml",true);
  script << "<?xml version='1.0' encoding='UTF-8' standalone='yes'?>"  << endl;
  script << "<W3SysService>" << endl;

  AspellWrapper speller("en_US");
  cgiInput & args = script.ClientArguments();

  // To check against a custom dictionary
  if( args.count("dictionary") )
  {
    speller.addWords(args["dictionary"].c_str());
  }
  if( args.count("word") )  // checks a single word
  {
    
    if( speller.checkWord(args["word"].c_str()) )
    {
      script << "<good/>" << endl;
    }
    else
    {
      alternatives & list = speller.suggestWords(args["word"].c_str());
      for( int l=0; l < list.size(); ++l )
      {
        script << "<suggest>" << list[l] << "</suggest>" << endl;
      }
    }
  }
  else if( args.count("text") ) // checks text
  {
    wordlist badWords;
    ocString fText = args["text"].c_str();
    
    if( speller.checkText(fText, badWords) )
    {
      script << "<good/>" << endl;
    }
    else
    {
      bool noSuggestions = true;
      for( int i = 0; i < badWords.size(); i++ )
      {
        string & badWord = badWords[i];
        alternatives & list = speller.suggestWords(badWord);
        for( int l=0; l < list.size(); ++l )
        {
          script << "<suggest word='"<< badWord <<"'>" << list[l] << "</suggest>" << endl;
          noSuggestions = false;
        }
      }
      if( noSuggestions ) // if you cant suggest something, then allow it!
      {
        script << "<good/>" << endl;
      }
    }
  }
  else
  {
    script << "<what>You asked no good question about spelling! Usage: dictionary=name&(word=word||text=text)</what>" << endl;
  }
  script << "</W3SysService>" << endl;
  return 0;
}



