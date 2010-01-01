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
function pickTag( tag )
{
  // scripts are accessible
  opener.placeTagCallback(tag);
  window.close();
}



