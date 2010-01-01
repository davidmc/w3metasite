
// Common routines for combined control support
function dateComboFixup( ctrlId )
{  
  var baseCtrl = myDom.getElementById(ctrlId);
  var mId = "m_"+ctrlId;
  var dId = "d_"+ctrlId;
  var yId = "y_"+ctrlId;
  var mCtrl = myDom.getElementById(mId);
  var dCtrl = myDom.getElementById(dId);
  var yCtrl = myDom.getElementById(yId);
  if( baseCtrl && mCtrl && dCtrl && yCtrl )
  {
    var dateStr = new String(baseCtrl.value);
    var arry = dateStr.split("/");
    if( arry.length == 3 )
    {
      mCtrl.value = arry[0];
      dCtrl.value = arry[1];
      yCtrl.value = arry[2];
    }    
  }  
}
function m_Changed( ctrl )
{ 
  datePart_Changed( ctrl, 0 );   
}
function d_Changed( ctrl )
{
  datePart_Changed( ctrl, 1 );  
}
function y_Changed( ctrl )
{
  datePart_Changed( ctrl, 2 ); 
}

function datePart_Changed( ctrl, idx )
{
  // get base name
  var base_name = ctrl.name.substr(2);
  // get base ctrl
  var baseCtrl = myDom.getElementById(base_name);
  if( baseCtrl )
  {
    // get base value
    var dtVal = new String(baseCtrl.value);
    // split
    var dtAry = dtVal.split("/");  
    // set m value of array to this ctrl value
    dtAry[idx] = ctrl.value;
    // reconstruct value for base control
    baseCtrl.value = dtAry[0] + "/" + dtAry[1] + "/" + dtAry[2];
  } else alert( ctrl.name + " has no base? " + base_name );
  // notify updates with base name
  updates[base_name] = base_name; 
}


