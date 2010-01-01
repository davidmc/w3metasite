#include "cgiExtend.h"
#include "cgiCookie.h"
#include "cgiTemplates.h"

using namespace std;

int options( ifstream & clsFile, cgiScript & script, cgiTemplates & content)
{
  int ret = -1;
  string line;
  ocString option = content.getParagraph("fields");
  getline( clsFile, line ); // {
  while(  clsFile )
  { 
    ocString line;
    getline( clsFile, line ); // type name;
    if( line.regExMatch("}") == false )
    { 
      // this is better at tokenizing
      aString toker = line.c_str();
      string type = toker.token(" ;");
      string name = toker.token(" ;");
      type += " ";
      type += name;
      script << option.replace("_field_",type.c_str())
                      .replace("_field_",name.c_str());   
    }
    else
    {
      ret =0;
      break;
    } 
  }
  return ret;
}
// Standard main function
int main(int argc, char ** argv)
{
  int ret = -1;
  cgiScript script;
  cgiTemplates content;
  cgiInput & args = script.ClientArguments();
  ifstream clsFile;
  if( content.load( "pickFields.html" ) )
  {
    ocString top = content.getParagraph("top");
    if( args.count("file") && args.count("class") )
    {
      script << top.replace("_file_",args["file"].c_str())
                   .replace("_class_",args["class"].c_str()) ;

      string path = "./ClassFiles/";
      path += args["file"].c_str();
      string choseClass = args["class"].c_str();

      clsFile.open( path.c_str() );
      while(clsFile)
      { 
        ocString line; 
        getline( clsFile, line );
        if( line.regExMatch("^class") )
        {
          line.parse(" ");
          string className = line.parse(" ");
          if( className ==  choseClass )
          {
            ret = options( clsFile, script, content );
            break;
          }        
        }      
      }
    }
    script << content.getParagraph("bottom"); 
  }
  else
  {
     script << "Error loading web page template!";
  }     
  return ret;
}
