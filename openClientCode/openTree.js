/*
   openTree object
   ==========================
    A browser independent table cell based
    Tree Abstraction.
    -------------------------------------------------
    - This resolves many of the
        inconsistancies between Netscape and MSIE.
    ------------------------
    Depends on openDom.js
    ------------------------
    (c) 2002 w3 Systems Development (w3sys.com)
    Author David McCombs. (davidmc@w3sys.com)
*/

function openTree()
{
  this.dom = new openDom();
  this.dom.parseCookies();
  this.elements = new Array();
  this.sel = new String("");
  this.collapseAll = openTree_collapseAll;
  this.expandCat = openTree_expandCat;
  this.addLeaf = openTree_addLeaf;
  this.persistState = openTree_persistState;
  this.cookieLabel = "otreesel";
}

function openTree_persistState()
{
  if( this.dom.hasCookie(this.cookieLabel) )
  {
    this.expandCat( this.dom.parsedCookies[this.cookieLabel].value );
  }
}

function openTree_addLeaf( id )
{
  this.elements.push( id );
}
function openTree_collapseAll()
{
  for( i =0;
       i < this.elements.length;
       i++ )
  {
    this.dom.hideTableElement( this.elements[i] );
  }
}

function openTree_expandCat( id )
{
  this.collapseAll();
  if( id != this.sel )
  {
    this.dom.showTableElement(id);
    this.sel = id;
    // pesist the sel status across http requests
    this.dom.setCookie( this.cookieLabel, id );
  }
  else
  {
    this.sel = "";
    this.dom.setCookie( this.cookieLabel, "" );
  }
}