/*
  openCart.cpp 
  ----------------------------------------
  this is the source file for openCart.cgi
  
  Copyright 2003, David McCombs, www.w3sys.com
  
  for exclusive, but unrestricted use of my eCommerce clients.  

*/

#include "shoppingCart.hpp"
#include "cgiTemplates.h"

int main( int iCount, char ** pchArgs )
{
  // Create an instance of the cgi script but DO NOT close the http header
  cgiScript script( "text/html", false);
  cgiTemplates htemplate;
  htemplate.load("Templates/ShoppingCart.html");
  
  // Create an instance of the shopping cart.
  uiShoppingCart cart( script );
  script.closeHeader();  
  
  // See what the user wants.
  cgiInput & args = script.ClientArguments (); 
  
  if( args.count("action") )
  {
    if( args["action"] == "add" )
    {
      string productId = args["id"].c_str();
      cart.AddProduct( productId );
    }
    else if( args["action"] == "remove" )
    {
      string productId = args["id"].c_str();
      cart.RemoveProduct( productId );
      script << "<script type='text/javascript'>"
                "var loco=parent.location;"
                "parent.location = loco"
                "</script>";
    }    
  }  
  cart.stats();
  
  ocString itemCount;
  itemCount.append(cart.items);
  if( cart.items != 1 )
  {
    itemCount += " items";
  }
  else
  {
    itemCount += " item";
  }  
  ocString qs = "?crtid=";
  qs.append(cart.Id);
  ocString content = htemplate. getUnparsedHtml();
  script << content. replace("O Items", itemCount.c_str())
                   . replace("$url",CHECKOUT)
                   . replace("?qs",qs.c_str() );
  return 0;
}

// just add it here instead of linking it  
#include "read_write_base.cpp"
