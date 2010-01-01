/*

 filePick.hpp
 definitiion and implementation of the 
 filePick class
 default path is 'Template'.
 override by calling filePicker::fileBase('new path')

NEEDS:
   "Templates/filelist.htmp"

*/ 

#ifndef File_Pick_hpp
#define File_Pick_hpp

#include "cgiTemplates.h"
// #include "connectstring"
#include "ocFileSys.h"

// #define DO_OPEN_LOGGING
#include "openLogin.h"
#include "openLogger.h"

class datesort_ocDirEntry
{
public:

  /* sort by date */
  bool operator ( ) ( const ocDirEntry & d1, const ocDirEntry & d2 )
  {
    return d1.last_mod > d2.last_mod;
  }

};
datesort_ocDirEntry datesort;
namesort_ocDirEntry namesort;

class filePicker
{
private:
  string m_fileBase;

  string m_fileDir;

  string m_domain;
  string m_urlDir;
  string m_exe_name;
  
  string m_fileUploaded;
  ocString fLnk;
  ocString sLnk ;
  ocString delLink;
  ocFileSys fs;
  string lastOfToken;
  
public:
  ocString debug;
  
public:
  filePicker(openLogin & login, string base="Templates", string exe_name="filePick.cgi", string lastOfTokenIn="/")
  :m_fileBase(base),m_exe_name(exe_name)
  ,fLnk("<a href='javascript:pickfile(\"_file_\")'>Pick</a> &middot; ")
  ,sLnk("<a href='javascript:showfile(\"_file_\")'>Show</a>")
  ,delLink("<a href='javascript:deletefile(\"_file_\")'>"
                      "[delete]"
                      "</a>")
  ,lastOfToken(lastOfTokenIn)
  {    
    init(login);
  }
  void FLink( string value )
  {
    fLnk = value;
  }
  void init(openLogin & login)
  { 
    try {
    ocString fullpath = getenv("DOCUMENT_ROOT");  // from the site config file
    if( fullpath.regExMatch("\\/$") == false ) fullpath += "/";  // make sure it has a trailing '/'
    m_fileDir = fullpath;
    
    debug = " fullpath: " + fullpath;
    debug += "<br>\r\n lastOfToken: " + lastOfToken;
    
    m_domain = getenv("SERVER_NAME");
    string scriptname= getenv("SCRIPT_NAME");
    m_urlDir = "";
    
    debug += "<br>\r\n scriptname: " + scriptname;
    
    // take off the executible part
    string::size_type pos = scriptname.rfind(lastOfToken);
    if( pos  != string::npos )
    {
      m_urlDir = scriptname.substr(0,pos);
    }
    debug += "<br>\r\n m_urlDir: " +  m_urlDir;
    debug += "<br>\r\n pos: ";
    debug.append(pos);
    
    pos = fullpath.rfind(lastOfToken);
    if( pos  != string::npos )
    {
      m_fileDir = fullpath.substr(0,pos) + m_urlDir;
    }
    debug += "<br>\r\n  m_fileDir: " + m_fileDir;
    debug += "<br>\r\n pos: ";
    debug.append(pos);
    
    m_fileDir += "/";
    m_urlDir += "/";
    
    m_fileDir += m_fileBase;
    m_urlDir += m_fileBase;
    debug += "<br>\r\n  final m_fileDir: " + m_fileDir;
    debug += "<br>\r\n  final m_urlDir: " + m_urlDir;
    }
    catch (...)
    {
      debug += "<br>\r\n Error! exception caught!";
    }

  }  
  virtual ~filePicker(){;}

  void checkForfilesToDelete(cgiScript & script)
  {
    cgiInput & args = script.ClientArguments(); 
    if( args.count("delete") > 0 && args["delete"].length() > 0)
    {
      string delPath = m_fileDir;      
      delPath +="/";
      delPath += args["delete"].c_str();

      writelog2(  "Deleting ", delPath );
      // remove the file
      if( !fs.remove(delPath) )
      {
        script << "Error Deleting " << delPath << " - " << fs.check();
      }
    }
  }
  
  bool isFile( ocString candidate )
  {
    bool bret;
    bret = candidate.regExMatch(".[a-z,A-Z]");    
    return bret;
  }
  
  void  fileList(cgiScript & script, string title="File In: " )
  {
    cgiInput & args = script.ClientArguments(); 
    
    cgiTemplates htmlDoc;
    title += m_urlDir; // fileDir();
    
    writelog("Loading Templates/filelist.htmp");
    htmlDoc.load("Templates/filelist.htmp");
    char * Rows[2] = { "oddcell", "evencell" };
    writelog("Writing top of page");
    script << ocString(htmlDoc.getParagraph("top"))
              .replace( "__REPORT_TITLE_GOES_HERE", title.c_str() )
              .replaceAll( "filePick.cgi", m_exe_name.c_str() )
           << endl;
    ocString headitem = htmlDoc.getParagraph("headitem");
    string rowsep = htmlDoc.getParagraph("rowsep");

    writelog("Writing headers");
    
    script << headitem.replace("__hcell_label","File");
    script << headitem.replace("__hcell_label","Action");       
    script << headitem.replace("__hcell_label","Delete");
    


    if( fs.openDir(m_fileDir) )
    {      
      ocDirectory & entries = fs.getDirectoryEntries();

      if(args.count("File") > 0) // show the uploaded file first
      {
        sort( entries.begin(), 
              entries.end(), 
              datesort );
      }
      else  // default to alphnumeric sort
      { 
        sort( entries.begin(), 
              entries.end(), 
              namesort );
      }
      // new code to limit the view      
      int iMax = entries.size()?entries.size():0;
      if( iMax > 20 ) iMax = 20; // default to limit show to 20 items
      bool fileFound = false;
      if( args.count("showall") )  iMax = entries.size();
      
      for( int iRow = 0; 
           iRow < iMax || (!fileFound && iRow < entries.size()); 
           ++iRow )
      {
        
        if(isFile(entries[iRow].name)) // make sure it's a file
        {
          fileFound = true;
          string imgPath = m_fileDir;
          imgPath +="/";
          imgPath += entries[iRow].name;
          string imgLink = entries[iRow].name;
          string relLink = m_urlDir;
          relLink +="/";
          relLink += entries[iRow].name;
          ocString row = htmlDoc.getParagraph(Rows[ iRow % 2 ] );
          string actions = fLnk.replace( "_file_", imgLink.c_str());
          actions += sLnk.replace( "_file_",  relLink.c_str() );
          script << rowsep;          

          script << row.replace( "__cell_data", entries[iRow].name.c_str() );                                      
          script << row.replace( "__cell_data", actions.c_str() ); 
          script << row.replace( "__cell_data",
                                delLink.replaceAll( "_file_",
                                                    entries[iRow].name.c_str()
                                                  ).c_str() );
          script << endl;
        }
      } // end for
    }
    script << htmlDoc.getParagraph("bottom") << endl;
  }
  string fileDir( void )
  {
    return m_fileDir;
  }
  string fileBase( void )
  {
    return m_fileBase;
  }
  void fileBase( string in )
  {
    m_fileBase = in;
  }  
};

#endif
