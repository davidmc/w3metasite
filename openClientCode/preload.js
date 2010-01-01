/* Pretty good preload images function stolen off the internet somewhere and modified to suit w3sys */

var arImages=new Array(); // global array

function preLoad() 
{
  var args = preLoad.arguments; 
  for(x=0; x < args.length; x++) 
  {
    arImages[x]=new Image();
    arImages[x].src=args[x];
  }
}
/*

Example use:
preLoad("pg_images/NewLogo.gif","pg_images/arrow.gif","pg_images/tabback.gif",
        "pg_images/TiltedBag.png","pg_images/tabstart.gif",
        "pg_images/tabsep.gif","pg_images/tabend.gif",
        "pg_images/blueleft.gif","pg_images/blueright.gif",
        "pg_images/bluetopleft.gif","pg_images/bluetopright.gif","pg_images/Go.gif",
        "pg_images/greenleft.gif","pg_images/greenright.gif","pg_images/greentopleft.gif",
        "pg_images/greentopright.gif",       
        "pg_images/menu_sep.gif","pg_images/bullet.gif");
*/ 