/*
  rant.cpp
  random ranting version of pages.meta

*/
//#define OPEN_LOGGING_FILE
#include "openLogger.h"
#include "connectstring"
#include "cgiClass.h"
#include "ocXML.h"
// one global
string baseUrl;
#include "page.hpp"
#include "page_control.hpp"
#include <time.h>
#include <vector>
using namespace std;

typedef vector<string> words;
class rant
{
  public:
    string buf;
    words attributes;
    words predicates;
    words nouns;
    words adverbs;
    words hedge;
    words clause;
    words prep;
    words pred_prep;
    enum tense { Present, Past } Tense;
    int nsentences;
    int Lpos;
    bool Intitle, Inclause, Inpred;
    rant():Tense(present),nsentences(50),
    
    {
      ifstream grammer("grammer.xml");
      xmlParser gparse(grammer);
      gparse.parse();
      gparse.states.nodeMap["attributes"];
      
      time_t lt;
      time(&lt);
      srand(lt);
    }
    ~rant(){;}

    // the randomizing phrase picker
    inline string & pick(words & ary)
    {
      return ary[rand()%(ary.length())];
    }
    // generate the rant 
    void generate( ostream & away )
    {
      title(away);

    }
    // title of rant
    void title (ostream & away)
    {
      Intitle = 1;
      buf = "The ";
      buf += pick(nouns);
      buf += "."
      away << output ();
      buf = "";
      attribute();
      buf += " ";
      buf += pick(nouns);
      buf += " ";
      buf += pick(prep));
      buf += " ";
      nounphr();
      Intitle = 0;
    }
    void sentence (ostream & away)
    {
      if (rand() % 3) put_clause (buf);
      nounphr (buf);
      buf += " ";
      be (buf);
      predicate (buf);
      away << output();
    }
    void nounphr (away)
    {
        string  noun = pick(nouns);
        if (index(noun, ' ') || Intitle || !(random() % 4))
          buf =  !(random() % 2)? "the ": (isvowel(noun[0]))? "an ": "a ";
        else
        {
          attribute (buf);
          buf += " ";
        }
        buf += noun;
        if (!Intitle && !Inpred && !(random() % 3))
        {
          buf += " ";
          buf += pick(prep);
          buf += " ";
          nounphr (buf);
        }
    }
    attribute (char *buf)
    {
        register char *nextw, *attr, *adv = NULL;
    
        nextw = attr = PICK1(attributes);
        if (index(attr, ' ') == NULL && !(random() % 3))
      nextw = adv = PICK1(adverbs);
        strcat (buf, !Intitle && !(random() % 2)? "the ":
        (isvowel(*nextw))? "an ": "a ");
        if (adv) {
      strcat (buf, adv);
      strcat (buf, " ");
        }
        strcat (buf, attr);
    }
    
    be (char *buf)
    {
        register char *verb;
    
        verb =  Tense == PRESENT? "is ":
          Tense == PAST? "was ":
          random() % 3? "is " :
          "was ";
        Tense = *verb == 'i'? PRESENT: PAST;
        strcat (buf, verb);
        if (!(random() % 3))
      strcat (buf, "not ");
    }
    
    predicate (char *buf)
    {
        register char *pred;
    
        Inpred = 1;
        if (random() % 3) {
      if (!(random() % 10))
          strcat (buf, "known to be ");
      pred = PICK1(predicates);
      if (index(pred, ' ') == NULL && !(random() % 2)) {
          strcat (buf, PICK1(adverbs));
          strcat (buf, " ");
      }
      strcat (buf, pred);
        }
        else {
      strcat (buf, PICK1(pred_prep));
      strcat (buf, " ");
      nounphr (buf);
        }
        Inpred = 0;
    }
    
    put_clause (char *buf)  /* a clause to preceed the sentence */
    {
      if (!Inclause) {    /* Recursive clauses are too much! */
        Inclause = 1;
        if (random() % 3) {
          strcat (buf, PICK1(hedge));
          strcat (buf, " ");
          Tense = PRESENT;
        }
        else {
          strcat (buf, PICK1(clause));
          strcat (buf, " ");
          sentence (buf);
          strcat (buf, ", ");
        }
        Inclause = 0;
      }
    }
    
    output( char *bufbuf)
    {
      register char *sp, *p;
    
      if (*buf == '\1')
        buf++;
      if (islower(*buf))
        *buf = toupper (*buf);
      if (!Intitle)
        strcat (buf, ".");
      for (sp = buf; *sp; sp++) {
        if (*sp == '\1')
          continue;
        if (sp == buf || *sp == ' ') {
          for (p = sp + 1; *p && *p != ' '; p++);
          if (Lpos + (p - sp) + (sp == buf? 2: 0) >= 72) {
            putchar('\n');
            Lpos = 0;
            if (*sp == ' ')
              continue;
          }
          else if (sp == buf && Lpos > 0) {
            putchar(' ');
            if (!Intitle)
              putchar(' ');
          }
        }
        putchar(*sp);
        Lpos++;
      }
    }

} ;

// Standard main function
int main(int argc, char ** argv)
{  
  baseUrl = "pages.meta";  // this is my compiled name!
  writelog( "instance of script" );
  cgiScript script;  
  writelog( "instance of page" );
  page pg(script);  
  writelog( "load page" );  
  if( pg.load() )
  {    
    writelog( "instance of page control" );
    page_control ctrl(pg);     
    writelog( "page control emit" );
    ctrl.emit ();    
  }
  else
  {
    script << "<html><head><title>Page Unavailable</title></head>" << endl
           << "<body><h1>Sorry</h1>" << endl
           << "<p>The page you requested is not available</p></body></html>";
  } 
  return 0;
}
