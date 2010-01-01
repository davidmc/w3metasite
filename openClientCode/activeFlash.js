function flashObj()
{
this.contentVersion = 7;
this.plugin = (navigator.mimeTypes && navigator.mimeTypes["application/x-shockwave-flash"]) ?  
               navigator.mimeTypes["application/x-shockwave-flash"].enabledPlugin : 0;
if ( this.plugin ) 
{
  var words = navigator.plugins["Shockwave Flash"].description.split(" ");
  for (var i = 0; i < words.length; ++i)
  {
    if (isNaN(parseInt(words[i])))
	continue;
	var MM_PluginVersion = words[i]; 
  }
  var MM_FlashCanPlay = MM_PluginVersion >= MM_contentVersion;
}
else if (navigator.userAgent && navigator.userAgent.indexOf("MSIE") >= 0 && (navigator.appVersion.indexOf("Win") != -1)) 
{
	document.write('<SCR' + 'IPT LANGUAGE=VBScript\> \n'); //FS hide this from IE4.5 Mac by splitting the tag
	document.write('on error resume next \n');
	document.write('MM_FlashCanPlay = ( IsObject(CreateObject("ShockwaveFlash.ShockwaveFlash." & MM_contentVersion)))\n');
	document.write('</SCR' + 'IPT\> \n');
}
if ( MM_FlashCanPlay ) {
		document.write('<OBJECT classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000"');
	document.write('  codebase="http://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=6,0,0,0" ');
	document.write(' ID="whc_top" WIDTH="326" HEIGHT="76" ALIGN="">');
	document.write(' <PARAM NAME=movie VALUE="images/whc_top.swf"> <PARAM NAME=quality VALUE=high> <PARAM NAME=bgcolor VALUE=#9B9966>  '); 
	document.write(' <EMBED src="images/whc_top.swf" quality=high bgcolor=#9B9966  ');
	document.write(' swLiveConnect=FALSE WIDTH="326" HEIGHT="76" NAME="whc_top" ALIGN=""');
	document.write(' TYPE="application/x-shockwave-flash" PLUGINSPAGE="http://www.macromedia.com/go/getflashplayer">');
	document.write(' </EMBED>');
	document.write(' </OBJECT>');
} else{
	document.write('<IMG SRC="/images/whc_top.jpg" WIDTH="326" HEIGHT="76" usemap="#whc_top" BORDER=0>');
}