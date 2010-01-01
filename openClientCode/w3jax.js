/*
  w3jax.js
   Code does xml http request for netscape, ie.
   Handles completion through ajaxCallback if defined
   (or not if not).

*/

var hdoc = new openDom();
var xdoc = hdoc.newXMLHttpRequest();
var ajaxCallback = null;

function nxResponse()
{
  if(xdoc)
  {
    if( typeof(ajaxCallback) == "function" )
    {
      if( xdoc.responseXML )
      {
        ajaxCallback( xdoc.responseXML, xdoc.responseText );
      }
      else if( xdoc.responseText )
      {
        ajaxCallback( "<no_xml/>", xdoc.responseText );
      }
    }
  } 
}

function Response()
{
  if(xdoc && xdoc.readyState == 4)
    nxResponse(); 
}

function ajaxCall( xmlURL, xmlData )
{ 
  if( xdoc )
  {
    xdoc.open( "POST", xmlURL, true );
    if(hdoc.isIE)
    {      
      xdoc.onreadystatechange=Response;
      if( xmlData )
        xdoc.send(xmlData);
      else
        xdoc.send();
    }  
    else
    { 
      xdoc.onload=nxResponse;
      if( xmlData )
        xdoc.send(xmlData);
      else
        xdoc.send(null);
    }
  } 
}





