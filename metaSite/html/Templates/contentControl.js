var ctrlId;
var ctrl;
var attrs;
var relativePath;

function wrapSelection(id, left, right)
{
  var txtarea = myDom.getElementById(id);
  myDom.wrapSelection(txtarea, left, right);
  ctrlChanged(txtarea);
}
function wrapSelectionWithLink(id, attr)
{
  var my_link = prompt("Enter URL:","");
  if (my_link != null)
  {
    var left="<a ";
    if(attr)left += attr;
    left += " href='";
    left += my_link;
    left += "'>";
    var right="</a>";
    wrapSelection(id, left, right);
  }
  return;
}
function placeImage(id,attr,relPath)
{
  // save id, relPath  and attributes for call back  
  ctrlId = id;
  attrs=attr;
  if( relPath ) relativePath = relPath;
  else relativePath = "../";
  window.open("imagePick.cgi","_blank",
    "scrollbars=yes,toolbar=no,menubar=no,location=no");
}
function placeImageCallback( link )
{
  var image = "<img ";
  if(attrs)image += attrs;  
  image += " src='";
  image += relativePath;
  image += link;
  image += "'>";
  // Using id saved for call back
  putAtCursor(ctrlId,image);
}
function putAtCursor(id, text)
{
  var txtarea = myDom.getElementById(id);
  myDom.putAtCursor(txtarea, text);
  ctrlChanged(txtarea);
}
function mouseover(el)
{
  el.className = "raise";
}
function mouseout(el)
{
  el.className = "buttons";
}
function mousedown(el)
{
  el.className = "press";
}
function mouseup(el)
{
  el.className = "raise";
}
function jopen( url )
{
  window.open( url, "_child", "scrollbars,resizable,width=500,height=400");
}

function jcontrol(id,url)
{
  // save id for call back
  ctrl=id;
  window.open(url, "ctrlaid", "scrollbars,resizable,toolbar=no,menubar=no,location=no,width=500,height=400");
}
function placeTagCallback( value )
{
  // Using id saved by jcontrol for call back
  ctrl.value = value;
  ctrlChanged(ctrl);
}
