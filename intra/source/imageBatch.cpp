/*
  Copyright (c) 2006
  imageBatch.cpp
  Author David McCombs -- www.w3sys.com


*/


// #define DO_OPEN_LOGGING
#include "openLogger.h"
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <Magick++.h>
#include "ocString.h"
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
class imageConditioner 
{
private:
  string m_imageBase;
  string m_imageDir;
  string m_thumbDir;

  // bounding rectangle dimeinsions for thumbnail
  int boundX; 
  int boundY;
  int FSboundX; 
  int FSboundY;
  
  // Quality and Filter Type
  FilterTypes filter;
  unsigned int quality;

public:

  // constructor  
  imageConditioner()
    :m_imageBase("./")
    ,boundX(120),boundY(120)
    ,FSboundX(510),FSboundY(510)
    ,filter(LanczosFilter),quality(75)
  {
     m_imageDir = "./images";
     m_thumbDir = "./small_images";
  }
  virtual ~imageConditioner(){;}

  void ImagesDelete( string imgDelete )
  {
      string delPath = m_imageDir;
      delPath +="/";
      delPath += imgDelete;
      string delThumbPath = m_thumbDir;
      delThumbPath +="/";
      delThumbPath +=imgDelete;
      ocFileSys fs;
      writelog2(  "Deleting ", delPath );
      // remove the image
      if( !fs.remove(delPath) )
      {
        cout << fs.check();
        cout << " image path: " << delPath << endl;
      }
      // remove the thumbnail
      if( !fs.remove(delThumbPath) )
      {
        cout << fs.check();
        cout << " thumb path: " << delThumbPath << endl;
      }
  }
  void setArgs(  int argcount, char ** args  )
  {
    /*   
    if( cout.ClientArguments().count("FSboundX") &&
    cout.ClientArguments().count("FSboundY") )
    {
    FSboundX = atoi(cout.ClientArguments()["FSboundX"].c_str());
    FSboundY = atoi(cout.ClientArguments()["FSboundY"].c_str());
    if( cout.ClientArguments().count("Quality") )
    { quality = atoi(cout.ClientArguments()["Quality"].c_str());;
  }
    if( cout.ClientArguments().count("FilterType") )
    { filter = (MagickLib::FilterTypes) atoi(cout.ClientArguments()["FilterType"].c_str());;
  }
  }
    if( cout.ClientArguments().count("boundX") &&
    cout.ClientArguments().count("boundY") )
    {
    boundX = atoi(cout.ClientArguments()["boundX"].c_str());
    boundY = atoi(cout.ClientArguments()["boundX"].c_str());
  }
    */
  }
  bool isFile( ocString candidate ) // is it an image really
  {
    bool bret;
    bret = candidate.regExMatch("[A-Z][a-z][0-9]*\x2e[bmpnjpgtif,BMPNJPGTIF]*");
    if( bret ) bret = !  candidate.regExMatch("imageBatch");
    return bret;
  }
  
  void  imageProcess(void)
  {

    cout << "Image Processing "<< m_imageBase << endl;


    ocFileSys fs;
    writelog2("Scanning image Directory",m_thumbDir);
    if( fs.openDir(m_imageBase) )
    {      
      ocDirectory & entries = fs.getDirectoryEntries();
      
      sort( entries.begin(), 
            entries.end(), 
            rev_dates );
      

      int iEntries = entries.size();
      int iMax = 0;
      bool filesFound = false; 
      bool showAll = true;
      
      for( int iRow = 0;
           iRow < iEntries ;
           ++iRow )
      {
        if( isFile(entries[iRow].name) ) // make sure it's a file
        {
          cout << endl << "  Processing :  " <<  entries[iRow].name << endl;
          makeThumb(entries[iRow].name);
          scaleSource(entries[iRow].name);
        }
      } // end for
    }
   
  }
  string imageDir( void )
  {
    return m_imageDir;
  }
  string imageBase( void )
  {
    return m_imageBase;
  }
  
  void makeThumb( string imgEntry  )
  {
    string m_fileUploaded = imgEntry;
    string imgPath = m_imageBase;
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
      cout << "Error creating thumbnail - Exception caught: " << error_.what() << endl;
    }
  }

  void scaleSource( string imgEntry )
  {
    string m_fileUploaded = imgEntry;
    if (m_fileUploaded.length())
    {
      string imgBasePath = m_imageBase;
      string imgPath = m_imageDir;
      imgPath +="/";
      imgPath += m_fileUploaded;
      imgBasePath += m_fileUploaded;

      try
      {
        Image image;
        // read the image in
        image.read(imgBasePath);
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
        cout << "Error sizing Full Scale Image - Exception caught: " << error_.what() << endl;
      }
    }
  }
};

int main( int argcount, char ** args )
{

  imageConditioner picker;
  picker.imageProcess();

  return 0;
}


