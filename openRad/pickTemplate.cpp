#include "cgiExtend.h"
#include "cgiCookie.h"
#include "cgiTemplates.h"
#include "ocFileSys.h"
#include "codeMap.hpp"

using namespace std;
// don't use global much
string file, class_name, fields, template_name;  


// Standard main function
int main(int argc, char ** argv)
{
  cgiScript script; 
  ocFileSys filesys;
  cgiTemplates content;
  cgiInput & args = script.ClientArguments();
  if(args.count("class")) class_name=args["class"].c_str();
  if(args.count("field")) fields=args["field"].c_str();
  if(args.count("file")) file=args["file"].c_str();
  if(args.count("template")) template_name=args["template"].c_str();      
  if( content.load( "pickTemplate.html" ) )
  {  
    ocString top = content.getParagraph("top");
    script << top.replace("_file_",file.c_str())
                 .replace("_class_",class_name.c_str())
                 .replace("_fields_",fields.c_str());
    if( filesys.openDir("./CodeTemplates") )
    {      
      ocDirectory & entries = filesys.getDirectoryEntries();
      namesort_ocDirEntry nmsrt;
      sort( entries.begin(), entries.end(), nmsrt );
      for( int iRow = 0; iRow < entries.size(); ++iRow )
      { 
        if((int)(entries[iRow].type)==8)
        {
          ocString option = content.getParagraph("templates");
          string current = entries[iRow].name;
          if(current==template_name)
          {
            
            script << option.replace("<option", "<option selected")
                            .replaceAll("_template_",current.c_str());
          }
          else
          {
            script << option.replaceAll("_template_",current.c_str());
          }
        }
      }
    }
    if( template_name.length() )
    {
      ocString bottom = content.getParagraph("bottom");
      string result;  
      codeMap gen;
      result += gen.genCode( class_name, fields, template_name);
      script << bottom.replace("_CODE_",result.c_str());
    }
    else
    {     
      script << content.getParagraph("bottom");
    }
  }
  else
  {
    script << "Error loading web page template!";
  }    
  return 0;
}
