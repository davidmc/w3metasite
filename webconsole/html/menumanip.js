var xdoc = myDom.newXMLHttpRequest();
var xmlURL;
function up(id)
{ 
  mimove(id,"u");
}
function bottom(id)
{ 
  mimove(id,"b"); 
}
function left(id)
{ 
  mimove(id,"l"); 
}
function right(id)
{ 
  mimove(id,"r"); 
}
function mimove(id,dir)
{
  xmlURL = "move_mi_svc.cgi?mid=";
  xmlURL += id;
  xmlURL += "&dir=" + dir;
  xdoc.open( "POST", xmlURL, false );
  xdoc.send(null);
  var ixml = xdoc.responseXML;
  if(ixml) 
  {
    var nodes = ixml.getElementsByTagName("menu_item_movement");
    var node = nodes[0];
    if( node )
    {    
      var result = myDom.XMLAttribute(node,"result").value;
      var reason = myDom.XMLAttribute(node,"reason").value;
      if( result == "OK" )
      {
        document.location.href = document.location.href; 
      }
      else
      {
        var note = result;
        note += " reason: ";
        node = reason;
        alert(note);
      }
    }
  }
  else
  {
    alert("no service: " + xmlURL  );
  }
}
