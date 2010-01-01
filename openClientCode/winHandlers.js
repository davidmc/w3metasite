/*
  winHandlers.js

  D.K.McCombs ~ www.w3sys.com

 -- 5/30/06 no longer focuses on select controls

  OJO! assumes myDom instanced already 
*/
function focusFirstControl( formID )
{
  var frm = document.forms[formID];
  if( frm )
  {
    // iterate throuugh form controls, find the first usable one to focus.
    for(i in frm.elements)
    { 
      if( frm.elements[i] &&  frm.elements[i].tagName )
      { 
        var tagName = frm.elements[i].tagName;        
        if( tagName == "INPUT" && frm.elements[i].type == "text" || tagName == "TEXTAREA" )
        {   
          if( tagName == "INPUT" && frm.elements[i].readOnly )// check readonly property
            continue;               
          if( frm.elements[i].disabled ) // is it disabled
            continue;
          if( frm.elements[i].offsetWidth == 0 ) // best visibility indicator I could find
            continue;         
          frm.elements[i].focus();          
          break;           
        }      
      }      
    }  
  } 
} 
 
function winAlert( body )
{
  var dattr="scrollbars,resizable,width=550";
  var dname="alert_win";
        
  var win = window.open( "", dname, dattr );
  if ( win )
  {
     var html = "<html><body>"
     html += body;
     html += "</body></html>\n";
     win.document.write(html);
     win.focus();
  }
}
function jopen( url, attr, nm )
{
  var dattr="scrollbars,resizable";
  var dname="_child";
  if( attr ) dattr=attr;
  if( nm ) dname=nm;    

  var win = window.open( "", dname, dattr );
  if ( win && typeof(win.focus) != "undefined" )
  {
    scrnX = screen.availWidth;
    scrnY = screen.availHeight;
    winX = window.innerWidth?win.outerWidth:win.document.body.clientWidth;
    winY = window.innerWidth?win.outerHeight:win.document.body.clientHeight;
    win.moveTo(Math.round( (scrnX-winX)/2 ), Math.round( (scrnY-winY)/2 ) );
    win.location.href=url;
    win.focus();
  }
 
}
function doExit()
{
  myDom.setCookie( "mode","s" );
  var form;
  if( window.opener.document.forms.length )
  {
    form = window.opener.document.forms[0];
  }
  if( form )
  {
    if( form.xmldirections )
    {
      form.xmldirections.value="<mode>s</mode>";
    }
  }
  myDom.setCookie( "mode","s" );
  if( window.opener )
  {
    var uri = window.opener.location;
    var sep = new String("");
    if( window.opener.location.search.length == 0 )
    {
      sep = "?";
    }
    else
    {
      sep = "&";
    }
    if( form && form.elements["id"] && form.elements["id"].value && form.elements["id"].value != "0" )
    {
      uri += sep +  "id=" + form.elements["id"].value;
    }
    else if( form && form.elements["Id"] && form.elements["Id"].value  && form.elements["Id"].value != "0" )
    {
      uri += sep + "Id=" + form.elements["Id"].value;
    }
    window.opener.location = uri;
  }
  window.close();
}

var myDom = new openDom();
var updates = new Array();
var ctrlId;

function placeFile(id)
{
    // save id for call back  
  ctrlId = id;    
  window.open("filePick.cgi", 
    "_blank",
    "scrollbars=yes,toolbar=no,menubar=no,location=no");
}
function placeFileCallback( link )
{
  var ctrl = myDom.getElementById(ctrlId);
  ctrl.value = link;
  if( updates) updates[ctrl.name] = ctrl.name;    
}
function placeImage(id)
{
    // save id, relPath  and attributes for call back  
  ctrlId = id;    
  window.open("imagePick.cgi?noBasePath=true", 
    "_blank",
    "scrollbars=yes,toolbar=no,menubar=no,location=no");
}
function placeImageCallback( link )
{
  var ctrl = myDom.getElementById(ctrlId);
  ctrl.value = link;
  if( updates) updates[ctrl.name] = ctrl.name;    
}

function spellCheck(id)
{
  ctrlId = id;
  var ctrl = myDom.getElementById(ctrlId);
  var spellText = ctrl.value;
  var uri = new String( "SpellChecker.cgi?TextUnderTest=" );
  uri += spellText;
  window.open( uri, "_blank",
    "scrollbars=yes,toolbar=no,menubar=no,location=no");
}
function placeTextCallback( newText )
{
  var ctrl = myDom.getElementById(ctrlId);
  ctrl.value = newText;
  if( updates) updates[ctrl.name] = ctrl.name;
}

function placeTagCallback( value )
{
    // Using id saved by jcontrol for call back
  ctrl.value = value;
  if( updates) updates[ctrl.name] = ctrl.name; 
}
function jcontrol(id,url)
{
    // save id for call back
  ctrl = myDom.getElementById(id);  
  window.open(url, "ctrlaid", "resizable,toolbar=no,menubar=no,location=no,width=500,height=400");
}
function toggleVis( elemId )
{
    // used to hide and unhide form elements considered 'advanced'
  ctrl = myDom.getElementById(elemId);
  if( ctrl.style.display == "none" || !ctrl.style.display )
    ctrl.style.display = "block";
  else
    ctrl.style.display = "none"; 
}
function addChild( cgi, name)
{
    // add a child item to a combo box
  jopen(cgi,'resizable,width=400,height=300',name);
}
function updateChild(cgi, name, ctrl_id )
{
    // update the current selected combo box item
  var link=cgi;
  link += '?Id=';
  link += myDom.getElementById(ctrl_id).value;
  jopen(link,'resizable,width=400,height=300',name);
}
function addComboItem( comboID, valID, valLabel )
{
  var cbo = myDom.getElementById(comboID);
  if( cbo )
  {
    if( myDom.isIE )
    {
      cbo.add(  new Option( valLabel, valID ) );
    }
    else
    {
      cbo.add(  new Option( valLabel, valID ), null );
    }
    cbo.value=valID;
  }
}

/* NEW! pick actual selected item */
function selopen( url, attr, nm )
{
  /*
  ctrl = myDom.getElementById(nm);
  if(ctrl)
  {
  
  }
  */
  var dattr="scrollbars,resizable";
  var dname="_child";
  if( attr ) dattr=attr;
  if( nm ) dname=nm;
  var win = window.open( url, dname, dattr );
  if ( win ) win.focus();
}