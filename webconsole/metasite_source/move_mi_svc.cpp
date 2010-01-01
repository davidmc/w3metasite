/* 
  move_mi_svc.cpp
  ========================
  Service for moving menu items  
  ========================    
  Schema:
  menu_item_movement:= result reason
  result:=OK, NO
  reason:=Success, At Limit
  
*/
#include "cgiClass.h"
#include "menu_item.hpp"

typedef map<string,menu_item_direction> directionmap;

// Standard main function
int main(int argc, char ** argv)
{ 
  // Tell the client to expect xml 
  cgiScript script("text/xml",true);
  cgiInput & args = script.ClientArguments(); 
  string response="NO", reason="NULL ARGS";
  
  // parts of the xml
  ocString xmlTmp = "<?xml version='1.0' encoding='UTF-8' standalone='yes'?>\n"
                    "<menu_item_movement result='$RES$' reason='$REAS$'>\n"
                    "$INFO$"
                    "</menu_item_movement>";
  ocString information;
  if( args.count("mid") && args.count("dir") )
  {
    menu_item mi;
    long long mid = atoll(args["mid"].c_str());
    directionmap dirmap;
    dirmap["u"]=up; 
    dirmap["b"]=btm;
    dirmap["l"]=lft;
    dirmap["r"]=rgt;
    dirmap["v"]=vabs;
    menu_item_direction dir = dirmap[args["dir"].c_str()];
    long abs_pos = 0;
    if( dir ==  vabs )
    {
      if( args.count("abs_pos") )
      {
        abs_pos = atol(args["abs_pos"].c_str());
      }
    }
    
    information = "<mid>";
    information.append(mid);
    information += "</mid><dir>";
    information.append(dir);
    information += "</dir>";
    if( mi.move( mid, dir, abs_pos ) )
    {
      response="OK";
      reason="Success";      
    }
    information+=mi.last_result();
    
  }
  script << xmlTmp.replace("$RES$",response.c_str())
                  .replace("$REAS$",reason.c_str())
                  .replace("$INFO$",information.c_str()); 
  return 0;
}
// compile implementations here
#include "read_write_base.cpp"
