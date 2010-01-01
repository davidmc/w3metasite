/*
  vlistmanager.h

  Virtual List Manager
  ======================
  Keeps track of where we are in the dataset between stateless
  data requests.
  -------------
  Uses session cookies (must have cgiCookie.h)
  On client side - must use of openDom.js

*/
#ifndef Virtual_LIST_MANAGER_H
#define Virtual_LIST_MANAGER_H
struct vListManager{
  int perPage;      // how many items per page
  int rowCount;     // total row count
  int direction;    // down = -1 up = 1
  int realAmount;
  int previousPos;  // the previous starting position
  int currentPos;   // the starting position for the current query
  int nextPos;      // the next starting position
  bool eof;         // at end of file
  bool bof;         // at beginning of file
  string args;      // arguments to send

  // Use this constructor!
  vListManager( int pagePer )
  :perPage(pagePer),rowCount(0),direction(1),realAmount(0),previousPos(0)
  ,currentPos(0),nextPos(pagePer),eof(false),bof(false)
  {
    // get initial extra info from the cookies
    cgiCookie cookie;
    string value;
    value = cookie.get("scroll");
    if( value.length() )
    {
      value = cookie.get("currentPos");
      currentPos = atoi(value.c_str());
      value = cookie.get("direction");
      if(value.length())
      {
        direction = atoi(value.c_str());
      }
      value = cookie.get("rowCount");
      if(value.length())
      {
        rowCount = atoi(value.c_str());
      }
      value = cookie.get("realAmount");
      {
        realAmount = atoi(value.c_str());
      }
      if(direction<0)
      {
        currentPos -= (realAmount+perPage);
      }
      if( currentPos >= rowCount-1 )
      {
        eof = true;
        currentPos = rowCount-perPage;
      }
      if( currentPos <= 0 )
      {
        bof = true;
        currentPos = 0;
      }
      realAmount = 0;
    }
  }
  void inc( void )
  {
    realAmount++;
  }
  /*
    Event after the query
  */
  void postQuery( int realAmount)
  {
    // update info for output by lastScript()
    previousPos = currentPos;
    currentPos  = currentPos + realAmount;
  }

  // return row position and limit for DB selects
  string getLimit()
  {
    ocString limit;
    limit.append(currentPos);
    limit += ", ";
    limit.append(perPage);
    return limit;
  }

  // The current navigation controls (if any)
  string navigation ( void )
  {
    ocString nav;
    // only emit controls if the rowCount is greater than perPage
    if( rowCount > perPage )
    {
      if( ! bof )
      {
        nav = "<a href='javascript:goBack()'> &nbsp; &lt; &nbsp; Previous &nbsp; </a>";
      }
      nav+= "&nbsp; Page ";
      nav.append(currentPos+1);      
      nav.append(currentPos+realAmount);
      nav+= " of ";
      nav.append(rowCount/perPage);
      nav+= " Pages &nbsp;";
      if( ! eof )
      {
        nav+= "<a href='javascript:goForward()'> &nbsp; Next &nbsp; &gt; &nbsp; </a>";
      }
    }
    return nav;
  }

  // The current navigation controls (if any)
  string navigation_lite ( void )
  {
    ocString nav;
    // only emit controls if the rowCount is greater than perPage
    if( rowCount > perPage )
    {
      if( ! bof )
      {
        nav = "<a href='javascript:goBack()'> &nbsp; &lt; &nbsp; Previous &nbsp; </a>";
      }
      nav+= "&nbsp; &nbsp;";
      nav.append(rowCount);
      nav+= " items found &nbsp; &nbsp;";
      if( ! eof )
      {
        nav+= "<a href='javascript:goForward()'> &nbsp; Next &nbsp; &gt; &nbsp; </a>";
      }
    }
    return nav;
  }

  /*
   The final script with methods for the controls
   and setting cookies with the current position.
   (Using client side cookies, too late for server side,
   already out of the header because
   I'm not going to buffer results!)
  */
  string lastScript( void )
  {
    ocString script = "<script type='text/javascript'>\n";
    script += "var dom = new openDom(); ";
    script += "dom.setCookie( 'scroll',''); ";
    script += "function goBack(){";
    script += "  dom.setCookie( 'currentPos', '";
    script.append(currentPos);
    script += "');";
    script += "  dom.setCookie( 'rowCount', '";
    script.append(rowCount);
    script += "');";
    script += "  dom.setCookie( 'realAmount', '";
    script.append(realAmount);
    script += "');";
    script += "  dom.setCookie( 'direction', '-1');";
    script += "  dom.setCookie( 'scroll','true'); ";
    script += "  var uri = new String(document.location.href);";
    if( args.length() )
    {
      script += "  uri += '";
      script += args;
      script += "';";
    }
    script += "  location.href = uri;";
    script += "}";
    script += "function goForward(){";
    script += "  dom.setCookie( 'currentPos', '";
    script.append(currentPos);
    script += "');";
    script += "  dom.setCookie( 'rowCount', '";
    script.append(rowCount);
    script += "');";
    script += "  dom.setCookie( 'realAmount', '";
    script.append(realAmount);
    script += "');";
    script += "  dom.setCookie( 'direction', '1');";
    script += "  dom.setCookie( 'scroll','true'); ";
    script += "  var uri = new String(document.location.href);";
    if( args.length() )
    {
      script += "  uri += '";
      script += args;
      script += "';";
    }
    script += "  location.href = uri;";
    script += "}</script>\n";
    return script;
  }

  // destructor
  ~vListManager()
  {
    ;
  }

};
#endif
