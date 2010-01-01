/*
  Copyright 2003 by D.K. McCombs.
    
  // Numeric Integer
  FIELD_TYPE_SHORT,FIELD_TYPE_LONG,
  FIELD_TYPE_LONGLONG,FIELD_TYPE_INT24
  
  // Numeric Float
  FIELD_TYPE_FLOAT,FIELD_TYPE_DOUBLE
  
  // Time and Date
  FIELD_TYPE_TIMESTAMP,FIELD_TYPE_DATE,FIELD_TYPE_TIME,
  FIELD_TYPE_DATETIME
  
  // String
  FIELD_TYPE_TINY_BLOB,FIELD_TYPE_MEDIUM_BLOB,
  FIELD_TYPE_LONG_BLOB,FIELD_TYPE_BLOB,
  FIELD_TYPE_VAR_STRING,FIELD_TYPE_STRING
   
 */

#include "openLogin.h"
#include "cgiTemplates.h"
#include "objectBase.h"
#include "connectstring"
#include "forms.h"
#include "siteLimit.h"

class pageObject: public objectBase
{
  string siteLimitation,sql,template_sql;
public:
  pageObject(openLogin &lin, cgiScript & script):objectBase(lin,script)
  {
    string siteLimitation = siteFocus(lin);
    
    // Add fields
    addField( "id", FIELD_TYPE_LONGLONG );
    addField( "site_id", FIELD_TYPE_LONGLONG );
    addField( "template_id", FIELD_TYPE_LONGLONG );
    addField( "name", FIELD_TYPE_STRING );
    addField( "url", FIELD_TYPE_STRING ); 
    addField( "meta_description", FIELD_TYPE_STRING );  

    // Add controls
    addControl( "id", staticEntry( "id", "10" ) );
    sql += "select id, name from metasite.sites";
    template_sql += "select id, name from metasite.templates";
    if( siteLimitation.length() != 0 )
    {
      sql += " where id ";
      sql += siteLimitation;
      template_sql += "  where site_id ";
      template_sql += siteLimitation;
    }
    
    sql += " order by name";    
    addControl( "site_id", cmboEntry( "site_id", sql ) );
    addControl( "template_id", cmboEntry( "template_id", template_sql ) );
    addControl( "name", textEntry( "name", "64" ) );        
    addControl( "url", textEntry( "url", "64" ) );
    addControl( "meta_description", areaEntry( "meta_description" ) );
    
    // set name
    setName("pages");    
  }
  
  // must have no user roles on user delete
  bool validate(void){ return true; }

  // will use cleanup to add, delete user account, or change account password
  bool cleanup(void) { return true; }
};

void page_menuList( cgiScript & script, 
                    cgiInput & args, 
                    cgiTemplates & htmlDoc,
                    ocString & bottom1,
                    string & id )
{
  string newLink = "page_menuForm.cgi?page_id=";
  newLink += id;  
  ocString childtop = htmlDoc.getParagraph("childtop");
  script << childtop.replace("__child__",
                             "Page Menu "
                             "<span class='instructions'>Add/Modify page menus here.</span>")
                    .replace("_header_","Page Name</th><th>Menu Name</th><th>Place Order")                      
                    .replace("_link_",newLink.c_str());  
  mySqlDB db(CONNECTSTRING);
  mySqlRS rs(db); 
  string sql = "select pm.id, p.name, m.name, pm.place_order "
      "from metasite.page_menus pm "
      "inner join metasite.menus m on m.id = pm.menu_id "
      "inner join metasite.pages p on p.id = pm.page_id where pm.page_id = ";
  sql+= id;
  sql += " order by p.name, pm.place_order";      
  script << "<!-- " << sql << "-->" << endl;

  for( bool b = rs.open(sql); b; b= rs.next() )
  {
    string vis = rs.getField(1).format() + "</td><td>";
    string compoundId=rs.getField(0).format() + "&page_id=";
    compoundId+=id; 
    vis += rs.getField(2).format() + "</td><td>";
    vis += rs.getField(3).format();
    ocString listitem = htmlDoc.getParagraph("childitem");
    script <<  listitem.replace("_id_",rs.getField(0).format().c_str())
                       .replace("_listdata_",vis.c_str())                               
                       .replace("_link_","page_menuForm.cgi")
                       .replace("_id_",compoundId.c_str());                         
  }
  script << htmlDoc.getParagraph("childbottom");
}
void paragraphList( cgiScript & script, 
                    cgiInput & args, 
                    cgiTemplates & htmlDoc,
                    ocString & bottom1,
                    string & id )
{
  string newLink = "paragraphForm.cgi?page_id=";
  newLink += id;                      
  ocString childtop = htmlDoc.getParagraph("childtop");
  script << childtop.replace("__child__",
                             "Paragraphs "
                             "<span class='instructions'>Add/Modify paragraphs to/on the page here.</span>")
                    .replace("_header_","Paragraph</th><th>Placement Order</th><th>Template Tag")                      
                    .replace("_link_",newLink.c_str());
  string sql;
  mySqlDB db(CONNECTSTRING);
  mySqlRS rs(db);    
  sql = "select id, name, place_order, template_tag "
      "from metasite.paragraphs "
        "where page_id = ";
  sql+= id;
  sql += " order by place_order";    
  script << "<!-- " << sql << "-->" << endl;

  for( bool b = rs.open(sql); b; b= rs.next() )
  {
    string vis = rs.getField(1).format() + "</td><td>";
    string compoundId=rs.getField(0).format() + "&page_id=";
    compoundId+=id; 
    vis += rs.getField(2).format() + "</td><td>";
    vis += rs.getField(3).format();
    ocString listitem = htmlDoc.getParagraph("childitem");
    script <<  listitem.replace("_id_",rs.getField(0).format().c_str())
                       .replace("_listdata_",vis.c_str())                               
                       .replace("_link_","paragraphForm.cgi")
                       .replace("_id_",compoundId.c_str());                         
  }
  script << htmlDoc.getParagraph("childbottom");
}
void pageForm(cgiScript & script, openLogin & login)
{
  pageObject obj(login,script);
  cgiTemplates htmlDoc;
  htmlDoc.load("Templates/siteAdminForm.html");  
  ocString top(htmlDoc.getParagraph("top"));
  cgiInput & args = script.ClientArguments();

  script << top.replace( "_entity_", "Page" )
               .replace( "_link_", "pageList.cgi" )
               .replace( "__link__", "pageForm.cgi" ) << endl;

  obj.go();

  ocString listTemplate(htmlDoc.getParagraph("formitem"));

  script << listTemplate.replace ( "_label_", "Id" )
                        .replace ( "_form_item_", obj.getControl("id")->getHtml().c_str() );
                        
  script << listTemplate.replace ( "_label_", "Site" )
                        .replace ( "_form_item_", obj.getControl("site_id")->getHtml().c_str() );
                        
  script << listTemplate.replace ( "_label_", "Template" )
                        .replace ( "_form_item_", obj.getControl("template_id")->getHtml().c_str() );
 
  script << listTemplate.replace ( "_label_", "Name" )
                        .replace ( "_form_item_", obj.getControl("name")->getHtml().c_str() );

  script << listTemplate.replace ( "_label_", "URL" )
                        .replace ( "_form_item_", obj.getControl("url")->getHtml().c_str() );
                        
   script << listTemplate.replace ( "_label_", "Metatag Description Text" )
                         .replace ( "_form_item_", obj.getControl("meta_description")->getHtml().c_str() );

  // PLACE CHILDREN (page items)
  ocString bottom1(htmlDoc.getParagraph("altbottom1"));
  // Get the current id
  string id = obj.getControl("id")->getContent();
  if( id.length() )
  {
    script << bottom1.replace("_form_buttons_", formButtons(args).c_str()); 
    page_menuList( script, args, htmlDoc, bottom1, id );
    paragraphList( script, args, htmlDoc, bottom1, id );    
  }
  else
  {
    script << bottom1.replace("_form_buttons_", formButtons(args).c_str()).replace( "__more_instructions__", " " );
  }
  ocString bottom2(htmlDoc.getParagraph("altbottom2"));
  script << bottom2.replace( "__results__", obj.resultString().c_str() )
                   .replace( "__js__", "document.forms[0].name.focus();" ) << endl;                
}

int main( int argcount, char ** args )
{
  bool isSignedOn = false;

  // An instance of the script
  // must be created before any other output.
  cgiScript script("text/html",false);
  openLogin login;

  // attempt to get cookie
  if ( login.testLoginStatus() )
  {
    isSignedOn = true;
  }
  if( isSignedOn )
  {
    script.closeHeader();
    pageForm(script,login);
  }
  else
  {
    script.Redirect("signIn.html");
    script.closeHeader();
    return(0);
  }
}
