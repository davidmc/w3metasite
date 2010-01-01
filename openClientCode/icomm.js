/*
  icomm.js

  This file manages opener child communications.

  David McCombs
  W3 Systems Development
  davidmc@w3sys.com
*/

function pickImage( image )
{
  // scripts are accessible
  opener.placeImageCallback(image);
  window.close();
}
function pickfile( url )
{
  // scripts are accessible
  opener.placeFileCallback(url);
  window.close();
}

// new function to set arbitrary resources
function setResource( url )
{
  opener.getResourceCallback(url);
  window.close();
}

function showfile( url )
{
  // scripts are accessible
  var win = window.open( url, "view", 
                         "scrollbars,resizable,width=520,height=340" );
  if ( win ) win.focus();
}
function pickTag( tag )
{
  // scripts are accessible
  opener.placeTagCallback(tag);
  window.close();
}



