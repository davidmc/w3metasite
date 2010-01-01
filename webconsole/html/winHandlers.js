function jopen( url, attr, nm )
{
  var dattr="scrollbars,resizable,width=740";
  var dname="_child";
  if( attr ) dattr=attr;
  if( nm ) dname=nm;    
  var win = window.open( url, dname, dattr );
  if ( win ) win.focus();
}


function doExit()
{
  // refresh the opener
  window.opener.document.forms[0].submit();
  window.close();
}
