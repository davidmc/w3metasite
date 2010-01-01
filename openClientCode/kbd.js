/*
   kbd methods and miniDom object
   ==========================
    
    2005 w3 Systems Development (w3sys.com)
    Author David McCombs. (davidmc@w3sys.com)
*/

// miniDom constructor

function miniDom()
{

  this.isIE=false;
  this.focusedCtrl;
  
  // test for Client Version
  if ( parseInt(navigator.appVersion) >= 4 )
  {
    var exp = new RegExp( "^Microsoft","i" ); 
    this.getElementById = miniDom_getElementById; 
    this.insertAtEnd = minDom_insertAtEnd;
    this.deleteAtEnd = minDom_deleteAtEnd;
    if( !exp.test(navigator.appName) )    
    { 
      this.addListener = Gecko_addListener;
    }
    else 
    {
      this.isIE = true;  
      this.addListener = IE_addListener; 
    }
  }
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

function minDom_insertAtEnd( text )
{
  if( this.focusedCtrl )
  {

    var val = this.focusedCtrl.value;
    val += text;
    this.focusedCtrl.value = val;
  }      
}

function minDom_deleteAtEnd( text )
{
  if( this.focusedCtrl )
  {
    var val = this.focusedCtrl.value;
    if( val.length ) val = val.substr(0,val.length-1);
    this.focusedCtrl.value = val;
  }      
}

// ALL 6+ element by id
function miniDom_getElementById( id )
{  
  return document.getElementById(id);  
}

var mDom = new miniDom();
var caps = false;  

/*
 the methods
*/
function ky(char)
{
  var val = new String(char);
  if( caps ) val = val.toUpperCase();
  mDom.insertAtEnd(val);
} 

function kySpace()
{
  mDom.insertAtEnd(" ");
} 
function kyTab()
{
  mDom.insertAtEnd("\t");
} 
function kyDel()
{
  mDom.deleteAtEnd();
} 
function kyEnter()
{
  var frm = document.forms[0];
  if( frm ) frm.submit();
} 
function kyShift()
{
  caps = caps?false:true;
}

function setFocus(mEvent)
{ 
  if( !mDom.isIE )
  {     
    mDom.focusedCtrl = mEvent.target;   
  }
  else
  {
    mDom.focusedCtrl = window.event.srcElement;    
  } 
}

function focusFirstControl()
{
  var frm = document.forms[0];
  if( frm )
  {
    // iterate throuugh form controls, find the first usable one to focus.
    for(i in frm.elements)
    { 
      if( frm.elements[i] &&  frm.elements[i].tagName )
      { 
        var tagName = frm.elements[i].tagName;        
        if( tagName == "INPUT" && ( frm.elements[i].type == "text" || 
                                    frm.elements[i].type == "password" ) ||
            tagName == "TEXTAREA" )
        {   
          if( tagName == "INPUT" && frm.elements[i].readOnly )// check readonly property
            continue;               
          if( frm.elements[i].disabled ) // is it disabled
            continue;
          if( frm.elements[i].offsetWidth == 0 ) // best visibility indicator I could find
            continue;      

          if( !mDom.focusedCtrl )
          {          
            frm.elements[i].focus();
            mDom.focusedCtrl = frm.elements[i];          
          }
          
          mDom.addListener( frm.elements[i], "focus", setFocus, true );
        }      
      }      
    }  
  } 
}  
