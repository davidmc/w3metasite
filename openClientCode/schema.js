/*
  schema.js

*/

function schemaTable( id, name, abbr )
{
  this.id = new String(id);
  this.name = new String(name);
  this.abbr = new String(abbr);
  this.selected = new Boolean(false);
}

function schemaField( id, name, table, type, link )
{
  this.id = new String(id);
  this.name = new String(name);
  this.table = new String(table);
  this.type = new String(type);
  this.link = new String(link);
  this.selected = new Boolean(false);
}

var schemaTables = new Array();
var schemaFields = new Array();

function addTable( id, name, abbr )
{
  var tb = new schemaTable( id, name, abbr );
  schemaTables[tb.id] = tb;
}
function addField( id, name, table, type, link )
{
  var fd = new schemaField( id, name, table, type, link  );
  schemaFields[fd.id] = fd;
}
function chooseTable( ctrl )
{
  var id = new String(ctrl.value);  
  var fieldsId = id + "fields";
  if( ctrl.checked )
  {    
    schemaTables[id].selected = true;
    myDom.showTableElement(fieldsId);     
  }
  else
  {
    schemaTables[id].selected = false;
    myDom.hideTableElement(fieldsId); 
  }  
}
function chooseField( ctrl )
{
  var id = new String(ctrl.value);  
  if( ctrl.checked )
  {    
    schemaFields[id].selected = true;         
  }
  else
  {
    schemaFields[id].selected = false; 
  }  
}
function joining( table )
{
  var jn = new String(" on ");
  var init = false;
  // find this tables fields

  for( id in schemaFields )
  {
    // winAlert("Table '" + table + "' = '" + schemaFields[id].table + "' ? <br>" );
    if( table == schemaFields[id].table )
    {
      // winAlert("YES");
      var farid = schemaFields[id].link;
            
      if( Number(farid) > 0 )
      {        
        if( init ) jn += " and ";
        // get the table abrev
        jn += schemaTables[schemaFields[id].table].abbr;
        jn += ".";
        // get the field name
        jn += schemaFields[id].name;
        jn += "=";

        // get the far table abrev
        jn += schemaTables[schemaFields[farid].table].abbr;
        jn += ".";
        // get the far field name
        jn += schemaFields[farid].name;        
        init = true;
      }
    }
  }
  if( init == false ) jn = "";
  return jn;
}
function putQuery()
{
  var fromCtrl = myDom.getElementById("From_Part");
  if( fromCtrl )
  {
    var from = new String("");
    var started = false;
    for( id in schemaTables )
    {
      var SchemaTable = schemaTables[id];
      if( SchemaTable && SchemaTable.selected == true )
      {
        if( started )
          from += " inner join ";        
        from += SchemaTable.name + " as " + SchemaTable.abbr;
        if( started )
          from += joining(id); 
        else
          from += "\n-- " + joining(id);  
        started = true;
        from += "\n";  
      }
    }
    fromCtrl.value = from;
  } 
  var selCtrl = myDom.getElementById("Select_Part");
  if( selCtrl )
  {
    var sel = new String("");
    var started = false;
    for( id in schemaFields )
    {
      if( schemaFields[id].selected == true )
      {
        if( started == true ) sel += " , ";
        var table = schemaFields[id].table;
        var abbr = schemaTables[table].abbr;
        sel += abbr;
        sel += ".";
        sel += schemaFields[id].name;
        started = true;       
      }       
    }
    selCtrl.value = sel;
  }  
  
}
// id='From_Part'
