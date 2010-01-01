/*
  Copyright (c) 2003  - 2006
  imagePick.cpp 
  Author David McCombs -- www.w3sys.com
  
  NEEDS: Templates/imagelist.htmp  
*/

#include "cgiTemplates.h"
#include "connectstring"

#include "siteLimit.h"
#include "read_write_base.hpp"

// #define DO_OPEN_LOGGING
#include "openLogger.h"
#include <Magick++.h>
#include "ocFileSys.h"
class rev_date_ocDirEntry 
{
public:
  bool operator ( ) ( const ocDirEntry & d1, const ocDirEntry & d2 )
  {    
    return d1.last_mod > d2.last_mod;
  }
};
rev_date_ocDirEntry rev_dates;

using namespace Magick;
class imagePicker : public read_write_base
{
private:
  string m_imageBase;
  string m_thumbBase;
  string m_imageDir;
  string m_thumbDir;
  string m_domain;
  string m_urlDir;
  string m_urlThumbDir;
  string m_fileUploaded;

  // bounding rectangle dimeinsions for thumbnail
  int boundX; 
  int boundY;
  int FSboundX; 
  int FSboundY;
  
  // Quality and Filter Type
  FilterTypes filter;
  unsigned int quality;

public:
  // the relative path, to either a tumbnail or fullsized image
  string m_relPath;
  bool m_isBasePathed;
  
  // constructor  
  imagePicker(openLogin & login)
  :read_write_base(),m_imageBase("images"),m_thumbBase("small_images")
  ,boundX(120),boundY(120),FSboundX(0),FSboundY(0),m_isBasePathed(true)
  ,filter(LanczosFilter),quality(75)
  {
    string siteLimitation = siteFocus();
    string sql = "select path, domain, url from sites";
    if( siteLimitation.length() )
    {
      sql += " where id ";
      sql += siteLimitation;
    }
    writelog2( "SQL to get site path", sql ); 
    
    if( rs.open(sql ) )
    {
      m_imageDir = rs.getField(0).format();
      m_thumbDir = rs.getField(0).format();
      m_domain = rs.getField(1).format();
      m_urlDir = rs.getField(2).format();
      m_urlThumbDir = rs.getField(2).format();
      m_imageDir += "/";
      m_thumbDir += "/";
      // m_urlDir += "/";
      m_imageDir += m_imageBase;
      m_thumbDir += m_thumbBase;
      // m_urlDir += m_imageBase;
      m_urlThumbDir += m_thumbBase;
      rs.close();
    }
  }
  virtual ~imagePicker(){;}

  void checkForImagesToDelete(cgiScript & script)
  {
    cgiInput & args = script.ClientArguments(); 
    if( args.count("delete") > 0 && args["delete"].length() > 0)
    {
      string delPath = m_imageDir;      
      delPath +="/";
      delPath += args["delete"].c_str();
      string delThumbPath = m_thumbDir;
      delThumbPath +="/";
      delThumbPath += args["delete"].c_str();            
      ocFileSys fs;
      writelog2(  "Deleting ", delPath );
      // remove the image
      if( !fs.remove(delPath) )
      {
        script << fs.check();
        script << " image path: " << delPath << endl;
      }
      // remove the thumbnail
      if( !fs.remove(delThumbPath) )
      {
        script << fs.check();
        script << " thumb path: " << delThumbPath << endl;
      }
    }
  }
  
  bool isFile( ocString candidate )
  {
    bool bret;
    bret = candidate.regExMatch(".[a-z,A-Z]");    
    return bret;
  }
  
  void  imageList(cgiScript & script)
  {
    cgiInput & args = script.ClientArguments(); 
    
    cgiTemplates htmlDoc;
    writelog("Loading Templates/imagelist.htmp");
    htmlDoc.load("Templates/imagelist.htmp");
    char * Rows[2] = { "oddcell", "evencell" };
    writelog("Writing top of page");
    script << ocString(htmlDoc.getParagraph("top"))
                .replace( "__REPORT_TITLE_GOES_HERE", "Image Picker" )
                .replaceAll( "$relPath$", m_relPath.c_str() )
                .replaceAll( "$noBasePath$", m_isBasePathed ? "":"true" )
           << endl;
    script << "Path: " <<  m_thumbDir << "<BR>" << endl;      
    ocString headitem = htmlDoc.getParagraph("headitem");
    string rowsep = htmlDoc.getParagraph("rowsep");

    writelog("Writing headers");
    
    script << headitem.replace("__hcell_label","Image");
    script << headitem.replace("__hcell_label","Name"); 
    script << headitem.replace("__hcell_label","Action");       
    script << headitem.replace("__hcell_label","Delete");
    
    ocString iLnk = "<img src='_img_'>";

    ocString fLnk = "<a href='javascript:pickImage(\"_img_\")'>Pick</a></div>";

    ocString delLink = "<a href='javascript:deleteImage(\"_img_\")'>"
                      "[delete]"
                      "</a>";
    ocFileSys fs;
    writelog2("Scanning image Directory",m_thumbDir);
    if( fs.openDir(m_thumbDir) )
    {      
      ocDirectory & entries = fs.getDirectoryEntries();
      
      sort( entries.begin(), 
            entries.end(), 
            rev_dates );
      
      // new code to limit the view      
      string filter;
      if(  args.count("filter") ) filter = args["filter"].c_str();      
      int iEntries = entries.size();
      int iMax = iEntries?10:0;
      bool filesFound = false; 
      bool showAll = args.count("showall")?true:false;                
      if( filter.length() ) showAll = true;
      
      for( int iRow = 0;
           ( showAll && iRow < iEntries) ||
           ( filesFound && iRow < iMax && iRow < iEntries ) ||
           ( !filesFound && iRow < iEntries );
           ++iRow )
      {
        if( isFile(entries[iRow].name) ) // make sure it's a file
        {
          if( filter.length() )
          {
            ocString testString = entries[iRow].name;
            if( testString.regExMatch(filter.c_str()) == false)
            {
              continue;
            }
          }
          filesFound = true;
          string imgPath = m_imageDir;
          imgPath +="/";
          imgPath += entries[iRow].name;
          
          // base relative image link is the list link for picking an image
          string imgLink;
          if(m_isBasePathed)
          {
            imgLink = m_urlDir;     
            imgLink += m_relPath;
          }
          imgLink += entries[iRow].name; 
          
          // absolute link to thumbnail
          string absThumb = "http://";       
          absThumb += m_domain; 
          absThumb += m_urlThumbDir;
          absThumb += "/";
          absThumb += entries[iRow].name;                           

          ocString row = htmlDoc.getParagraph(Rows[ iRow % 2 ] );
          script << rowsep;
          
          script << row.replace( "__cell_data",
                                 iLnk.replace( "_img_",
                                              absThumb.c_str() 
                                              ).c_str() );
          script << row.replace( "__cell_data", entries[iRow].name.c_str() );                                      
          script << row.replace( "__cell_data",
                                 fLnk.replace( "_img_",
                                              imgLink.c_str()
                                              ).c_str() ); 
          script << row.replace( "__cell_data",
                                delLink.replaceAll( "_img_",
                                                    entries[iRow].name.c_str()
                                                  ).c_str() );
        }
      } // end for
    }
    script << htmlDoc.getParagraph("bottom") << endl;
  }
  string imageDir( void )
  {
    return m_imageDir;
  }
  string imageBase( void )
  {
    return m_imageBase;
  }
  
  void makeThumb( cgiScript & script )
  {    
    if( script.RequestMethod().upper() == "POST" && script.ClientArguments().count("File") )
    {
      
           
      cgiInput & args = script.ClientArguments();
      if( args.count( "boundX" ) )
      {
        boundX = atoi(script.ClientArguments()["boundX"].c_str());
      }
      if( args.count( "boundY" ) )
      {
        boundY = atoi(script.ClientArguments()["boundY"].c_str());
      }
      // Add loop here 
      ocString files = script.ClientArguments()["File"].c_str();;
      m_fileUploaded = files.parse("|");
      while (m_fileUploaded.length())
      {
        string imgPath = m_imageDir;
        imgPath +="/";
        imgPath += m_fileUploaded;
        string smlPath = m_thumbDir;
        smlPath +="/";
        smlPath += m_fileUploaded; 
        try 
        {          
          Image image; 
          // read the image in
          image.read(imgPath);
          // set the scaling geometry   
          Geometry geom( boundX, boundY );      
          // only scale images that are greater in size
          geom.greater(true);      
          // do not change the aspect ratio
          geom.aspect(false);           
          
          // set sizing parmeters.
          image.filterType(filter);
          image.quality(quality);
          
          // use scale method to resize
          image.scale(geom);               
          // write the small image out
          image.write(smlPath); 
        }
        catch( Exception &error_ )
        { 
          script << "Error creating thumbnail - Exception caught: " << error_.what() << endl;      
        }
        
        // get next if any
        m_fileUploaded = files.parse("|"); 
      }
    }  
  }
  void setArgs( cgiScript & script )
  {    
    if( script.ClientArguments().count("FSboundX") &&
        script.ClientArguments().count("FSboundY") )
    { 
      FSboundX = atoi(script.ClientArguments()["FSboundX"].c_str());
      FSboundY = atoi(script.ClientArguments()["FSboundY"].c_str());
      if( script.ClientArguments().count("Quality") )
      { quality = atoi(script.ClientArguments()["Quality"].c_str());;
      }
      if( script.ClientArguments().count("FilterType") )
      { filter = (MagickLib::FilterTypes) atoi(script.ClientArguments()["FilterType"].c_str());;
      } 
    }
    if( script.ClientArguments().count("boundX") &&
        script.ClientArguments().count("boundY") )
    { 
      boundX = atoi(script.ClientArguments()["boundX"].c_str()); 
      boundY = atoi(script.ClientArguments()["boundX"].c_str());
    }
  }
  void scaleSource( cgiScript & script )
  {    
    if( script.RequestMethod().upper() == "POST" && 
        script.ClientArguments().count("File") )
    { 
      
      // scale the image
      ocString files = script.ClientArguments()["File"].c_str();         
      FSboundX = atoi(script.ClientArguments()["FSboundX"].c_str());
      FSboundY = atoi(script.ClientArguments()["FSboundY"].c_str());
      // do loop 
      m_fileUploaded = files.parse("|");
      while (m_fileUploaded.length())
      {
        string imgPath = m_imageDir;
        imgPath +="/";
        imgPath += m_fileUploaded;

        try 
        {          
          Image image;
          // read the image in
          image.read(imgPath);
          // set the scaling geometry   
          Geometry geom( FSboundX, FSboundY );      
          // only scale images that are greater in size
          geom.greater(true);      
          // do not change the aspect ratio
          geom.aspect(false);
          
           // set sizing parmeters.
          image.filterType(filter);
          image.quality(quality); 
                      
          // use scale method to resize
          image.scale(geom);           
          // write the small image out
          image.write(imgPath); 
        }
        catch( Exception &error_ )
        { 
          script << "Error sizing Full Scale Image - Exception caught: " << error_.what() << endl;      
        }
        // get next if any
        m_fileUploaded = files.parse("|"); 
      }
    }  
  }
  
};

int main( int argcount, char ** args )
{
  // An instance of the script
  // must be created before any other output.
  // the last param automatically save images to the path
  openLogin login;
  imagePicker picker(login);  
  cgiScript script("text/html", false, picker.imageDir().c_str());
  writelog("OK - script made");
  // Attempt to get cookie
  if ( login.testLoginStatus() )
  {
    script.closeHeader();
    cgiInput & args = script.ClientArguments();
    // This is to specify the relative path, usually 'images' or 'small_images'
    if( args.count( "relPath" ) )
    {
      picker.m_relPath = args["relPath"].c_str();
    }
    
    // This is to pick JUST the image name by itself - no preceding image path
    if( args.count( "noBasePath" ) )
    {
      picker.m_isBasePathed = false;
    }
    
    // Perform image manipulation  
    picker.setArgs( script );  
    picker.makeThumb( script );
    picker.scaleSource( script );
        
    writelog("Checking for images to delete");    
    picker.checkForImagesToDelete(script);
    
    writelog("Generating image list");
    picker.imageList(script); 
       
    writelog("Done");
  }
  else
  {
    script.Redirect("signIn.html");
    script.closeHeader();
    writelog("INVALID ACCESS ATTEMPT");      
  }
  writelog("OK - All Done");
  return(0);
}

#include "read_write_base.cpp"

