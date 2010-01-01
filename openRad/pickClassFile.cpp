#include "cgiExtend.h"
#include "cgiCookie.h"
#include "cgiTemplates.h"
#include "ocFileSys.h"


using namespace std;


// Standard main function
int main(int argc, char ** argv)
{
  cgiScript script;
  cgiTemplates content;
  ocFileSys filesys;
  cgiInput & args = script.ClientArguments();

  if( content.load( "pickClassFile.html" ) )
  {
    script << content.getParagraph("top");
    if( filesys.openDir("./ClassFiles") )
    {
      ocDirectory & entries = filesys.getDirectoryEntries();
      namesort_ocDirEntry nmsrt;
      sort( entries.begin(), entries.end(), nmsrt );
      for( int iRow = 0; iRow < entries.size(); ++iRow )
      { 
        if((int)(entries[iRow].type)==8)
        {
          ocString option = content.getParagraph("files");
          script << option.replaceAll("_file_",entries[iRow].name.c_str());
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
