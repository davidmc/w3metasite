/* 
  anyData.js
  This file prevents the browser from monkeying with my form data
  It needs code on the server side to encode/decode the inverse
  
  (c) David McCombs - W3 Systems 2006 
  
  LGPL License terms
*/ 


// ascii lookups
var aryHexEncode = new Array();
aryHexEncode["\t"]="09";aryHexEncode["\n"]="0A";aryHexEncode["\r"]="0D";
aryHexEncode[" "]="20";
aryHexEncode["!"]="21";aryHexEncode["\""]="22";aryHexEncode["#"]="23";aryHexEncode["$"]="24";
aryHexEncode["%"]="25";aryHexEncode["&"]="26";aryHexEncode["'"]="27";aryHexEncode["("]="28";
aryHexEncode[")"]="29";aryHexEncode["*"]="2A";aryHexEncode["+"]="2B";aryHexEncode[","]="2C";
aryHexEncode["-"]="2D";aryHexEncode["."]="2E";aryHexEncode["/"]="2F";aryHexEncode["0"]="30";
aryHexEncode["1"]="31";aryHexEncode["2"]="32";aryHexEncode["3"]="33";aryHexEncode["4"]="34";
aryHexEncode["5"]="35";aryHexEncode["6"]="36";aryHexEncode["7"]="37";aryHexEncode["8"]="38";
aryHexEncode["9"]="39";aryHexEncode[":"]="3A";aryHexEncode[";"]="3B";aryHexEncode["<"]="3C";
aryHexEncode["="]="3D";aryHexEncode[">"]="3E";aryHexEncode["?"]="3F";aryHexEncode["@"]="40";
aryHexEncode["A"]="41";aryHexEncode["B"]="42";aryHexEncode["C"]="43";aryHexEncode["D"]="44";
aryHexEncode["E"]="45";aryHexEncode["F"]="46";aryHexEncode["G"]="47";aryHexEncode["H"]="48";
aryHexEncode["I"]="49";aryHexEncode["J"]="4A";aryHexEncode["K"]="4B";aryHexEncode["L"]="4C";
aryHexEncode["M"]="4D";aryHexEncode["N"]="4E";aryHexEncode["O"]="4F";aryHexEncode["P"]="50";
aryHexEncode["Q"]="51";aryHexEncode["R"]="52";aryHexEncode["S"]="53";aryHexEncode["T"]="54";
aryHexEncode["U"]="55";aryHexEncode["V"]="56";aryHexEncode["W"]="57";aryHexEncode["X"]="58";
aryHexEncode["Y"]="59";aryHexEncode["Z"]="5A";aryHexEncode["["]="5B";aryHexEncode["\\"]="5C";
aryHexEncode["]"]="5D";aryHexEncode["^"]="5E";aryHexEncode["_"]="5F";aryHexEncode["`"]="60";
aryHexEncode["a"]="61";aryHexEncode["b"]="62";aryHexEncode["c"]="63";aryHexEncode["d"]="64";
aryHexEncode["e"]="65";aryHexEncode["f"]="66";aryHexEncode["g"]="67";aryHexEncode["h"]="68";
aryHexEncode["i"]="69";aryHexEncode["j"]="6A";aryHexEncode["k"]="6B";aryHexEncode["l"]="6C";
aryHexEncode["m"]="6D";aryHexEncode["n"]="6E";aryHexEncode["o"]="6F";aryHexEncode["p"]="70";
aryHexEncode["q"]="71";aryHexEncode["r"]="72";aryHexEncode["s"]="73";aryHexEncode["t"]="74";
aryHexEncode["u"]="75";aryHexEncode["v"]="76";aryHexEncode["w"]="77";aryHexEncode["x"]="78";
aryHexEncode["y"]="79";aryHexEncode["z"]="7A";aryHexEncode["{"]="7B";aryHexEncode["|"]="7C";
aryHexEncode["}"]="7D";

var aryHexDecode = new Array();
aryHexDecode["09"]="\t";aryHexDecode["0A"]="\n";aryHexDecode["0D"]="\r"
aryHexDecode["20"]=" ";
aryHexDecode["21"]="!";aryHexDecode["22"]="\"";aryHexDecode["23"]="#";aryHexDecode["24"]="$";
aryHexDecode["25"]="%";aryHexDecode["26"]="&";aryHexDecode["27"]="'";aryHexDecode["28"]="(";
aryHexDecode["29"]=")";aryHexDecode["2A"]="*";aryHexDecode["2B"]="+";aryHexDecode["2C"]=",";
aryHexDecode["2D"]="-";aryHexDecode["2E"]=".";aryHexDecode["2F"]="/";aryHexDecode["30"]="0";
aryHexDecode["31"]="1";aryHexDecode["32"]="2";aryHexDecode["33"]="3";aryHexDecode["34"]="4";
aryHexDecode["35"]="5";aryHexDecode["36"]="6";aryHexDecode["37"]="7";aryHexDecode["38"]="8";
aryHexDecode["39"]="9";aryHexDecode["3A"]=":";aryHexDecode["3B"]=";";aryHexDecode["3C"]="<";
aryHexDecode["3D"]="=";aryHexDecode["3E"]=">";aryHexDecode["3F"]="?";aryHexDecode["40"]="@";
aryHexDecode["41"]="A";aryHexDecode["42"]="B";aryHexDecode["43"]="C";aryHexDecode["44"]="D";
aryHexDecode["45"]="E";aryHexDecode["46"]="F";aryHexDecode["47"]="G";aryHexDecode["48"]="H";
aryHexDecode["49"]="I";aryHexDecode["4A"]="J";aryHexDecode["4B"]="K";aryHexDecode["4C"]="L";
aryHexDecode["4D"]="M";aryHexDecode["4E"]="N";aryHexDecode["4F"]="O";aryHexDecode["50"]="P";
aryHexDecode["51"]="Q";aryHexDecode["52"]="R";aryHexDecode["53"]="S";aryHexDecode["54"]="T";
aryHexDecode["55"]="U";aryHexDecode["56"]="V";aryHexDecode["57"]="W";aryHexDecode["58"]="X";
aryHexDecode["59"]="Y";aryHexDecode["5A"]="Z";aryHexDecode["5B"]="[";aryHexDecode["5C"]="\\";
aryHexDecode["5D"]="]";aryHexDecode["5E"]="^";aryHexDecode["5F"]="_";aryHexDecode["60"]="`";
aryHexDecode["61"]="a";aryHexDecode["62"]="b";aryHexDecode["63"]="c";aryHexDecode["64"]="d";
aryHexDecode["65"]="e";aryHexDecode["66"]="f";aryHexDecode["67"]="g";aryHexDecode["68"]="h";
aryHexDecode["69"]="i";aryHexDecode["6A"]="j";aryHexDecode["6B"]="k";aryHexDecode["6C"]="l";
aryHexDecode["6D"]="m";aryHexDecode["6E"]="n";aryHexDecode["6F"]="o";aryHexDecode["70"]="p";
aryHexDecode["71"]="q";aryHexDecode["72"]="r";aryHexDecode["73"]="s";aryHexDecode["74"]="t";
aryHexDecode["75"]="u";aryHexDecode["76"]="v";aryHexDecode["77"]="w";aryHexDecode["78"]="x";
aryHexDecode["79"]="y";aryHexDecode["7A"]="z";aryHexDecode["7B"]="{";aryHexDecode["7C"]="|";
aryHexDecode["7D"]="}";

function w3HexEncode( asciiIn )
{
  var AsciiIn = new String( asciiIn );
  var HexOut = new String("");
  for( var i=0;i<AsciiIn.length;i++)
  {
    var toCvt = new String(AsciiIn.charAt(i));
    HexOut += aryHexEncode[toCvt]; 
  }
  return HexOut;
}
function w3HexDecode( hexIn )
{
  var HexIn = new String( hexIn );
  var AsciiOut = new String("");
  for( var i=0;i<HexIn.length;i+=2)
  {
    var toCvt = new String(HexIn.substr(i,2));
    AsciiOut += aryHexDecode[toCvt];
  }
  return AsciiOut;
}


