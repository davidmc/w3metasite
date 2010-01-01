/* 
  eDom.js 
  
  Document Object Model for editing html documents  
  copyright (c) 2004 by David K. McCombs  
  
  W3 Systems Design
  davidmc@w3sys.com

*/
function emitRichEditor() // !OJO! deprecated, just use your own iframe with the blank page where you want it
{
  if ( navigator.appName == 'Netscape' )
  {
    document.write("<iframe id='richEditor'  src='editblank.html'> - NO IFRAME ABILITY! - </iframe>");
  }
  else
  {
    document.write("<iframe id='richEditor'> - NO IFRAME ABILITY! - </iframe>");
  }  
}

function eDom( editControl )
{
  this.formPrep = eDom_formPrep;
  this.setmode = eDom_setmode;
  this.transferdata = eDom_transferdata;
  this.setmode(editControl);

  
  // Editor emission
  var exp = new RegExp( "^Microsoft","i" );  
  if( !exp.test(navigator.appName) ) 
  {
    this.initialize = eDom_moz_initialize;
    this.backcolor  = eDom_moz_backcolor;     
  }
  else
  {
    this.initialize = eDom_msie_initialize;
    this.backcolor  = eDom_msie_backcolor;
             
  }  
  this.securityFixup = eDom_securityFixup; 
  
  // Edit Methods   
  this.execCommand = eDom_execCommand;
  this.bold = eDom_bold;
  this.italic = eDom_italic;
  this.insertImage = eDom_insertImage;
  this.createLink = eDom_createLink;
  this.undo = eDom_undo;
  this.redo = eDom_redo;
  this.underline = eDom_underline;  
  this.unorderedlist = eDom_unorderedlist;
  this.orderedlist = eDom_orderedlist;
  this.justifyright = eDom_justifyright;
  this.justifyleft = eDom_justifyleft;
  this.justifycenter = eDom_justifycenter;
  this.outdent = eDom_outdent;
  this.indent = eDom_indent;
  this.forecolor = eDom_forecolor;
  this.formatblock =  eDom_formatblock;
  this.fontname = eDom_fontname
  this.fontsize = eDom_fontsize;
  this.initialize();

}
function eDom_formPrep()
{
  if( this.richMode == true )
  {
    this.securityFixup();
    this.txtarea.value = this.doc.body.innerHTML;
  }  
  ctrlChanged( this.txtarea );
}
function eDom_setmode( editControl )
{
  if( editControl.tagName=="IFRAME" )
  {
    this.richMode = new Boolean(true);
    this.wnd = editControl.contentWindow;    
    this.doc = this.wnd.document;         
  }
  else
  {
    this.richMode = new Boolean(false);
    this.txtarea = editControl;    
  }
  if( this.txtarea && this.wnd )
  {
    this.transferdata();
  }
}
function eDom_transferdata()
{  
  if( this.richMode == true )
  {
    this.doc.body.innerHTML = this.txtarea.value;
    if( !myDom.isIE ) 
    {
      setTimeout(setEditable,500);      
    }
  }
  else
  {
    this.securityFixup();     
    this.txtarea.value = this.doc.body.innerHTML;
  }
}
function eDom_moz_initialize()
{
  if(this.richMode)
  {
    this.doc.designMode = "on";    
  }
}

function eDom_msie_initialize()
{
  if(this.richMode)
  {   
    try
    { 
      this.doc.designMode = "on";
      // this.wnd.location = "../editblank.html";
    }
    catch (e)
    {
      if( e.message=="Permission denied" )
      {
        // from probable refresh - force it!
        var loc = document.location.href;
        document.location.href = loc;
      }
    }
  }
}
function eDom_securityFixup()
{
  /* this is for a MSIE security patch that broke things,
    A knowledge-base entry tells how to fix, but not why!
  */
  if( myDom.isIE )
  {
    ifrm = myDom.getElementById("richEditor");
    this.wnd = ifrm.contentWindow;    
    this.doc = this.wnd.document;  
  }
}
function eDom_undo()
{
  this.execCommand('Undo');
}
function eDom_redo()
{
  this.execCommand('Redo');
}
function eDom_execCommand( cmdId, show, param )   
{
  this.securityFixup();
  this.doc.execCommand( cmdId, show, param);
}
function eDom_bold()
{
  this.execCommand('Bold', false, null);
}
function eDom_italic()
{
  this.execCommand('Italic', false, null);
}
function eDom_underline()
{
  this.execCommand('Underline', false, null);
}
function eDom_insertImage(imagePath)
{
  this.execCommand('InsertImage', false, imagePath);
}
function eDom_createLink(urlPath)
{
  this.execCommand('CreateLink', false, urlPath);
}
function eDom_createLink(urlPath)
{
  this.execCommand('CreateLink', false, urlPath);
}
function eDom_unorderedlist()
{
  this.execCommand('InsertUnorderedList', false, null);
}
function eDom_orderedlist()
{
  this.execCommand('InsertOrderedList', false, null);
}
function eDom_justifyright()
{
  this.execCommand('JustifyRight', false, null);
}
function eDom_justifyleft()
{
  this.execCommand('JustifyLeft', false, null);
}
function eDom_justifycenter()
{
  this.execCommand('JustifyCenter', false, null);
}
function eDom_outdent()
{
  this.execCommand('Outdent', false, null);
}
function eDom_indent()
{
  this.execCommand('Indent', false, null);
}
function eDom_forecolor( color )
{  
  this.execCommand('Forecolor', false, color);
}
function eDom_msie_backcolor ( color )
{
  this.execCommand('Backcolor', false, color);
}
function eDom_moz_backcolor ( color )
{
  this.execCommand('hilitecolor', false, color);
}
function eDom_formatblock ( fmt )
{
  this.execCommand('formatblock', false, fmt );
}
function eDom_fontname ( face )
{
   this.execCommand('fontsize', false, face ); 
} 
function eDom_fontsize ( sz )
{
   this.execCommand('fontsize', false, sz ); 
} 

// Global instance variables and event methods
var myDom = new openDom(); 
var editDom;     
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
function richEditSetup()
{
  thisForm=document.frmTest;
  frm = myDom.getElementById("richEditor");
  editDom = new eDom(frm);
  txtarea = myDom.getElementById("plainEditor");
  // decode the xfer-safe text
  htmlDecode(txtarea);
  editDom.txtarea = txtarea;
  richTools = myDom.getElementById("toolbar");
  srcTools = myDom.getElementById("srctoolbar");
  setTimeout(setRichContent,500)
}
function setRichContent()
{
  // var frm = myDom.getElementById("richEditor");
  // var txtarea = myDom.getElementById("richEditor");
  try
  {  
    frm.contentWindow.document.body.innerHTML=txtarea.value;
  }
  catch(e)
  {    
    setTimeout(setRichContent,500);
  }
}
function setEditable()
{
  try
  {
    frm.contentWindow.document.designMode = "on"; 
  }
  catch(e)
  {
    setTimeout(setEditable,500);
  }
}

// xhtml fixups
function xFixImgs( oContent )
{
  pContent = new String(oContent);
  nContent = new String("");
  var pos = pContent.search( /<img/i );
  if( pos == -1 )
  {
    nContent = pContent
  }
  else
  {
    // add initial content
    nContent += pContent.substr(0,pos)
    do
    {     
      // move on
      pContent =  pContent.substr(pos);
      var ePos = pContent.search(/>/)
      // there should always be one of these, check anyway
      if( ePos == -1 )
      {
        nContent += pContent;
        break;
      }
      var imgContent = pContent.substr(0,ePos)
            
      // put the image guts
      nContent += imgContent;
      
      // test for alt and '/', add xml compliance stuff 
      var altPos = imgContent.search(/alt/i);
      var monoPos = imgContent.search(/\/$/);
      if( altPos == -1 ) nContent += " alt='' ";
      if( monoPos == -1 ) nContent += " /";    
      
      // this will put string at '>...'
      pContent =  pContent.substr(ePos);
      pos = pContent.search( /<img/i );
      if( pos == -1 )
      {
        nContent += pContent;
      }
      else
      {
        nContent += pContent.substr(0,pos)
      } 
     
           
    } while( pos != -1 ) 
  }
  return nContent; 
}
function xFixBreaks( oContent )
{
  pContent = new String(oContent);
  nContent = new String("");
  var pos = pContent.search( /<br/i );
  if( pos == -1 )
  {
    nContent = pContent
  }
  else
  {
    // add initial content
    nContent += pContent.substr(0,pos)
    do
    {     
      // move on
      pContent =  pContent.substr(pos);
      var ePos = pContent.search(/>/)
      // there should always be one of these, check anyway
      if( ePos == -1 )
      {
        nContent += pContent;
        break;
      }
      var brContent = pContent.substr(0,ePos)
            
      // put the br guts
      nContent += brContent;
      
      // test for '/', add xml compliance  
      var monoPos = brContent.search(/\/$/);
      if( monoPos == -1 ) nContent += " /";    
      
      // this will put string at '>...'
      pContent =  pContent.substr(ePos);
      pos = pContent.search( /<br/i );
      if( pos == -1 )
      {
        nContent += pContent;
      }
      else
      {
        nContent += pContent.substr(0,pos)
      }      
           
    } while( pos != -1 ) 
  }
  return nContent; 
}

function htmlFixup(html_content)
{  
  // encode for safe transmission
  var content = new String(html_content.value);
  content = xFixImgs(content);
  content = xFixBreaks(content);
  html_content.value = content.replace(/\"/g,"`22;")
                              .replace(/\'/g,"`27;") 
                              .replace(/\+/g,"`29;")
                              .replace(/\</g,"`3c;")
                              .replace(/\>/g,"`3e;")
                              .replace(/\&/g,"`26;");   
                              
}
function htmlDecode(html_content)
{ 
  // decode the safe transmission
  var content = new String(html_content.value);
  html_content.value = content.replace(/\`22;/g,"\"")
                              .replace(/\`27;/g,"'") 
                              .replace(/\`29;/g,"+")
                              .replace(/\`3c;/g,"<")
                              .replace(/\`3e;/g,">")
                              .replace(/\`26;/g,"&");
                              
}
