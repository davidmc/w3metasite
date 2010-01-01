#include "page_control.hpp"
/*

  Show Control:
  =============
  control tags
  
  detail
  ------
      attr:
      -----
      item := the template section
      for_slide_items := the matching slides for this detail
      mutex := whether or not presentation is mutually exclusive with arg s
      replace_tag := the replacement tag
  
  replacements made
      $selectIndicator with " id='user_selected_slide' " if selected otherwise empty string
      "$Id" with the order of the paragraph, used to set argument 's'     
      replace_tag with the paragraphs first image.
      $label with the paragraphs name
      
  slide
  -----
      attr:
      -----
      items
      replace_tag
      mutex
      
  replacements made:

*/

// Only holds valid slide paragraphs.
typedef vector<int> slide_selector;

class show_page : public page 
{
 slide_selector slides;
public:
  show_page(cgiScript & in):page(in)
  {
    ;
  }
  virtual ~show_page()
  {
    ;
  }
  
  // Invoke after page load but before emit
  bool loadSlides( ocString tags )
  {
    bool bret = true;
    page_paragraphs & pg_paras = get_page_paragraphs();
    for( page_paragraphs::size_type szt=0; szt < pg_paragraphs.size(); ++szt )
    {
      page_paragraph & para = pg_paragraphs.at(szt);
      if( tags.regExMatch( para.template_tag().c_str() ) )
      {
        slides.push_back(szt);
      }
    }    
    return bret;
  }
  
  // special purpose functions for slideshows 
  virtual bool emitDetail( const string & for_slide_items, string contentTags, string replaceTag )
  {
    bool bRet = true;
    paragraphMap & paras  = pg_template.getParagraphs();
    int slideNo = 0;
    string qryString = script.QueryString().c_str();
    cgiInput & args = script.ClientArguments();
    if( args.count("s") )
    {
      slideNo = atoi( args["s"].c_str() );
    }
    paras_emitDetail(get_page_paragraphs(),paras,contentTags,for_slide_items,replaceTag,slideNo);                         
    return bRet;                  
  }
  void paras_emitDetail( page_paragraphs & pg_paragraphs, // the DB's stored paragraphs
                         paragraphMap & paras, // the .html template's set of paragraphs
                         ocString templateTags, // the paragraph retrieval key(s)
                         ocString for_slide_items, // the associated slide key
                         string replace_tag, // from client arg 's'
                         int slideNo )
  {
    templateTags.parseInit();    
    do 
    {
      string tag = templateTags.parse(",");
      if( tag.length() )
      { // slide_selector slides       
        for( slide_selector::size_type szt=0; szt < slides.size(); ++szt )
        {
          page_paragraph & para = pg_paragraphs.at(slides.at(szt));
          
          if( for_slide_items.length() ) // if the detail references slides...          
          { // new method, very flexible, points a detail to a particular slide             
            if( for_slide_items.regExMatch(para.template_tag().c_str()) &&
                szt==slideNo)
            {            
              para_emitDetail(para, paras,replace_tag,
                              tag, slideNo,szt,slides.size());
            }          
          }
          else // backward compat where detail was used to build page instead of slides
          {        
            if( para.template_tag() == tag  &&
                szt==slideNo )
            {
              para_emitDetail(para, paras,replace_tag,
                              para.template_tag(), slideNo,szt,slides.size());
            }
          }  
        }
      }      
    } while( !templateTags.endOfParse() );
  }
  void para_emitDetail( page_paragraph & para, 
                        paragraphMap & paras, 
                        const string & replace_tag,
                        const string & template_key,
                        int selSlide, 
                        int sztPos, 
                        int arrySize )
  {
    paragraphMap::iterator pos = paras.find(template_key);
        
    int iLast = arrySize?arrySize-1:0;
    int iPrev = selSlide?sztPos-1:iLast;
    int iNext = arrySize?(sztPos+1)%arrySize:0;
    ocString First, Previous, Next, Last;
    First.append(0);
    Previous.append(iPrev);
    Next.append(iNext);
    Last.append(iLast);

    if( pos != paras.end() )
    {      
      ocString output = pos->second;
      
      script << 
        output.replace( replace_tag.c_str(),
                        para.content().c_str() )
              .replaceAll("$Idf", First.c_str())
              .replaceAll("$Idp", Previous.c_str())
              .replaceAll("$Idn", Next.c_str())
              .replaceAll("$Idl", Last.c_str());
    }    
  }
  virtual bool emitSlides( string contentTags, string replaceTag )
  {
    bool bRet = true;
    paragraphMap & paras  = pg_template.getParagraphs();
    int slideNo = 0;
    cgiInput & args = script.ClientArguments();    
    if( args.count("s") )
    {
      slideNo = atoi( args["s"].c_str());
    }
    paras_emitSlides(get_page_paragraphs(),paras,contentTags,replaceTag,slideNo);                         
    return bRet;                  
  }
  void paras_emitSlides( page_paragraphs & pg_paragraphs, 
                        paragraphMap & paras, 
                        ocString templateTags, 
                        string replace_tag, 
                        int SlideNo )
  { // slide_selector slides    
    for( slide_selector::size_type szt=0; szt < slides.size(); ++szt )
    {
      page_paragraph & para = pg_paragraphs.at(slides.at(szt));
      para_emitSlide(para,paras,templateTags,replace_tag,SlideNo,szt);                    
    }        
  }
  void para_emitSlide( page_paragraph & para, 
                        paragraphMap & paras, 
                        ocString templateTags, 
                        const string replace_tag, 
                        int SlideNo, 
                        int sztPos )
  {
    templateTags.parseInit(); 

    /* OK, Here is another backward compatiblility, we want to emit a slide 
      regardless of whether we find a match, but we want to try to get a match first.
      So I'll add a variable called isMatchFound, if none, then use the first tag
    */
    bool isMatchFound=false;
    bool isSelected = SlideNo==sztPos;      
    string firstTag,tag;  
    do {    
      tag = templateTags.parse(",");
      if(firstTag.length()==0) firstTag=tag;

      if( tag.length() && para.template_tag() == tag )
      { isMatchFound = true;
        paragraphMap::iterator pos = paras.find(tag);
        if( pos != paras.end() )
        {
          slideEmission(para,replace_tag,SlideNo,sztPos,pos->second,isSelected);
        }
      }
    } while( !templateTags.endOfParse() );
    if( isMatchFound == false )
    {
      // emit something!
      paragraphMap::iterator pos = paras.find(firstTag);
      if( pos != paras.end() )
      {
        slideEmission(para,replace_tag,SlideNo,sztPos,pos->second,isSelected);
      }  
    }   
  }
  void slideEmission( page_paragraph &para,
                      const string replace_tag, 
                      int SlideNo, 
                      int sztPos,
                      ocString output, bool isSelected )
  {
    ocString parseContent = para.content();
    string selectIndicator;
    if( isSelected )
    {
      selectIndicator = " id='user_selected_slide' ";
    }
    // find an image tag
    if( parseContent.find("<img") != string::npos ) parseContent.parse("<img");      
    else parseContent.parse("<IMG");  
    if( !parseContent.endOfParse() ) // found one!
    {
      ocString uniqueId;
      ocString image = "<img";
      image += parseContent.parse(">");
      if( image.find("small_images") == string::npos &&
          image.find("images") != string::npos )
      {
        image = image.replace("images","small_images");
      }
      image += " alt='";
      image += para.name();
      image += "'>";
      uniqueId.append(sztPos);
      
      script << 
        output.replace("$selectIndicator",selectIndicator)
              .replaceAll( "$Id",uniqueId.c_str() ) 
              .replaceAll( replace_tag.c_str(),
                          image.c_str() )
              .replaceAll( "$label", para.name().c_str() ); 
    }
  }


  // end special purpose functions  
  
};


// global method
bool isMutex( xmlNode & node )
{
  // does the node have a mutex attribute?
  node_attr::iterator x = node.attr.find("mutex");
  // if it's there, I dont care about the value, return true...
  return (x!=node.attr.end());
}

class detail_functor: public base_functor
{
  cgiScript & script;
public:
  
  detail_functor(page & ipg,cgiScript & script):base_functor(ipg),script(script){;}
  
  virtual ~detail_functor(){;}
  
  show_page & showpg(void) { return dynamic_cast<show_page &>(this->pg); }
  
  bool shouldShow(xmlNode & node )
  {
    bool ret=true; // assume we should
    if( isMutex(node) )
    {
      if( script.ClientArguments().count("s") == 0 )
        ret=false; // since we're mutually exclusive, and we have a detail, dont show
    }
    return ret;
  }
  
  virtual  bool operator()( xmlNode & node )
  {
    bool bRet = false;
    
    if( shouldShow( node ) )
    {
      string for_slide_items, items, replace_tag;
      node_attr::iterator x = node.attr.find("for_slide_items"); // detail for what slide?
      if( x!=node.attr.end() ) for_slide_items = x->second;
      x = node.attr.find("item"); // new default suppresses show on admin pane
      if( x==node.attr.end() ) x = node.attr.find("items"); // backward compat
      if( x!=node.attr.end() )
      {
        items=x->second;
        x = node.attr.find("replace_tag"); // preferred
        if( x==node.attr.end() ) x = node.attr.find("replace-tag"); // backward compat   
               
        if( x!=node.attr.end())
        {
          replace_tag=x->second;
          bRet = showpg().emitDetail( for_slide_items, items, replace_tag );        
        }
        else
        {
          script << "no content replace tag" << endl;
        }       
      }
      else
      {
        script << "no content items" << endl;
      }
    }
    return bRet;    
  }
};


class slides_functor: public base_functor
{
  cgiScript & script;
public:
  slides_functor(page & ipg,cgiScript & script):base_functor(ipg),script(script){;}
  virtual ~slides_functor(){;}
  
  show_page & showpg(void) { return dynamic_cast<show_page &>(this->pg); }
  
  bool shouldShow(xmlNode & node )
  {
    bool ret=true; // assume we should
    if( isMutex(node) )
    {
      if( script.ClientArguments().count("s") )
        ret=false; // since we're mutually exclusive, and we have a detail, don't show
    }
    return ret;
  }
  
  virtual  bool operator()( xmlNode & node )
  {
    bool bRet = false;
    
    if( shouldShow( node ) )
    {
    
      string items, replace_tag;        
      node_attr::iterator x = node.attr.find("items");
      if( x!=node.attr.end() )
      {

        items=x->second;
        x = node.attr.find("replace_tag");  // preferred  
        if( x==node.attr.end() ) x = node.attr.find("replace-tag"); // backward compat
        if( x!=node.attr.end())
        {
          replace_tag=x->second;
          bRet = showpg().emitSlides( items, replace_tag );        
        }
        else
        {
          cout << "no content replace tag" << endl;
        }       
      }
      else
      {
        cout << "no content items" << endl;
      }
    }
    return bRet;    
  }
};
