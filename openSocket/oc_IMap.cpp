#include "oc_IMap.h"

// test harness
void mailCli( oc_IMap & mail )
{
  char sendline[MAXLINE];

  cout << "mail>>";
  while (fgets(sendline, MAXLINE, stdin) != NULL)
  {
    if( strlen( sendline )> 1 )
    {
      // debugging
      cout << endl << "Command: " << sendline ;
      mail.Command( sendline );
      do
      {
        // debugging cout << endl << "More!" << endl;
        string Response = mail.Response();
        if( Response.length() )
        {
          cout << Response;
        }
      } while ( ! mail.GetResponseComplete() );

      cout << "==============end of read============" << endl;

    } // end if user input
    cout << "mail>>";
  }
}

int main(int argc, char *argv[])	/* simple TCP client */
{
  if( argc == 4 )
  {
    try
    {
      oc_IMap mail( "webMail", "127.0.0.1", 143 );

      // login
      if( mail.login( argv[1], argv[2] ) )
      {
        cout << "login ok" << endl;

        if( mail.select( argv[3] ) )
        {
          cout << "mailbox ok" << endl;
          if( mail.fetchHeaders() )
          {
            cout << "==== Items in mail =====" << endl;
            vector <string> & listItems = mail.GetListItems();
            for( size_t x=0; x<listItems.size(); x++ )
            {
              mlistItem mitem( listItems[x] );
              cout << "id " << mitem.id << endl
                   << " to " << mitem.to << endl
                   << " from " << mitem.from << endl
                   << " sent " << mitem.sent << endl
                   << " reply_to " << mitem.reply_to << endl
                   << " cc " << mitem.cc << endl
                   << " subject " << mitem.subject << endl
                   << " received " << mitem.received << endl
                   << " flags " << mitem.flags  << endl
                   << " size " << mitem.size << endl
                   << endl;
            }
            cout << "==== End of items in mail =====" << endl;
          }

          // assume we have 1 peice of mail
          cout << mail.fetchRFC822("1");

          // command loop
          mailCli( mail );
        }
        else throw exception();
      }
      else throw exception();

    }
    catch( exception & err )
    {
      cout << "Caught " << err.what() << endl;
      return -1;
    }
  }
  else
  {
    cout << "usage: oc_IMap user pwd mailbox" << endl;
  }
  return 0;
}



