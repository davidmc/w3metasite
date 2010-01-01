#ifndef OPEN_CORE_RICHFORM_H
#define OPEN_CORE_RICHFORM_H 123456

#include "forms_base.hpp"
class ocRicharea: public ocBase
{

public:
  string ctrlName;
  ocString ctrlTemplate;

  
  ocRicharea(string ctrlTemplate, string name):ctrlTemplate(ctrlTemplate),ctrlName(name)
  {
    ;
  }
  virtual string getHtml( void )
  {
    string result;
    if( ctrlTemplate.length() > 0 )
    {       
      result = ctrlTemplate.replace("$name",ctrlName.c_str()).replace("$content",content.c_str());
    }
    return result;
  }
  ~ocRicharea(){;}
};

ocRicharea * richEntry( string ctrlTemplate, string name = "content" )
{
  ocRicharea * ra = new ocRicharea(ctrlTemplate,name);
  return ra;
}

class richForm: public forms_base
{

public:
  string tmpltPath;
  string ctrlName;
  cgiTemplates ctrlTemplate;
    
  richForm( cgiScript & in, string templatePath = "Templates/FullyRichEditor.html" ):forms_base(in)
  {
    tmpltPath = templatePath;
    if(tmpltPath.length())  ctrlTemplate.load(tmpltPath.c_str());
  }
  string makeRichEntry( string label, string name,  string value ) 
  {
    ocString ret = formTemplate.getParagraph("wide_group");    
    ocRicharea * pRich = richEntry( ctrlTemplate.getParagraph("control") ,  name );
    if( pRich )
    {
      pRich->setContent(value);      
      ret = ret.replace("$label$",label.c_str())
               .replace("$control$", pRich->getHtml().c_str());
      delete pRich;
    }
    return ret;
  }
  // encode
  string htmlFixup(string in)
  {
    ocString content(in);     
    return content.replaceAll("\"","`22;")
                  .replaceAll("'","`27;") 
                  .replaceAll("+","`29;")
                  .replaceAll("<","`3c;")
                  .replaceAll(">","`3e;")
                  .replaceAll("&","`26;");                                
  }
  // decode
  string htmlDecode(string in)
  {    
    ocString content(in);    
    return content.replaceAll("`22;","\"")
                  .replaceAll("`27;","'") 
                  .replaceAll("`29;","+")
                  .replaceAll("`3c;","<")
                  .replaceAll("`3e;",">")
                  .replaceAll("`26;","&");
  }
};

#endif
