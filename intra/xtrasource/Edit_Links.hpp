/* 
  Edit_Links.hpp

  Object Definition and Implementation for Edit_Links.
  
  These classes are built by w3sys RAD tools
  to be compatible with STL, and to be derivable
  
  Copyright(c)2007 - D.K. McCombs.
  W3 Systems Design - davidmc@w3sys.com
                      http://www.w3sys.com  

*/  
#ifndef Edit_Links_HPP
#define Edit_Links_HPP


// yes a global variable
cgiTemplates editLinkCollection;

void editLinksInit( void )
{
  // get the link from the edit collection
  if( editLinkCollection.getUnparsedHtml().length() == 0 )
  {
    editLinkCollection.load("./Templates/DefaultEditLinks.htmp");
    // DBG  cout << "GOOD LOAD of ./Templates/DefaultEditLinks.htmp " << endl;
  }
}
void fixEditLinks( ocString items,  page & apg)
{
  paragraphMap & paras  = apg.get_template().getParagraphs();

  // For any of the items in this list
  items.parseInit();
  string item = items.parse(",");
  while( item.length() )
  {
    // find a paragraph from the page template
    paragraphMap::iterator pos;
    pos = paras.find(item);
    if( pos != paras.end() )
    {
      // get the paragraph, find any insert points
      ocString outTemp = pos->second;
      string workspace  =  outTemp.parse("<!--@insert_before-->");
      workspace +=  "<!--@insert_before-->";
      if( outTemp.endOfParse() == false )
      {
        string editStuff = outTemp.parse("<!--/@insert_before-->");
        
        if( editStuff == "[INSERT]" )
        {
          workspace += editLinkCollection.getParagraph("INSERTBEFORELINK");
          workspace += "<!--/@insert_before-->";
          workspace += outTemp.parse("<!--@edit-->");
          workspace += "<!--@edit-->";
          if( outTemp. endOfParse() == false )
          {
            editStuff = outTemp.parse("<!--/@edit-->");
            if( editStuff == "[EDIT]" )
            {
              workspace += editLinkCollection.getParagraph("EDITLINK");
              workspace += "<!--/@edit-->";
              workspace +=  outTemp.parse("<!--@insert_after-->");
              workspace += "<!--@insert_after-->";
              if(  outTemp.endOfParse() == false )
              {
                editStuff = outTemp.parse("<!--/@insert_after-->");
                if(  editStuff   == "[INSERT]" )
                {
                   workspace += editLinkCollection.getParagraph("INSERTAFTERLINK");
                   workspace += "<!--/@insert_after-->";
                }
                if(  outTemp.endOfParse() == false )
                {
                  workspace +=  outTemp.remainder();
                }
                pos->second =  workspace;
              }
            }
          }
        }  
      }
    }
    item = items.parse(",");
  }
}
#endif
