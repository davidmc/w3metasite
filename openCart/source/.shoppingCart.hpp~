/*
  shoppingCart.hpp
  
  The shopping cart abstraction

  XML Schema
  ===============================================
    product-tag := product
    product-tag-attributes := (id,count,options)
    id := identifier 
    count := count    
    options := "label=value"
    label := option label
    value := option value        
*/

#ifndef Shopping_Cart_hpp
#define Shopping_Cart_hpp

#include "connectstring"
#include "read_write_base.hpp"
#include "cgiClass.h"
#include "cgiCookie.h"
#include "ocXML.h"
#include "ocString.h"

class ShoppingCart: public read_write_base
{
protected:
  changeMap chMap;
public:
  identifier Id;
  string     XML;
  time_date  Last_Access;
  bool       Checked_Out;  
  
  // constructor
  ShoppingCart():read_write_base(),Id(0LL),Checked_Out(false)
  {
    data_name("ShoppingCart");       
    // ALWAYS add the key first, so we can skip it in insert logic
    addDXMap( new llongXfer("Id", &Id) );
    addDXMap( new stringXfer("XML", &XML) );
    addDXMap( new time_dateXfer("Last_Access", &Last_Access ) );
    addDXMap( new boolXfer("Checked_Out", &Checked_Out) ); 
    // using these forces base class to update all dependent values
    chMap["XML"]="XML";
    chMap["Last_Access"]="Last_Access";
    chMap["Checked_Out"]="Checked_Out";     
    // always keep this var current so we can track the age of the carts.
    Last_Access.now();   
  }   
  // destructor
  virtual ~ShoppingCart()
  {
    ;
  }    
};

/*

  The idea behind a shopping cart is simple enough, hold products.
  The cart doesn't care what the products are.
  The cart doesn't exist until the shopper needs to hold a product.
  The cart persists, until at checkout, its spills it's guts, i.e. the products.
  It can then be considered Checked Out.
  The checked out status is useful for daily store activity reporting.
  
  NOTE: On items in cart: items in cart are sorta virtual.  
  Two peaople can have the same item in their carts at the same time!
  The items becomes 'real' during check out, first to check out wins,
  as the stock_count of the product is decremented.  
  This does not model the physical world, but a more ideal world for the marketer.
  Products should be available to all until someone actually purchases something.
  
*/

class uiShoppingCart: public ShoppingCart
{
private:
  bool addNewCart( void )
  {
    // Add the default cart
    db_action( "i", chMap );    
    if( Id )
    {
      // set the session cookie to the new cart Id.
      cgiCookie cookie; 
      ocString tempId;
      tempId.append(Id);
      cookie.setPath("/");
      cookie.set("crtid",tempId.c_str());
    }
    return true;
  }

public:
  
  cgiScript & script;  
  cgiCookie cookie; 
  int items;
  
  uiShoppingCart(cgiScript & in ): ShoppingCart(),script(in),items(0)
  {
    string id = cookie.get("crtid"); 
    if( id.length() == 0 )
    {
      // try the query string
      if( script.ClientArguments()["crtid"].length() )
      {
        id = script.ClientArguments()["crtid"].c_str();
      }  
    }   
    if( id.length() == 0 ) 
    {
      // try to add the cart
      addNewCart();
    }
    else 
    {
      // get the data
      key( atoll( id.c_str() ) );
      get_data(); 
      
      // here we want to see if the cart is checked out, if so, well
      // set the crtid to nothing, this will forse the creation of a 
      // new cart the next time a cart constructor is called
      if(Checked_Out)
      {
        cookie.setPath("/");
        cookie.set("crtid","");
      }      
    }
  }
  
  virtual ~uiShoppingCart()
  {
    ;
  }
  
  // Sets items to the total number of items in the cart
  void stats( void )
  {
    items = 0;
    xmlParser parser( XML );
    parser.parse();
    node_vector & xnodes = parser.nodeList();
    int i;
    for(i=0;i<xnodes.size();i++)
    {
      xmlNode & node = xnodes[i];
      if( node.name == "product" )
      {
        // no  node.data
        // node attributes should have id, count ,options
        items += atol(node.attr["count"].c_str());             
      } // end if 
    } // end for    
  }
  
  /*
  
   UpdateProduct();
   
   Updates existing product data
   This API does not check the stock count!
   It will unconditionally set the product count to whatever is passed
   
   The 'node' param is for orders where items of the same product may be customized
   for instance, when you order 2 PC's but with different amounts of RAM.
   The Product ID could be the same, but the options and data different.
   
  */ 
  bool UpdateProduct( string productId, 
                      string productCount = "1", 
                      string productOptions = "",
                      string productData = "",
                      int node=0 )
  {
    bool bret = false;
    // Make sure we have a cart    
    if( Id )
    {  
      int prodCount = atol(productCount.c_str());

      // parse the current XML           
      xmlParser parser( XML ); 
      parser.parse();   
      
      // update the last time cart accessed
      Last_Access.now();      
 
      // See if the there is a node with this product
      node_vector::iterator & xIt = parser.findFirstNodeByAttribute("id",productId);
      if( xIt != parser.nodeList().end() )
      {        
        
        if( node > 0 ) // 0 is the first node of like products in the cart
        {

          // more than one node
          for( int idx=1; idx<=node; ++idx)
          {
            
            node_vector::iterator & xIt2 = 
                parser.findNextNodeByAttribute("id",productId);

            if( xIt2 != parser.nodeList().end() )
            {
              xIt=xIt2;
            }
          }         
        }        
        if( prodCount == 0 )
        {
          parser.nodeList().erase(xIt);
        }
        else 
        {
          ocString newCount;
          newCount.append(prodCount);
          xmlNode & xmlItem = (*xIt);
          xmlItem.attr["count"] = newCount;
          if( productOptions.length() )
          {
            xmlItem.attr["options"] = productOptions;
          }
        }
        XML = parser.emit();
        bret=db_action( "u", chMap );        
      }  
    }
    return bret;
  }
  
  /*
    AlwaysAddProduct()
    This version adds the product even if there 
    is already one by the productId.
    
    This is for products that allow for user customization.
    
    Returns true if cart update was a success (false otherwise.)
    
  */  
  bool AlwaysAddProduct( string productId, 
                   string productCount = "1",
                   string productOptions = "",
                   string productData = "",
                   int stock=1 ) // default stock count is one
  { 
    bool bret = false;
    // Make sure we have a cart    
    if( Id )
    {   
      int prodCount = atol(productCount.c_str());
      
      // parse the current XML           
      xmlParser parser( XML ); 
      parser.parse();   
      
      // update the last time cart accessed
      Last_Access.now();
      
      
      // try to get the stock count    
      if(script.ClientArguments().count("stock"))
      stock = atol(script.ClientArguments()["stock"].c_str());
 
      xmlNode  xmlItem;
      xmlItem.name="product";
      xmlItem.monopole=true;
      xmlItem.attr["id"]=productId;
      if( productOptions.length() )
      {
        xmlItem.attr["options"] = productOptions;
      } 
      if( productData.length() )
      {
        xmlItem.data = productData;
        xmlItem.monopole = false;
      } 
      
      // give the customer as much as we can 
      int iCan = prodCount<stock?prodCount:stock;
      ocString canCount;
      canCount.append(iCan);
      xmlItem.attr["count"] = canCount;
      
      // don't add if zero (or less)
      if( iCan > 0 ) parser.addNode( xmlItem );      
      XML = parser.emit();
      bret = db_action( "u", chMap );
    }
    return bret;
  }
  /**
    AddProduct() 
    this version of add product, will look for an existing
    product id and increment the stock count.
    It will also overwrite options and data, if there are any passed.
    0 length options or data will not null existing data.
    You can effectively null by passing a space character.
    The far end should interpret a space as nothing to be concerned about.
    If no such product by the given productId exists, this method will add it.
    returns true if update to cart succeeded
  **/
  bool AddProduct( string productId, 
                   string productCount = "1",
                   string productOptions = "",
                   string productData = "",
                   int stock=1 ) // default stock count is one
  { 
    bool bret = false;
    // Make sure we have a cart    
    if( Id )
    {         
      // parse the current XML           
      xmlParser parser( XML ); 
      parser.parse(); 
       
      // See if the there is already a node with this product
      node_vector::iterator & xIt = parser.findFirstNodeByAttribute("id",productId);
      if( xIt != parser.nodeList().end() )
      {      
        // update the last time cart accessed
        Last_Access.now();
        int prodCount = atol(productCount.c_str());      

        // try to get the stock count    
        if(script.ClientArguments().count("stock"))
        stock = atol(script.ClientArguments()["stock"].c_str());
      
        int existingCount = atol( (*xIt).attr["count"].c_str() );
        existingCount += prodCount;
        
        // you can only give them as much as is in stock
        if( existingCount > stock )
        {
          existingCount = stock;
        }  
        ocString newCount;
        newCount.append(existingCount);
        (*xIt).attr["count"] = newCount;        
        if( productOptions.length() )
        {
          (*xIt).attr["options"] = productOptions;
        }
        if( productData.length() )
        {
          (*xIt).data = productData;
          (*xIt).monopole = false;
          
        }
        XML = parser.emit();
        bret = db_action( "u", chMap ); 
      }
      else
      {
        // this one will add unconditionally
        AlwaysAddProduct( productId, productCount, productOptions, productData, stock );
      }
    
    }
    return bret;
  }
  
  /*
   
   Removes one item by product id
   RemoveProduct( string productId )
   This is for simple cases where there is one item of a given product id.
   For customization, use Update Product instead.
  */ 
  bool RemoveProduct( string productId )
  { 
    bool bret=false;
    // Make sure we have a cart    
    if( Id )
    { 
      // parse the current XML           
      xmlParser parser( XML ); 
      parser.parse();   
      
      // update the last time cart accessed
      Last_Access.now();
      
      node_vector::iterator & xIt = parser.findFirstNodeByAttribute("id",productId);
      if( xIt != parser.nodeList().end() )
      {   
        script << " found node <br>" << endl;    
        int existingCount = atol((*xIt).attr["count"].c_str());
        if( existingCount > 1 )
        {
          script << " decrement count <br>" << endl; 
          // Decrement a specific product count
          ocString newCount;          
          newCount.append((existingCount-1));
          (*xIt).attr["count"] = newCount;
        }
        else
        {
          // If 0 remove product node
          parser.nodeList().erase(xIt);
        }  
      }
      XML = parser.emit();
      bret=db_action( "u", chMap );
    } 
    return bret;  
  }
  
  
  bool CheckOut( void )
  {
    // just set the Checked_Out flag
    Checked_Out = true;
    db_action( "u", chMap );
  }
  
  bool CancelOrder( void )
  {
    // just reset the XML product item content
    XML = "";
    db_action( "u", chMap );
  }     
};

#endif

