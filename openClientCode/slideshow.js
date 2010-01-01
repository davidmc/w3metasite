// global variables    
var el=0;
var changetime=5000;    
var interval=50;    
var opacity = 0;
var opacityStep = 5;

// global objects
var dom = new openDom();
var intervalObject;
var ary;

// global functions
function slideShow()
{            
  ary = new Array();
  var divs = document.getElementsByTagName("div");      
  var pushPos = 0;            
  for( var i=0; i < divs.length; i++) // Kate Parser! --> 
  {
    if( divs[i].className == "slide" )
    {          
      // I would use ary.push but for MAC IE!
      ary[pushPos] = divs[i];
      pushPos++;
    }
  }
  if( ary.length )
  {                  
    setTimeout(slideNext,changetime);
    posLeft(); 
    fadeIn();         
    intervalObject = setInterval( "fadeIn()", interval );        
    ary[el].style.visibility="visible";
  }          
}
function slideNext()
{             
  ary[el].style.visibility="hidden"; 
  opacity = 0;    
  fadeIn(); 
  el ++;
  if( el == ary.length ) el = 0;
  posLeft();       
  ary[el].style.visibility="visible";            
  setTimeout(slideNext,changetime);
  stopInterval();
  intervalObject = setInterval( fadeIn, interval );
}
function posLeft( )
{
  var bestLeft = 20 + document.body.clientWidth/2
                  -ary[el].offsetWidth/2;
  if( bestLeft < 200 ) bestLeft = 200; // Kate Parser! -->
  id = ary[el].id;
  dom.setPosition( id, bestLeft, 200 );                           
}
function fadeIn( )
{           
  dom.setOpacity( ary[el], opacity );
  opacity += opacityStep;
  if( opacity > 100 )
  {
    opacity = opacityStep;
    stopInterval();        
  }
}
function stopInterval()
{      
  clearInterval(intervalObject);
  delete intervalObject;
} 