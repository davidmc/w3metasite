/*
   openDOM object
   ==========================
    An application Independent Client DOM Abstraction.
    -------------------------------------------------
    - This attempts to resolve many of the
        inconsistancies between Netscape and MSIE.
    - Also performs other useful functions (as I think of them.)
    ------------------------
    2002-2003 w3 Systems Development (w3sys.com)
    Author David McCombs. (davidmc@w3sys.com)

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
  this.hasCookie = openDom_hasCookie;
  // test for Client Version
  if ( parseInt(navigator.appVersion) >= 4 )
  {

    if ( navigator.appName == 'Netscape' )
    {
      this.setPosition = Gecko_setPosition;
      this.setStylesheetStyle = Gecko_setStylesheetStyle;
      this.getElementById = Gecko_getElementById;
      this.showTableElement = Gecko_showTableElement;
      this.hideTableElement = Gecko_hideTableElement;
      this.wrapSelection = Gecko_wrapSelection;
      this.putAtCursor = Gecko_putAtCursor;
    }
    else // aren't there any other browsers out there? A: opera, konqueror, amaya
    {
      this.setPosition = IE_setPosition;
      this.setStylesheetStyle = IE_setStylesheetStyle;
      this.getElementById = IE_getElementById;
      this.showTableElement = IE_showTableElement;
      this.hideTableElement = IE_hideTableElement;
      this.wrapSelection = IE_wrapSelection;
      this.putAtCursor = IE_putAtCursor;
    }
  }
}

function IE_putAtCursor( ctrl, text )
{
  var doc = ctrl.document;
  var sel = doc.selection;
  if( sel )
  {

    var range = sel.createRange()
    if( range )
    {
      var selText = new String(range.text);
      if(selText.length)
      {
        range.text = selText + text;
      }
      else
      {
        alert( "In order to add to the page link, " +
               "you must select some text in the editor. " +
               "The added content will appear at the end of the selection." );
      }
    }
    else
    {
      alert( "no range to sel" );
    }
  }
  else
  {
    alert( "no selection");
  }
}

function Gecko_putAtCursor( ctrl, text )
{
  var selStart = ctrl.selectionStart;
  var txt = ctrl.value;
  var start = txt.substring(0,selStart);
  var end = txt.substring(selStart, txt.length);
  ctrl.value = start + text + end;
}

function IE_wrapSelection( ctrl, left, right )
{
  var doc = ctrl.document;
  var text = doc.selection.createRange().text;
  if(text && text != "" )
  {
    doc.selection.createRange().text = left + text + right;
  }
}
function Gecko_wrapSelection( ctrl, left, right )
{
  var selStart = ctrl.selectionStart;
  var selEnd = ctrl.selectionEnd;
  var txt = ctrl.value;
  var start = txt.substring(0,selStart);
  var contained = txt.substring(selStart, selEnd);
  var end = txt.substring(selEnd, txt.length);
  ctrl.value = start + left + contained + right + end;
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

function openDom_hasCookie( key )
{
  return typeof this.parsedCookies[key] == "object";
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
function IE_getElementById( id, otherDoc )
{
  if( otherDoc )
  {
    return otherDoc.all[id];
  }
  else
  {
    return document.all[id];
  }
}

// Netscape 6 element by id
function Gecko_getElementById( id, otherDoc )
{
  if( otherDoc )
  {
    return otherDoc.getElementById(id);
  }
  else
  {
    return document.getElementById(id);
  }
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
function IE_showTableElement ( Id )
{
  this.getElementById(Id).style.display = "block";
}
function Gecko_showTableElement ( Id )
{
  this.getElementById(Id).style.display = "table-cell";
}
function IE_hideTableElement( Id )
{
  this.getElementById( Id ).style.display = "none";
}
function Gecko_hideTableElement( Id )
{
  this.getElementById( Id ).style.display = "none";
}

/*
 Netscape Version 4.x will not be done - NS4 is dying
*/
