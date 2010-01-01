var changeArray = new Array();
var className = new String();

// for change tracking on user input
function ctrlChanged( ctrl )
{
  var name = new String(ctrl.name);
  var value = String();
  if( ctrl.type == "checkbox" )
  {
    value = ctrl.checked==true?"true":"false";
    changeArray[name] = value;
  }
  else if ( ctrl.type == "radio" )
  {
    if( ctrl.checked==true )
    {
      value = ctrl.value;
      changeArray[name] = value;
    }
  }
  else
  {
    value = ctrl.value;
    changeArray[name] = value;
  }
}

function doUpdate()
{
  var values = new String();
  for( var name in changeArray )
  {
    values += name + '~' + changeArray[name] + '`';
  }
  var strKey = new String( document.forms[0].Id.value );
  uForm = document.forms[1];
  uForm.classname.value=className;
  uForm.todo.value='u';
  uForm.key.value = strKey;
  uForm.values.value = values;
  uForm.response.value = document.location.href;
  uForm.submit();
}

// the delete function
function doDelete()
{
  var strKey = new String( document.forms[0].Id.value );
  if ( confirm('Are you sure you want to delete '+strKey+'?') )
  {

    var uForm = document.forms[1];
    uForm.classname.value=className;
    uForm.todo.value='d';
    uForm.key.value = strKey;
    uForm.values.value = ' ';
    uForm.response.value = document.location.href;
    uForm.submit();
  }
}

function doInsert()
{
  var values = new String();
  for( var name in changeArray )
  {
    values += name + '~' + changeArray[name] + '`';
  }
  var uForm = document.forms[1];
  uForm.classname.value=className;
  uForm.todo.value='i';
  uForm.key.value = '';
  uForm.values.value = values;
  uForm.response.value = document.location.href;
  uForm.submit();
}

