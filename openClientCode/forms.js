var phoneFormat = "###-###-#### ";

function hasNumericData(ctrl,visualFeedback)
{ var retVal = true
  if (ctrl.value.length == 0)
  { retVal = false;
  }
  else if( isNaN(parseInt(ctrl.value)) )
  { if( visualFeedback )
    { alert( "The " + ctrl.name + " entry value is not numeric. " );
      ctrl.focus()
    }
    ctrlChanged(ctrl);
    retVal = false
  }
  return retVal;
}
function hasFormattedNumericData(ctrl,visualFeedback)
{ var retVal = true
  if (ctrl.value.length == 0)
  { retVal = false;
  }
  else 
  {
    var val = new String(ctrl.value);
    val = val.replace(/\,/g,"");
    //winAlert( "val="+val+"<br>" );
    if( isNaN(parseInt(val) ) )
    { if( visualFeedback )
      { alert( "The " + ctrl.name + " entry value is not numeric. " );
        ctrl.focus();
      }
      retVal = false
    }
    else
    {      
      // reformat it
      var len = val.length;
      var rev = new String("");
      for(var i=1; i<=len; i++)
      {
        rev += val.substr(len-i,1);
        if( i%3 == 0 ) rev += ",";     
      }
      //winAlert("rev="+rev+"<br>");
      val="";
      len =rev.length;
      for(var i=1; i<=len; i++)
      {
        val += rev.substr(len-i,1);               
      }
      val = val.replace(/^,/,"");
      //winAlert( "val="+val+"<br>" );
      ctrl.value = val;
    }
    ctrlChanged(ctrl);     
  }
  return retVal;
}
function isMoney(ctrl,visualFeedback)
{ var retVal = true
  if (ctrl.value.length == 0)
  { retVal = false;
  }
  else 
  {
    var val = new String(ctrl.value);
    val = val.replace(/[$\,]/g,"");
    dec = new String("00");
    // winAlert("val: " + val + "<br>\n");
    moneyparts = val.split(".");    
    if( moneyparts.length == 2 )
    { 
      val = moneyparts[0];
      var tval = parseFloat("0."+moneyparts[1])*100;
      tval = Math.round(tval);
      // tval can only round up to 100 at most
      if(tval == 100)
      {
        tval = 0;
        newVal = parseInt(val);
        newVal += 1; 
        val = newVal.toString(); 
      }
      dec = tval.toString();
      if( dec.length == 1 ) dec = "0" + dec;  // prepend less than zero amounts        
    }
    // winAlert("dec: "+ dec + "<br>\n");
    if( isNaN(parseInt(val) ) )
    { if( visualFeedback )
      { alert( "The " + ctrl.name + " entry value is not numeric. " + val );
        ctrl.focus();
      }
      retVal = false
    }
    else
    {      
      // reformat it
      var len = val.length;
      var rev = new String("");
      for(var i=1; i<=len; i++)
      {
        rev += val.substr(len-i,1);
        if( i%3 == 0 ) rev += ",";     
      }
      // winAlert("rev="+rev+"<br>\n");
      val="";
      len =rev.length;
      for(var i=1; i<=len; i++)
      {
        val += rev.substr(len-i,1);               
      }
      val = val.replace(/^,/,"");
      val = val + ".";
      val = val + dec;
      
      //winAlert( "val="+val+"<br>\n" );
      ctrl.value = "$" + val;
    }
    ctrlChanged(ctrl);     
  }
  return retVal;
}
function hasText( ctrl )
{ return (ctrl.value.length > 0);
}

function validRange(ctrl,lo,hi,visualFeedback)
{ var retVal = true
  if (ctrl.value.length == 0)
  { retVal = false
  }
  else if( isNaN(parseInt(ctrl.value)) )
  { if( visualFeedback )
    { alert( "The " + ctrl.name + " entry value is not numeric. " +
          " The value must be between " + lo + " and " + hi);
      ctrl.focus();
    }
    retVal = false
  }
  else
  { // alert( "hi: " + hi );
    if( hi == 0 )
    { // when hi is 0 we just want to check the lower range
      if( parseInt(ctrl.value) < lo  )
      { if( visualFeedback )
        {
          alert( "The " + ctrl.name + " entry value is out of range. " +
              " The value must be at least " + lo );
          ctrl.focus();
        }
        retVal = false
      }
    }
    else
    { if( parseInt(ctrl.value) < lo || parseInt(ctrl.value) > hi )
      { if( visualFeedback )
        {
          alert( "The " + ctrl.name + " entry value is out of range. " +
              " The value must be between " + lo + " and " + hi);
          ctrl.focus();

        }
        retVal = false
      }
    }
  }
  if( retVal )
  { ctrlChanged(ctrl);
  }
  return retVal;
}
function validLength( ctrl, minLen, visualFeedback, ctrlName )
{
  var retVal = true
  if (ctrl.value.length < minLen)
  { if( visualFeedback )
    { alert( ctrlName + " is too short in length. " +
             " The length must be at least " + minLen + " characters");
      ctrl.focus();
    }
    retVal = false
  }  
  if( retVal )
  { ctrlChanged(ctrl);
  }
  return retVal;
}
function validSSN(ctrl, visualFeedback)
{ var retVal = false;
  var Value = new String(ctrl.value);
  if (Value.length != 0)
  { var reg = new RegExp("-","g");
    var ary = Value.split(reg);
    if( ary.length == 3 )
    { if( (new String(ary[0]).length == 3) &&
          (new String(ary[1]).length == 2) &&
          (new String(ary[2]).length == 4) &&
          (isNaN( parseInt( ary[0] ) ) == false) &&
          (isNaN( parseInt( ary[1] ) ) == false) &&
          (isNaN( parseInt( ary[2] ) ) == false) )
      { retVal = true;
      }
    }
  }
  if( retVal == false && visualFeedback == true && Value.length > 0 )
  { alert("The Social Security Number entered is not valid");
    ctrl.focus();
  }
  if( retVal )
  { ctrlChanged(ctrl);
  }
  return retVal;
}
function validDate(ctrl, visualFeedback)
{ var retVal = false;
  var Value = new String(ctrl.value);
  if (Value.length == 0)
  { retVal = true;
  }
  else
  { var reg = new RegExp("[/-]","g");
    var leader = new RegExp("^0","g");
    var ary = Value.split(reg);
    var iDate;
    var day;
    var month;
    var year;
    var strDay;
    var strMonth;
    var strYear;
    if( ary.length == 2 )
    { var iToday = new Date();
      strDay = new String( ary[1] );
      strMonth = new String( ary[0] );
      year =   iToday.getFullYear();
      month =  parseInt(strMonth.replace(leader,""));
      day =    parseInt(strDay.replace(leader,""));
    }
    else if( ary.length == 3 )
    { strDay =   new String( ary[1] );
      strMonth = new String( ary[0] );
      strYear =  new String( ary[2] );
      month =    parseInt(strMonth.replace(leader,""));
      day =      parseInt(strDay.replace(leader,""));
      year =     parseInt(strYear.replace(leader,""));
      if( year >= 0 && year < 50 )
      { year += 2000;
      }
      else if( year >= 50 && year <= 99 )
      { year += 1900
      }
    }
    if( ary.length == 2 || ary.length == 3 )
    { var strDate = new String( month + "-" + day + "-" + year );
      iDate = new Date(year,month-1,day);
      if( isNaN(iDate)  ||
          year != iDate.getFullYear() ||
          year < 1770 ||
          day != iDate.getDate() ||
          month != iDate.getMonth() + 1 ) // month is zero based
      { if( visualFeedback )
        {
          alert("The date entered is not valid");
          ctrl.focus();
        }
      }
      else
      { if( visualFeedback )
        {
          ctrl.value = strDate
        }
        retVal = true;
      }
    }
    else if( visualFeedback )
    { alert("The date entered is not valid");
      ctrl.focus();
    }
  }
  if( retVal )
  { ctrlChanged(ctrl);
  }
  return retVal;
}
function validTime(ctrl, visualFeedback)
{ var retVal = false;
  var Value = new String(ctrl.value);
  if (Value.length == 0)
  {
    retVal = true;
  }
  else
  {
    var reg = new RegExp("[: ]","g");
    var leader = new RegExp("^0","g");
    var dayIndicator = new RegExp("^[AP]","i" );
    var morningIndicator = new RegExp("^A","i" );
    var ary = Value.split(reg);
    var iHour = new Number(0);
    var iMinute = new Number(0);
    var iSecond = new Number(0);
    var AMPM = new String("AM");        
    if( ary.length >= 1 )
    { temp = new String( ary[0] )
      iHour = parseInt( temp.replace(leader,"") );       
    }
    if( ary.length >= 2 )
    { temp = new String(ary[1] );
      iMinute = parseInt( temp.replace(leader,"") );      
    }  
    if( ary.length == 3 )
    { temp = new String(ary[2] );
      // were not sure about this
      if( dayIndicator.test( temp ) )
      {
        AMPM = morningIndicator.test(temp)?"AM":"PM";
      }
      else
      {
        iSecond = parseInt(temp.replace(leader,"") );
      }
    }
    if( ary.length == 4 )
    { iSecond = parseInt( ary[2] );
      temp = new String(ary[3] );
      // were not sure about this
      if( dayIndicator.test( temp ) )
      {
        AMPM = morningIndicator.test(temp)?"AM":"PM";        
      }
    }
    if( iHour > 12 )
    { AMPM = "PM"
      iHour -= 12;
    }
    if( ary.length >= 2 && ary.length <= 4 )
    { if( isNaN(iHour) || isNaN(iMinute) ||
          iHour < 1 || iHour > 12 ||
          iMinute < 0 || iMinute > 59 ) 
      { if( visualFeedback )
        { alert("The time entered is not valid (expect HH:MM (A,P)M");
          ctrl.focus();
        }
      }
      else
      { if( visualFeedback )
        { var strTime = new String("");
          if( iHour < 10 ) strTime = "0";
          strTime += iHour.toString();
          strTime += ":";
          if( iMinute < 10 ) strTime += "0";
          strTime += iMinute.toString();    
          strTime += " ";
          strTime += AMPM;
          ctrl.value = strTime.toString();
        }
        retVal = true;
      }
    }
    else if( visualFeedback )
    { alert("The time entered is incomplete (expect HH:MM (A,P)M");
      ctrl.focus();
    }
  }
  if( retVal )
  { ctrlChanged(ctrl);
  }
  return retVal;
}
function isZip(ctrl, visualFeedback)
{ var reg1 = new RegExp( "[0-9]{5}-[0-9]{4}","i" );
  var reg2 = new RegExp( "[0-9]{5}" );
  var retval = false;
  var zipcode;
  if( ctrl.value.length != 0 )
  { zipcode = String(ctrl.value);
    switch(  zipcode.length )
    { case 10:
        if( zipcode.match(reg1) != null )
        { retval=true;
        }
      case 5:
        if( zipcode.match(reg2) != null )
        { retval=true;
        }
    }
    if( retval == false && visualFeedback )
    { alert( "The Zip is not valid" );
      ctrl.focus()
    }
  }
  if( retval )
  { ctrlChanged(ctrl);
  }
  return( retval );
}

function isEmail(ctrl, visualFeedback)
{ var retval = false;
  if( ctrl.value.length != 0 )
  {
    if( (/^[\w\.\-_]+@[\w\.\-_]+$/.test(ctrl.value)) )    
    { retval = true;
    }
    if( retval == false && visualFeedback )
    { alert( "The Email address is not valid" );
      ctrl.focus()
    }
  }
  if( retval )
  { ctrlChanged(ctrl);
  }
  return(retval);
}

function isPhoneNumber(ctrl, visualFeedback)
{ var retVal = true;
  var val = new String( ctrl.value );   
  val = val.replace(/[ a-zA\-\(\)\.]/gi,"");  
  if( val.length < 10 )
  { if( visualFeedback )
    { alert( "The " + ctrl.name + " entry value doensn't look like a phone number. " );
      ctrl.focus();
    }
    retVal = false;
  }
  else     
  { if( isNaN(parseInt(val)) )
    { if( visualFeedback )
      { alert( "The " + ctrl.name + " entry value doensn't look like a phone number. " );
        ctrl.focus();
      }
      retVal = false;
    }
    else
    { var fVal = phoneFormat.replace( /###/, val.substr(0,3) )
                            .replace( /###/, val.substr(3,3) )
                            .replace( /####/, val.substr(6,4) );
      if( val.length > 10 ) fVal += " x" + val.substr(10); // extension
      ctrl.value = fVal;
      ctrlChanged(ctrl);
    }     
    
  }

  return retVal;
}


