/*
  metaMedia.js

*/
function parsePlayer()
{
  var played = document.getElementById("mediaController");
  var fileBox = document.getElementById("mediaFile");
  var widthBox = document.getElementById("mediaWidth");
  var heightBox = document.getElementById("mediaHeight");
  
  if( played )
  {
    var innerContent = played.innerHTML;
    var arry = innerContent.split("\"");
    for( var i=0; i< arry.length; i++ )
    {
      switch( i )
      {
        case 5:  // media
        fileBox.value = arry[i];
        break;
        case 7: // width
        widthBox.value = arry[i];
        break;
        case 9: // height:
        heightBox.value = arry[i];
        break;
        default:
        break;
      }
    }
  }
}
function setPlay()
{
  var played = document.getElementById("mediaController");
  var fileBox = document.getElementById("mediaFile");
  var widthBox = document.getElementById("mediaWidth");
  var heightBox = document.getElementById("mediaHeight");
  if( played )
  {
    var parsedContent = "";
    var innerContent = played.innerHTML;
    var arry = innerContent.split("\"");
    //alert(  innerContent );
    for( var i=0; i< arry.length; i++ )
    {
      switch( i )
      {
        case 0:
        parsedContent += arry[i];
        break;
        case 5:  // media
        parsedContent +=  "\"" + fileBox.value; //alert (  fileBox.value );
        break;
        case 7: // width
        parsedContent += "\"" + widthBox.value; //alert (  widthBox.value );
        break;
        case 9: // height:
        parsedContent += "\"" + heightBox.value; //alert (  heightBox.value );
        break;
        default:
        parsedContent += "\"" +  arry[i];
        break;
      }
    }
    //alert ( parsedContent );
    played.innerHTML = parsedContent;
   
    // now actualy run it
    evalAry = parsedContent.split("(");
    if( evalAry.length == 2 )
    { 
      eevalAry = evalAry[1].split(")");
      if( eevalAry.length == 2 )
      {
        eval( "playMedia( " + eevalAry[0] + ")" );
      }
    }
  }
}
function playMedia( id, file, boxWidth, boxHeight )
{
  var path = "../media/";
  if( file.search(/[Ff][Ll][Vv]/) < 0 ) // handle a bug in the player
  {
    path = "./media/";
  }
  path += file;
  var divId = "player";
  divId += id;
  var player = document.getElementById(divId);
  if( player )
  {
    var s1 = new SWFObject("scripts/flvplayer.swf","single",boxWidth,boxHeight,"7");
    s1.addParam("allowfullscreen","true");
    s1.addParam("menu","false");
    s1.addVariable("file",path);
    s1.write(divId);
  }
}