/*
   openDOM object
   ==========================
    An application Independent Client DOM Abstraction.
    -------------------------------------------------
    - This attempts to resolve many of the
        inconsistancies between Netscape and MSIE.
    - Also performs other useful functions (as I think of them.)
    ------------------------
    2002-2007 w3 Systems Development (w3sys.com)
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
  this.isIE=false;
  // function parses the cookies
  this.parseCookies = openDom_parseCookies;
  this.setCookie = openDom_setCookie;
  this.unsetCookie = openDom_unsetCookie;
  this.hasCookie = openDom_hasCookie;
  this.getArg = openDom_getArg;
  this.getX = openDom_getX; 
  this.getY = openDom_getY; 
  this.windowWidth = openDom_windowWidth;
  this.windowHeight = openDom_windowHeight;

  // test for Client Version
  if ( parseInt(navigator.appVersion) >= 4 )
  {
    var exp = new RegExp( "^Microsoft","i" );  
    if( !exp.test(navigator.appName) )    
    { 
      this.setPosition = Gecko_setPosition;      
      this.setStylesheetStyle = Gecko_setStylesheetStyle;
      this.getElementById = Gecko_getElementById;
      this.showTableElement = Gecko_showTableElement;
      this.hideTableElement = Gecko_hideTableElement;
      this.wrapSelection = Gecko_wrapSelection;
      this.wrapSelectedRanges = Gecko_wrapSelectedRanges;
      this.putAtCursor = Gecko_putAtCursor;
      this.positionLeft = Gecko_positionLeft;
      this.positionTop = Gecko_positionTop;
      this.setOpacity = Gecko_setOpacity;
      this.editMode = Gecko_editMode;
      this.newXMLHttpRequest = Gecko_newXMLHttpRequest;
      this.XMLAttribute = Gecko_XMLAttribute;
      this.addListener = Gecko_addListener;
    }
    else 
    {
      this.isIE = true;
      this.setPosition = IE_setPosition;
      this.setStylesheetStyle = IE_setStylesheetStyle;
      if( navigator.userAgent.indexOf("Mac") == -1 )
      {
        this.getElementById = IE_getElementById;
      }
      else
      {
        // The DOM compliant version, 
        //  which later IE versions support
        this.getElementById = Gecko_getElementById;
      }      
      this.showTableElement = IE_showTableElement;
      this.hideTableElement = IE_hideTableElement;
      this.wrapSelection = IE_wrapSelection;
      this.wrapSelectedRanges = IE_wrapSelectedRanges;
      this.putAtCursor = IE_putAtCursor;
      this.positionLeft = IE_positionLeft;
      this.positionTop = IE_positionTop;
      this.setOpacity = IE_setOpacity;
      this.editMode = IE_editMode;
      this.newXMLHttpRequest = IE_newXMLHttpRequest; 
      this.XMLAttribute = IE_XMLAttribute;  
      this.addListener = IE_addListener;   
    }
  }
}
function openDom_windowWidth()
{
  var ret=0;
  if(window.innerWidth) ret = window.innerWidth;
  else ret = document.body.clientWidth;
  return ret;
}
function openDom_windowHeight()
{ 
  var ret=0;
  if(window.innerHeight) ret = window.innerHeight;
  else ret = document.body.clientHeight;
  return ret
}

// get an argument from the query (search) string of the URL
function openDom_getArg( argName )
{
  var ret = new String("");
  var qs = new String( document.location.search );
  if(qs.length)
  {
    qs = qs.substr(1); // get rid of ?
    var ary = qs.split("&");
    for( var i=0;i<ary.length;i++ )
    {
      var argval = new String(ary[i]);
      if( argval.length )
      {
        var argvalAry = argval.split("=");
        if(argvalAry.length == 2 && argvalAry[0] == argName )
        {
          ret = argvalAry[1];
        }        
      }
    }
  }
  return ret;
}

function Gecko_addListener(obj,evt,func,useCapture)
{
  obj.addEventListener(evt, func, useCapture);
}
function IE_addListener(obj,evt,func,useCapture)
{
  // this is as close to I can come to duplicating the 
  // mozilla capture functionality, NOT EXACT! 
  // Propagation is from opposite direction with IE. (window->children)
  if( useCapture )
  {
    obj.attachEvent("on"+evt, func );
  }
  else
  {
    document.attachEvent("on"+evt, func );
  }
}
function Gecko_XMLAttribute(xmlnode,attName)
{
  return xmlnode.attributes[attName];
}
function IE_XMLAttribute(xmlnode,attName)
{
  return xmlnode.attributes.getNamedItem(attName);
}

function Gecko_newXMLHttpRequest()
{
  return new XMLHttpRequest();
}
function IE_newXMLHttpRequest()
{
  return new ActiveXObject("Microsoft.XMLHTTP");
}

function Gecko_sendXMLHttpRequest(xdoc,handler)
{
  return new XMLHttpRequest();
  xdoc.onload=nchangeBoard;
  xdoc.send(null); 
}

var clientHandler;
function IE_sendXMLHttpRequest(xdoc,handler)
{
  xdoc.onreadystatechange=IE_XMLResponseHandler;
  clientHandler = handler;
  xdoc.send();
}
function IE_XMLResponseHandler()
{
  // MSIE only logic   
  if(xdoc && xdoc.readyState == 4) 
  {     
    clientHandler();
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
        // Stupid IE! - a clutsy workaround
        alert( "In order to add to the page link, " +
               "you must select some text in the editor. " +
               "The added content will appear at " +
               "the end of the selection." );
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
  // works only for text boxes and text areas!
  var selStart = ctrl.selectionStart;
  var selEnd = ctrl.selectionEnd;
  var txt = ctrl.value;
  var start = txt.substring(0,selStart);
  var contained = txt.substring(selStart, selEnd);
  var end = txt.substring(selEnd, txt.length);
  ctrl.value = start + left + contained + right + end;
}
function IE_wrapSelectedRanges( wnd, left, right )
{
}
function Gecko_wrapSelectedRanges( frm, left, right )
{
  // works for browser windows
  // query the selection
  var wnd = frm.contentWindow;
  var sel = wnd.getSelection();
  

  var rangeCount = sel.rangeCount;

  // get the start and end ranges
  var rangeStart = sel.getRangeAt(0);
  var rangeEnd = sel.getRangeAt(rangeCount-1);
  range = wnd.document.createRange(); 
  range.setStart(rangeStart.startContainer,0);

  range.setEnd(rangeStart.endContainer,sel.toString.length);
  var newNode = wnd.document.createElement("b");
  range.surroundContents(newNode);
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
function openDom_unsetCookie( key, path, domain )
{
  this.setCookie( key, "", "Thu, 01-Jan-1970 00:00:00 GMT", path, domain );
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
  this.getElementById( Id ).style.pixelLeft = x;
  this.getElementById( Id ).style.pixelTop = y;
}

// Netscape version 6 dynamic positioning
function Gecko_setPosition( Id, x, y )
{
  var X = new String(x);
  var Y = new String(y);
  X += "px";
  Y += "px";
  this.getElementById( Id ).style.left = X;
  this.getElementById( Id ).style.top = Y;
}

// IE dynamic positioning
function IE_positionLeft( Id )
{
  return this.getElementById( Id ).style.pixelLeft;  
}
function IE_positionTop( Id )
{
  return this.getElementById( Id ).style.pixelTop;  
}

// Netscape version 6 dynamic positioning
function Gecko_positionLeft( Id )
{
  var n = new String( this.getElementById( Id ).style.left )
  return n.replace(/[a-z,A-Z]*/gi, "");  
}
function Gecko_positionTop( Id )
{
  var n = new String( this.getElementById( Id ).style.top);
  return n.replace(/[a-z,A-Z]*/gi, "");  
}

// IE dynamic positioning
function openDom_getX( obj )
{
  var x = 0;
  var traverse = obj;
  while( traverse && traverse.tagName && traverse.tagName != "BODY" )
  {
    x += traverse.offsetLeft;
    traverse = traverse.offsetParent;
  }
  return x;  
}
function openDom_getY( obj )
{  
  var y = 0;
  var traverse = obj;
  while( traverse && traverse.tagName && traverse.tagName != "BODY" )
  {
    y += traverse.offsetTop;
    traverse = traverse.offsetParent;
  }
  return y; 
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

// Mozilla styles
function Gecko_setStylesheetStyle( name, style, value )
{

  for(i=0; i < document.styleSheets.length; i++)
  {
    for( ii=0; ii<document.styleSheets[i].cssRules.length; ii++ )
    {
     
      if( document.styleSheets[i].cssRules[ii].selectorText &&
          document.styleSheets[i].cssRules[ii].selectorText.valueOf() == name.valueOf() )
      {
        document.styleSheets[i].cssRules[ii].style[style] = value;
        break;
      }
      
    }
  } 
}

 function IE_showTableElement ( Id )
{
  var Elem = this.getElementById(Id);
  if( Elem && Elem.style )
  Elem.style.display = "block";
}
function Gecko_showTableElement ( Id )
{
  var Elem = this.getElementById(Id);
  if( Elem && Elem.style )
  Elem.style.display = "table-cell";
}
function IE_hideTableElement( Id )
{
  var Elem = this.getElementById(Id);
  if( Elem && Elem.style )
  Elem.style.display =  "none";
}
function Gecko_hideTableElement( Id )
{
  var Elem = this.getElementById(Id);
  if( Elem && Elem.style )
  Elem.style.display =  "none";
}


function Gecko_setOpacity( elem, degree )
{
  elem.style.MozOpacity = (degree/100);
}
function IE_setOpacity( elem, degree )
{
  elem.filters.alpha.opacity = degree;
}

function Gecko_editMode( wnd )
{
  wnd.document.designMode = "on";
}

function IE_editMode( wnd )
{
  wnd.document.body.contentEditable=true;
}
  
/*
 Netscape Version 4.x won't be done 
 It's too disimilar from the W3C DOM standard
 Very few are now using it.
 ----
 Intend to begin supporting Opera and 
 more fine-grained browser detection overall.
*/

