#include "cgiExtend.h"
#include "cgiCookie.h"
#include "cgiTemplates.h"
#include <fstream>

using namespace std;


// Standard main function
int main(int argc, char ** argv)
{
  cgiScript script;
  cgiTemplates content;
  cgiInput & args = script.ClientArguments();
  ifstream clsFile;
  if( content.load( "pickClass.html" ) )
  {
    ocString top = content.getParagraph("top");
        
    if( args.count("file") )
    {
      script << top.replace("_file_",args["file"].c_str());
      ocString option = content.getParagraph("class");
      string path = "./ClassFiles/";
      path += args["file"].c_str();
              
      clsFile.open( path.c_str() );
      while(clsFile)
      {
        ocString line;
        getline( clsFile, line );
        if( line.regExMatch("^class") )
        {
          line.parse(" ");
          script << option.replaceAll("_class_",line.parse(" ").c_str());          
        }
      
      }
    }
    script << content.getParagraph("bottom"); 
  }
  else
  {
     script << "Error loading web page template!";
  } 
    
  return 0;
}
