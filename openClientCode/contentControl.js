var ctrlId;
var ctrl;
var attrs;
var relativePath;
var txtarea;
var frm;
var richTools;
var srcTools;
var foreBack;

/* TextArea methods */
function wrapSelection(left, right)
{
  myDom.wrapSelection(txtarea, left, right);
  ctrlChanged(txtarea);
}

function wrapSelectionWithLink(attr)
{
  var my_link = prompt("Enter URL:","http://");  
  if (my_link != null)
  {
    var left="<a ";
    if(attr)left += attr;
    left += " href='";
    left += my_link;
    left += "'>";
    var right="</a>";
    wrapSelection(left, right);
  }
  return;
}

function placeImage(attr,relPath)
{
  // save relPath and attributes for call back 
  attrs=attr;
  if( relPath ) relativePath = relPath;
  else relativePath = "/images/";
  var imgPick = "imagePick.cgi?relPath=";
  imgPick += relativePath;
  window.open(imgPick,"_blank",
              "scrollbars=yes,toolbar=no,menubar=no,location=no");
}
function placeImageCallback( link, attrs )
{

  if( editDom && editDom.richMode == true)
  {
    imageCallback( link ); 
  }  
  else
  {
    var image = "<img ";
    if(attrs)image += attrs;
    image += " alt='' ";  
    image += " src='";
    image += link;
    image += "'>"; 
    // Using id saved for call back
    putAtCursor(image);
  }
}
function putAtCursor(text)
{
  myDom.putAtCursor(txtarea, text);
  ctrlChanged(txtarea);
}

/* New Methods */

function richView()
{
  editDom.setmode(frm);
  txtarea.style.display="none";
  srcTools.style.display="none";
  frm.style.display="block";
  richTools.style.display="block";
}
function sourceView()
{
  editDom.setmode(txtarea); 
  frm.style.display="none";
  richTools.style.display="none"; 
  txtarea.style.display="block"; 
  srcTools.style.display="block";     
}
// Mew 8/20/2005 resource linking

function getResource()
{
  window.open("resourcesPick.cgi","_blank",
              "scrollbars=yes,toolbar=no,menubar=no,location=no");
}
function getResourceCallback(my_link)
{
  if (my_link != null)
  {
    editDom.createLink("resources/"+my_link);
  }
  return;
}
function makeLink()
{
  var my_link = prompt("Enter URL:","http://");
  if (my_link != null)
  {
    editDom.createLink(my_link);
  }
  return;
}
function fmtblock( ctrl )
{
  fmt = ctrl.value;
  editDom.formatblock(fmt);
}
function fntsize( ctrl )
{
  sz = ctrl.value;
  editDom.fontsize(sz);
}
function undo()
{
  editDom.undo();
}
function redo()
{
  editDom.redo();
}
function makeBold()
{
  editDom.bold();
}
function makeItalic()
{
  editDom.italic();
}

function underline()
{
  editDom.underline();
}
function image(relPath)
{
  // var my_link = prompt("Enter Image URL:","");
  // editDom.insertImage(my_link); // for testing, or low budget
  
  // save relPath for call back   
  if( relPath ) relativePath = relPath;
  else relativePath = "/images/";
  var imgPick = "imagePick.cgi?relPath=";
  imgPick += relativePath;  
  window.open(imgPick,"_blank",
    "scrollbars=yes,toolbar=no,menubar=no,location=no");
    
}
function imageCallback( link )
{
  editDom.insertImage(link);
}

function unorderedlist()
{
  editDom.unorderedlist();
}
function orderedlist()
{
  editDom.orderedlist();
}
function justifyright()
{
  editDom.justifyright();
}
function justifyleft()
{
  editDom.justifyleft();
}
function justifycenter()
{
  editDom.justifycenter();
}
function outdent()
{
  editDom.outdent();
}
function indent()
{
  editDom.indent();
}
function forecolorSel()
{
  foreBack = "fore";
  window.open("colors.html","colors", "scrollbars=yes,toolbar=no,menubar=no,location=no,width=200,height=250");
}
function backcolorSel()
{
  foreBack = "back";
  window.open("colors.html","colors", "scrollbars=yes,toolbar=no,menubar=no,location=no,width=200,height=250");
}
function setColor(color)
{
  if( foreBack == "fore" )
  {
    forecolor(color);
  }
  else
  {
    backcolor(color);
  }
}

function forecolor(color)
{
  editDom.forecolor(color);
}
function backcolor(color)
{
  editDom.backcolor(color);
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
function jopen( url, attr, nm )
{
  var dattr="scrollbars,resizable,width=500,height=400";
  var dname="_child";
  if( attr ) dattr=attr;
  if( nm ) dname=nm;    
  window.open( url, dname, dattr );
  if ( win ) win.focus();
}
function jcontrol(id,url)
{
  // save id for call back
  ctrl = myDom.getElementById(id);  
  window.open(url, "ctrlaid", "scrollbars,resizable,toolbar=no,menubar=no,location=no,width=500,height=400");
}
function placeTagCallback( value )
{
  // Using id saved by jcontrol for call back
  ctrl.value = value;
  ctrlChanged( ctrl );
}
function doExit()
{
  // refresh the opener
  window.opener.document.forms[0].submit();
  //window.opener.location.reload(true);
   
  //var uri = window.opener.location;
  //window.opener.location = uri;
  
  // go away
  window.close();
}
