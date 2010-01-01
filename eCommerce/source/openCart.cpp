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
  // action=add, update
  // id=design id 
  // productId,line1,line2,...,font,fontsize,background,count
  if( args.count("action") )
  {
    string productId = args["productId"].c_str();
    string count=args["count"];

    if( args["action"] == "add" )
    {       
      string options;
      if(args.count("id"))
        options+=string("design=")+args["id"].c_str()+";";
      if(args.count("font")      )
        options+=string("font=")+args["font"].c_str()+";";
      if(args.count("fontsize")      )
        options+=string("fontsize=")+args["fontsize"].c_str()+";";
      if(args.count("background")      )
        options+=string("background=")+args["background"].c_str()+";"; 
      if(args.count("textcolor")      )
        options+=string("textcolor=")+args["textcolor"].c_str();     
      string keyBase="line";
      ocString key = "line1";
      string lines;
      int linNo=1;
      // line args go line1, line2, line3 .. you get the idea
      while( args.count(key.c_str()) )
      {
        lines += args[key.c_str()].c_str();
        lines += "\n";
        key=keyBase;
        key.append(++linNo);
      }
      
      cart.AlwaysAddProduct( productId, count, 
                             options, 
                             lines, 
                             120 ); // stock limiter

    }
    else if( args["action"] == "update" ) // just the count
    {
      // all I'm going to do is adjust the count!
      string productId = args["productId"].c_str();
      string count=args["count"];
      int node=atol(args["prodSeq"].c_str());

      cart.UpdateProduct(  productId, count,      
                           "",
                           "",
                           node );
      // refresh the parent that called me                  
      script << "<script type='text/javascript'>"
                "var loco=parent.location.href;"                
                "parent.location.href = loco"
                "</script>";
    }        
  }  
  // gets items in cart
  cart.stats();
  string checkoutMsg = "Check Out";
  ocString itemCount;
  itemCount.append(cart.items);
  if( cart.items == 0 )
  {
    checkoutMsg = "";
  }  
  ocString qs = "?crtid=";
  qs.append(cart.Id);
  ocString content = htemplate. getUnparsedHtml();
  script << content. replace("$cnt", itemCount.c_str())
                   . replace("$CheckOut", checkoutMsg.c_str())
                   . replace("$url",CHECKOUT)
                   . replace("?qs",qs.c_str() );
  return 0;
}

// just add it here instead of linking it  
#include "read_write_base.cpp"
