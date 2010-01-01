
function hasNumericData(ctrl,visualFeedback)
{
  var retVal = true
  if (ctrl.value.length == 0)
  {
    retVal = false;
  }
  else if( isNaN(parseInt(ctrl.value)) )
  {
    if( visualFeedback )
      {
      alert( "The " + ctrl.name + " entry value is not numeric. " );
      ctrl.focus()
    }
    ctrlChanged(ctrl);
    retVal = false
  }
  return retVal;
}

function hasText( ctrl )
{
  return (ctrl.value.length > 0);
}

function validRange(ctrl,lo,hi,visualFeedback)
{
  var retVal = true
  if (ctrl.value.length == 0)
  {
    retVal = false
  }
  else if( isNaN(parseInt(ctrl.value)) )
  {
    if( visualFeedback )
      {
      alert( "The " + ctrl.name + " entry value is not numeric. " +
          " The value must be between " + lo + " and " + hi);
      ctrl.focus();
    }
    retVal = false
  }
  else
  {
    // alert( "hi: " + hi );
    if( hi == 0 )
    {
      // when hi is 0 we just want to check the lower range
      if( parseInt(ctrl.value) < lo  )
      {
        if( visualFeedback )
        {
          alert( "The " + ctrl.name + " entry value is out of range. " +
              " The value must be at least " + lo );
          ctrl.focus();

        }
        retVal = false
      }
    }
    else
    {
      if( parseInt(ctrl.value) < lo || parseInt(ctrl.value) > hi )
      {
        if( visualFeedback )
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
  {
    ctrlChanged(ctrl);
  }
  return retVal;
}

function validSSN(ctrl, visualFeedback)
{
  var retVal = false;
  var Value = new String(ctrl.value);
  if (Value.length != 0)
  {
    var reg = new RegExp("-","g");
    var ary = Value.split(reg);
    if( ary.length == 3 )
    {
      if( (new String(ary[0]).length == 3) &&
          (new String(ary[1]).length == 2) &&
          (new String(ary[2]).length == 4) &&
          (isNaN( parseInt( ary[0] ) ) == false) &&
          (isNaN( parseInt( ary[1] ) ) == false) &&
          (isNaN( parseInt( ary[2] ) ) == false) )
      {
        retVal = true;
      }
    }
  }
  if( retVal == false && visualFeedback == true && Value.length > 0 )
  {
      alert("The Social Security Number entered is not valid");
      ctrl.focus();
  }
  if( retVal )
  {
    ctrlChanged(ctrl);
  }
  return retVal;
}
function validDate(ctrl, visualFeedback)
{
  var retVal = false;
  var Value = new String(ctrl.value);

  if (Value.length == 0)
  {
    retVal = true;
  }
  else
  {
    var reg = new RegExp("[/-]","g");
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
    {
      var iToday = new Date();
      strDay = new String( ary[1] );
      strMonth = new String( ary[0] );
      year =   iToday.getFullYear();
      month =  parseInt(strMonth.replace(leader,""));
      day =    parseInt(strDay.replace(leader,""));
    }
    else if( ary.length == 3 )
    {
      strDay =   new String( ary[1] );
      strMonth = new String( ary[0] );
      strYear =  new String( ary[2] );
      month =    parseInt(strMonth.replace(leader,""));
      day =      parseInt(strDay.replace(leader,""));
      year =     parseInt(strYear.replace(leader,""));
      if( year >= 0 && year < 50 )
      {
        year += 2000;
      }
      else if( year >= 50 && year <= 99 )
      {
        year += 1900
      }
    }
    if( ary.length == 2 || ary.length == 3 )
    {
      var strDate = new String( month + "-" + day + "-" + year );
      iDate = new Date(year,month-1,day);
      if( isNaN(iDate)  ||
          year != iDate.getFullYear() ||
          year < 1770 ||
          day != iDate.getDate() ||
          month != iDate.getMonth() + 1 ) // month is zero based
      {
        if( visualFeedback )
        {
          alert("The date entered is not valid");
          ctrl.focus();
        }
      }
      else
      {
        if( visualFeedback )
        {
          ctrl.value = strDate
        }
        retVal = true;
      }
    }
    else if( visualFeedback )
    {
      alert("The date entered is not valid");
      ctrl.focus();
    }
  }
  if( retVal )
  {
    ctrlChanged(ctrl);
  }
  return retVal;
}

function isZip(ctrl, visualFeedback)
{
  var reg1 = new RegExp( "[0-9]{5}-[0-9]{4}","i" );
  var reg2 = new RegExp( "[0-9]{5}" );
  var retval = false;
  var zipcode;

  if( ctrl.value.length != 0 )
  {
    zipcode = String(ctrl.value);
    switch(  zipcode.length )
    {
      case 10:
        if( zipcode.match(reg1) != null )
        {
          retval=true;
        }
      case 5:
        if( zipcode.match(reg2) != null )
        {
          retval=true;
        }
    }
    if( retval == false && visualFeedback )
    {
      alert( "The Zip is not valid" );
      ctrl.focus()
    }
  }
  if( retval )
  {
    ctrlChanged(ctrl);
  }
  return( retval );
}

function isEmail(ctrl, visualFeedback)
{
  var retval = false;

  if( ctrl.value.length != 0 )
  {
    if( (/^[\w\.-]+@[\w\.-]+$/.test(ctrl.value)) )
    {

      retval = true;
    }
    if( retval == false && visualFeedback )
    {
      alert( "The Email address is not valid" );
      ctrl.focus()
    }
  }
  if( retval )
  {
    ctrlChanged(ctrl);
  }
  return(retval);
}



