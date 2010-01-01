/*

  Artisan Form
  =============
  CGI Form for Artisan Point Artisan

*/
#include "connectstring"
#include <iostream>
#include <iomanip>
#include "ocTypes.h"
#include "ocString.h"
#include "read_write_base.hpp"
#include "forms_base.hpp"
#include "cgiTemplates.h"
#include "Artisan.hpp"
#include "nav_menu.h"
#include "list_base.hpp"

/*
  Artisan Form - Glues the business object to the form object through 
                 multiple inheritance.
                  
*/ 
class Artisan_List:  public list_base
{
public: 
  // Constructor
  Artisan_List(cgiScript&sc):list_base(sc){;}  
  ~Artisan_List(){;}
  
  bool list_display( void )
  {
    bool breturn = true;
    string sql = "select Id, First_Name, Last_Name from Person p inner join Artisan a"
                 " on p.Id = a.Person_Id";   
    sql += " order by Last_Name, First_Name";
    editLink = "<a href='artisan_form.cgi?Id=$key$'>Edit</a>";
    string heading = " [Id] | First| Last | <a href='artisan_form.cgi'>New</a>";
    emitHeadings(heading);
    getData( sql );
    emitData();
    emitEnd();
    return breturn;
  }      
}; 
class Artisan_Form:  public Artisan, public forms_base
{
public:
 
  // Constructor
  Artisan_Form(cgiScript & in):Artisan(),forms_base(in){setKey(*this);}  
  ~Artisan_Form(){;}
  // Implement pure virtual methods of form_base
  void form_id_transfer( void )
  {
    llongFXfer( "Id", Id );
  }
  void form_data_transfer( void )
  {
    // person
    stringFXfer( "First_Name", First_Name );
    stringFXfer( "Last_Name", Last_Name );
    stringFXfer( "Login",  Login);
    stringFXfer( "Password", Password );
    stringFXfer( "Address",  Address);
    stringFXfer( "City", City );
    stringFXfer( "State", State );
    stringFXfer( "Zip", Zip );
    stringFXfer( "Phone", Phone );
    stringFXfer( "EMail", EMail );    
    // artisan 
    llongFXfer( "User_Id", User_Id );
    llongFXfer( "Artisan_Group_Id", Artisan_Group_Id );
    stringFXfer( "Biography", Biography ); 
    llongFXfer( "Specialty", Specialty );
    llongFXfer( "SubSpecialty", SubSpecialty );
    stringFXfer( "image", image ); 
  } 
  bool dbf_action( string mode, changeMap & changes )
  {
    return db_action( mode, changes );
  } 
  // implement pure virtual of form display
  bool form_display( void )
  {
    bool breturn = true;
    script  << makeTop( "artisan_form.cgi", "Artisan" )
            << makeStaticBox("Id","Id",Id,"12")
            << makeTextBox("First_Name","First_Name",First_Name,"35")
            << makeTextBox("Last_Name","Last_Name",Last_Name,"35")
            << makeTextBox("Login","Login",Login,"12")
            << makePasswordBox("Password","Password",Password,"12")
            << makeTextBox("Address","Address",Address,"35")
            << makeTextBox("City","City",City,"35")
            << makeTextBox("State","State",State,"2")
            << makeZipBox("Zip","Zip",Zip,"10")
            << makeTextBox("Phone","Phone",Phone,"20")
            << makeEmailBox("EMail","EMail",EMail,"60")          
            << makeComboBox("Administrator ID","User_Id"
                            ,User_Id
                            ,"select Id, concat( first,' ',last) from users"
                            ,"Select only if administrator") 
            << makeComboBox("Artisan Group","Artisan_Group_Id"
                            ,Artisan_Group_Id
                            ,"select Id, Name from Artisan_Group"
                            ,"Select Group (if any)")    
            << makeTextArea("Biography","Biography",Biography,"15","80")  
            << makeComboBox("Specialty","Specialty"
                            ,Specialty
                            ,"select Id, Name from Category"
                            ,"Select Artisan General Specialty") 
            << makeComboBox("Specific Specialty ","SubSpecialty"
                            ,SubSpecialty
                            ,"select sc.Id, concat(c.Name, ' - ', sc.name) from Category c "
                            "inner join Sub_Category sc on c.Id=sc.Category_Id"
                            ,"Select Artisan Specific Specialty")
            << makeImageBox("image","image",image,"35")                 
            << makeButtons( key() )
            << makeBottom( m_result ) << endl; 
    return breturn;
  }      
};


int main( int argc, char ** argv )
{
  cgiScript script("text/html",false);
  Artisan_Form  artist(script);
  if( oLogin.testLoginStatus() )
  {
    script.closeHeader();   
    cgiTemplates pgTemplate;
    pgTemplate.load("Templates/adminPane.htmp");
    script << ocString(pgTemplate.getParagraph("top"))
              .replaceAll("$heading$","Artisan Editor")
              .replace("$menu$",getNavMenu().c_str())
              .replace("$instructions$",
                    "You may insert, update or delete an "
                    "artisan here.");


    artist.loadControlTemplates("Templates/form.htmp");  
    artist.form_action();  
    artist.form_display();
    script << pgTemplate.getParagraph("bottom");
  }
  else
  {
    showSignOnForm(script); 
  } 
};

// as an alternative to building more objects
#include "read_write_base.cpp"
#include "forms_base.cpp"

