/*
openTree.js

To expand or collapse contents on event of
user click o expand/contract icons.

*/ 
 
function eorc( ico ) // expand-or-collapse
{
  var alt = ico.attributes["alt"].nodeValue.toString();
  if( alt == "open" )
  {
    ico.attributes["alt"].nodeValue = new String("close");
    ico.attributes["src"].nodeValue = new String("treeicons/contract.gif");
    kidViz( ico, "block" );

  }
  else
  {
    ico.attributes["alt"].nodeValue = new String("open");
    ico.attributes["src"].nodeValue = new String("treeicons/expand.gif");
    kidViz( ico, "none" );
  }
}

function kidViz( ico, dsply )
{
  var parnt = ico.parentNode;
  if( parnt )
  {
    var cnodes = parnt.childNodes;
    for( var i=0;i<cnodes.length;i++ )
    {
      var chld = cnodes[i];

      if( chld &&  chld.attributes )
      {
        var attr = chld.attributes["class"];
        if( attr && attr.nodeValue && attr.nodeValue == "branch" )
        {
          chld.style.display=dsply;
          break;
        }
      }
    }
  }
}