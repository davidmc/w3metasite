/*
  AspellWrapperClass.hpp

  C++ STL compatible wrapper class for aspell api.
    http://aspell.net
    Version 0.9

  We need a single library:       libaspell.so
  We need a single include file:  aspell.h

  David McCombs davidmc@w3sys.com
  This code is free use and extend as you see fit.
    http://www.w3sys.com

*/

#ifndef   ASPELLWRAPPERCLASS_HPP
#define   ASPELLWRAPPERCLASS_HPP

#include "aspell.h"
#include <string>
#include <vector>
#include <fstream>
#include "ocString.h"

using namespace std;

typedef vector<string> wordlist;
typedef wordlist alternatives;

class AspellWrapper 
{
private:
  AspellConfig * config;
  AspellSpeller * speller;
public: 
  string error;
  alternatives suggestions;

public:
  AspellWrapper( string lang = "en_US" ):config(0),speller(0)
  { 
    AspellCanHaveError * ret = 0;
    config = new_aspell_config();
    aspell_config_replace(config, "lang", lang.c_str());

    // create the aspeller
    ret = new_aspell_speller(config);
    
    // set error if any
    const struct AspellError * arc = aspell_error((const AspellCanHaveError *)ret);
    if( arc != 0) 
    {
      error = aspell_error_message(ret);
      delete_aspell_can_have_error(ret);
    }
    // delete configurer
    delete_aspell_config(config);

    // make aspell instance
    speller = to_aspell_speller(ret);
  }

  // The path to the word list - words delimited by \n
  bool addWords( string path )
  {
    char buffer[512];
    ifstream inFile;
    inFile.open(path.c_str());
    bool breturn = inFile.is_open();
    if ( breturn )
    {
      do {
        inFile.getline (buffer,sizeof(buffer));
        if( strlen(buffer) )
        {
          aspell_speller_add_to_session(speller, buffer, strlen(buffer));
        }
      } while(  !inFile.eof() );
    }
  }
  bool checkWord( string word )
  { 
    int have = 0;
    if(speller)
      have = aspell_speller_check(speller, word.c_str(), -1);
    // -1 means null terminated string
    return have==1;
  }

  bool checkText( ocString text )
  {
    bool ret = true;
    string tokens=" \t\n\r,.;:!?()[]{}";
    text.parseInit();
    string word = text.tokenParse(tokens);
    while ( word.length() )
    {
      ret = checkWord(word);
      if( ret == false )
        break;
      word = text.tokenParse(tokens);
    }
    return ret;
  }

  bool checkText( ocString text, wordlist & badWords )
  {
    bool ret = true;
    
    string tokens=" \t\n\r,.;:!?()[]{}";
    text.parseInit();
    string word = text.tokenParse(tokens);
    while ( word.length() )
    {
      // DBG cout << "............ checking: " << word << endl;
      bool tRet = checkWord(word);
      if( tRet == false )
      {
        badWords.push_back(word);
        ret = false;
      }
      word = text.tokenParse(tokens);
    } 
    return ret;
  }

  alternatives & suggestWords( string word )
  {
    suggestions.clear();
    if(speller)
    {
      const AspellWordList *wl = aspell_speller_suggest(speller,word.c_str(),-1);
      AspellStringEnumeration * els = aspell_word_list_elements(wl);
      if( els )
      {
        const char * chWord;
        while ( (chWord = aspell_string_enumeration_next(els)) != 0) 
        {
          string strWord = chWord;
          suggestions.push_back(strWord);
        }
        delete_aspell_string_enumeration(els);
      }
    }
    return suggestions;
  }

  ~AspellWrapper()
  {
    if(speller) delete_aspell_speller(speller);
  }
};

#endif
