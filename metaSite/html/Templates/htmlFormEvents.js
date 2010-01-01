// for change tracking on user input
function ctrlChanged( ctrl )
{
  updates[ctrl.name] =  ctrl.name;
}
function htmlFixup(html_content)
{
  var content = new String(html_content.value);
  html_content.value = content.replace(/\+/g,"`29;")
                              .replace(/\</g,"`3c;")
                              .replace(/\>/g,"`3e;")
                              .replace(/\&/g,"`26;");   
}
function htmlDecode(html_content)
{
  var content = new String(html_content.value);
  html_content.value = content.replace(/\`29;/g,"+")
                              .replace(/\`3c;/g,"<")
                              .replace(/\`3e;/g,">")
                              .replace(/\`26;/g,"&");   
}
function doUpdate()
{
  myDom.setCookie( "formaction","u" );
  myDom.setCookie( "changes", updateList() );
  htmlFixup(document.forms[0].content ); 
  document.forms[0].submit();
}

function doDelete()
{
  if( confirm( "Are you sure you want to delete this item?" ) )
  {
    myDom.setCookie( "formaction","d" );
    myDom.setCookie( "changes", updateList() );
    document.forms[0].submit();
  }
}

function doInsert()
{
  myDom.setCookie( "formaction","i" );
  myDom.setCookie( "changes", updateList() );
  document.forms[0].submit();
}
function doReset()
{
  myDom.setCookie( "formaction","r" );
  myDom.setCookie( "changes", "" );
  document.forms[0].submit();
}
function updateList()
{
  var retVal = new String();
  for( var name in updates )
  {
    retVal += name + '`';
  }
  retVal = retVal.substr( 0, retVal.length - 1 );
  return retVal;
}

