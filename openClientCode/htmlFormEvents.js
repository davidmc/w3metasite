// for change tracking on user input
function ctrlChanged( ctrl )
{  
  updates[ctrl.name] = ctrl.name;
}

function doUpdate()
{
  myDom.setCookie( "mode","u" );  
  editDom.formPrep();
  htmlFixup(txtarea); // defined in eDom.js
  var directions = updateList();
  directions += "<mode>u</mode>"; 
  document.forms["uiForm"].xmldirections.value=directions;
  document.forms["uiForm"].submit();
}
function doInsert()
{  
  myDom.setCookie( "mode","i" );
  editDom.formPrep();
  htmlFixup(txtarea);  
  var directions = "<mode>i</mode>"; 
  document.forms["uiForm"].xmldirections.value=directions;
  document.forms["uiForm"].submit(); 
}
function doDelete()
{
  if( confirm( "Are you sure you want to delete this item?" ) )
  {
    var directions = "<mode>d</mode>"; 
    document.forms["uiForm"].xmldirections.value=directions;
    myDom.setCookie( "mode","d" );
    document.forms["uiForm"].submit();  
  }
}
function doReset()
{
  var directions = "<mode>r</mode>"; 
  document.forms["uiForm"].xmldirections.value=directions;
  myDom.setCookie( "mode","r" );
  document.forms["uiForm"].submit(); 
}
function doNew()
{
  var directions = "<mode>n</mode>"; 
  document.forms["uiForm"].xmldirections.value=directions;
  myDom.setCookie( "mode","n" );
  document.forms["uiForm"].submit(); 
}
function updateList()
{
  var retVal = new String();
  for( var name in updates )
  {
    retVal += "<change>" + name + "</change>\n";
  }  
  return retVal;
}



