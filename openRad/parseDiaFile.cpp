/*
  parseDiaFile.cpp
  
  
*/
#include "cgiExtend.h"
#include "cgiCookie.h"
#include "cgiTemplates.h"
#include "ocFileSys.h"
 #include "ocXML.h"

using namespace std;

// Standard main function
int main(int argc, char ** argv)
{
  cgiScript script;
  cgiTemplates content;
  ocFileSys filesys;
  cgiInput & args = script.ClientArguments();
  if( content.load( "pickDiaFile.html" ) )
  {
    script << content.getParagraph("top");
    if( args.count("diafile") )
    {
      ifstream dia;
      dia.open( args["diafile"].c_str() );
      if( dia )
      {
        xmlParser parser(&dia);
        parser.useXPATH = true;
        parser.parse();
        node_vector & xnodes = parser.nodeList();
        for(int i=0;i<xnodes.size();i++)
        {
          xmlNode & node = xnodes[i];
          ocString xpMatch = node.xpath;
          if( xpMatch.regExMatch("\\/?xml\\/dia:diagram\\/dia:layer\\/dia:object") )
          {
            // In Object
            if( xpMatch.regExMatch("\\/?xml/dia:diagram\\/dia:layer\\/dia:object\\/dia:attribute\\/dia:composite\\/dia:attribute\\/dia:string") )
            {
              script << "--------------------- > Class Name Node: " << node.data << " < --------------- <br>" << endl;
            }
            script << "Node: " << node.xpath << "<br>" << endl;
              //   << " contains: " << node.data << "<br>" <<endl;
            cout << "node attributes" << endl << "=================<br>" << endl;
            node_attr::iterator x;
            for( x=node.attr.begin(); x!=node.attr.end(); ++x)
            {
              script << x->first << " = " << x->second <<  "<br>" << endl;
            }
            cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX<br>" << endl;
          }
        }
      }
    }
    else
    {
      script << content.getParagraph("form");
    }
    script << content.getParagraph("bottom"); 
  }

  return 0;
}
