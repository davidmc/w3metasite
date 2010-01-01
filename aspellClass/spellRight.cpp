/*
  spellRight.cpp

  example and starting point for other code.

*/                      

#include <iostream>
#include <iomanip>
#include "AspellWrapperClass.hpp"

int main( int argc,  char** argv )
{
  if( argc == 2 )
  {
    AspellWrapper speller("en_US");
    
    cout << speller.error << endl;

    // add the custom dictionary
    speller.addWords("hydrowords");

    string passedArg = argv[1];
    cout << "Spelling for: " << passedArg << endl;

    if( passedArg.find(" ") != string::npos ) // whitespace implies multiple words
    {
      wordlist badWords;
      if( speller.checkText( passedArg, badWords ) )
      {
        cout << "Your spelling is good!" << endl;
      }
      else
      {
        cout << "Might we suggest... " << endl;
        for( int i = 0; i < badWords.size(); i++ )
        {
          string & badWord = badWords[i];
          cout << "FOR  " << badWord << ":" << endl;
          alternatives & list = speller.suggestWords(badWord);
          for( int l=0; l < list.size(); ++l )
          {
            cout << list[l] << endl;
          }
        }
      }
    }
    else
    {
      if( speller.checkWord( passedArg ) )
      {
        cout << "Your spelling is good!" << endl;
      }
      else
      {
        cout << "Might we suggest: " << endl;
        alternatives & list = speller.suggestWords(argv[1]);
        for( int l=0; l < list.size(); ++l )
        {
          cout << list[l] << endl;
        }
      }
    }
  }
  return 0;
}

 //  .aspell.HYDRO.pws    en_HYDRO.multi
