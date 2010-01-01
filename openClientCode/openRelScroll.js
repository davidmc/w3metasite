  /*
   openScroll.js - scrollBox object
   ==========================
    An browser independent scroll box abstraction.
    depends on openDom.js

    2003 w3 Systems Development (w3sys.com)
    Author David McCombs. (davidmc@w3sys.com)

*/

// constructor
function scrollBox( width, height, instanceName )
{ 
  // openDom instance
  this.dom = new openDom();
  // properties
  this.instanceName=instanceName;
  this.top=0;
  this.left=0;
  this.width=width;
  this.height=height;
  this.dtimeout=50;
  this.timeout=this.dtimeout;
  this.samount = 1;
  // methods
  this.startScroll = scrollBox_startScroll;
  this.endScroll = scrollBox_endScroll;
  this.go = scrollBox_go;
  this.slide = scrollBox_slide;    
}

function scrollBox_go( )
{  
  this.innerHeight = this.dom.getElementById("myScrollTest").offsetHeight;  
  this.currentTop = new Number( this.height ); // this.innerHeight );
  this.slide();
}
function scrollBox_slide()
{    
  this.currentTop -= this.samount;
  if( this.currentTop < -(this.innerHeight+10) ) this.currentTop = this.height;
  var currentLeft = this.dom.positionLeft("myScrollTest");
  this.dom.setPosition("myScrollTest",currentLeft,this.currentTop);
  var cmd = new String( this.instanceName );
  cmd += ".slide()";
  setTimeout(cmd,this.timeout);
}

function scrollBox_startScroll(  )
{
  document.write("<" + "div id=\"myScroll\"" +                 
                " style=\"width: " + this.width + 
                "; height: " + this.height + 
                "; overflow:hidden;" +                 
                " \"" +
                " onMouseOver='" + 
                this.instanceName + ".timeout=" + this.timeout*2 + "' onMouseOut='" +
                this.instanceName + ".timeout=" + this.dtimeout + "'>" + 
                "<div id=\"myScrollTest\" style=\"position: relative; width:" + this.width + 
                ";\">" );
}

function scrollBox_endScroll( )
{
  document.write( "</div><" + "/" + "div" + ">");
}
