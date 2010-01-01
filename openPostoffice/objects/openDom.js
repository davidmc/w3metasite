/*
   openDOM object
   ==========================
    An application Independent Client DOM Abstraction.
    -------------------------------------------------
    - This attempts to resolve many of the
        inconsistancies between Netscape and MSIE.
    - Also performs other useful functions as I think of them.
    ------------------------
    2002 Open Connections LLC.
    Author David McCombs.
*/

// openCookie constructor
function openCookie( name, value )
{
  this.name = new String( name );
  this.value = new String( value );
  this.expires = new String();
  this.path = new String();
  this.domain = new String();
  this.secure = new String();
}

// openDom constructor
function openDom()
{
  // create a cookies collection
  this.cookies = new String( document.cookie );
  this.parsedCookies = new Array();
  this.cookieCount = 0;

  // function parses the cookies
  this.parseCookies = openDom_parseCookies;
  this.setCookie = openDom_setCookie;

  // test for Client Version
  if ( parseInt(navigator.appVersion) >= 4 )
  {
    if ( navigator.appName == 'Netscape' )
    {
      this.setPosition = Gecko_setPosition;
      this.setStylesheetStyle = Gecko_setStylesheetStyle;
      this.getElementById = Gecko_getElementById;
    }
    else // aren't there any other browsers out there?
    {
      this.setPosition = IE_setPosition;
      this.setStylesheetStyle = IE_setStylesheetStyle;
      this.getElementById = IE_getElementById;
    }
  }
}

function openDom_setCookie( key, value, expires, path, domain, secure )
{
  var cookie = new String("");
  cookie = key + "=" + escape(value);
  if( expires ) cookie +=  "; expires=" + expires;
  if( path ) cookie += "; path=" + path;
  if( domain ) cookie += "; domain=" + domain;
  if( secure ) cookie += "; secure=" + secure;
  document.cookie = cookie;
}

function openDom_parseCookies()
{
  var reVars = new RegExp(";","gi");
  var reVals = new RegExp("=","gi");
  var reBWS = new RegExp("^[ \t]*","gi");
  var reEWS = new RegExp("[ \t]*$","gi" );
  var cookieArray = this.cookies.split( reVars );
  this.parsedCookies = new Array();
  this.cookieCount = 0;
  for( var i=0; i < cookieArray.length; i++ )
  {
    var pair = new String( cookieArray[i] );
    var pairArray = pair.split( reVals );
    var label;

    if( pairArray.length == 2 )
    {
      key = new String( pairArray[0] );
      key = key.replace(reBWS,"");
      key = key.replace(reEWS,"");

      /*
         determine if this is a new cookie
         or params of the current one
      */
      if( key != "expires" &&
          key != "path" &&
          key != "domain" &&
          key != "secure" )
      {
        // new cookie
        label = new String( key );
        this.parsedCookies[label] = new openCookie( label, pairArray[1] );
        this.cookieCount++;
      }
      else
      {
        // assign extra parameters to the members of the cookie;
        this.parsedCookies[label][key] = pairArray[1];
      }
    }
  }
  return this.parsedCookies;
}

// IE element by id
function IE_getElementById( id )
{
  return document.all[id];
}
// Netscape 6 element by id
function Gecko_getElementById( id )
{
  return document.getElementById(id);
}

// IE dynamic positioning
function IE_setPosition( Id, x, y )
{
  document.all[Id].style.pixelLeft = x;
  document.all[Id].style.pixelTop = y;
}

// Netscape version 6 dynamic positioning
function Gecko_setPosition( Id, x, y )
{
  document.getElementById(Id).style.left = x;
  document.getElementById(Id).style.top = y;
}


// IE styles
function IE_setStylesheetStyle( name, style, value )
{
  for( i=0; i < document.styleSheets.length; i++ )
  {
    for( ii=0; ii<document.styleSheets[i].rules.length; ii++ )
    {
      if( name.valueOf() == document.styleSheets[i].rules[ii].selectorText.valueOf() )
      {
        document.styleSheets[i].rules[ii].style[style] = value;
        break;
      }
    }
  }
}

// Netscape version 6 styles
function Gecko_setStylesheetStyle( name, style, value )
{
  for( i=0; i < document.styleSheets.length; i++ )
  {
    for( ii=0; ii<document.styleSheets[i].cssRules.length; ii++ )
    {
      if( name.valueOf() == document.styleSheets[i].cssRules[ii].selectorText.valueOf() )
      {
        document.styleSheets[i].cssRules[ii].style[style] = value;
        break;
      }
    }
  }
}

// Netscape Version 4.x not done - hopefully it will die soon first

