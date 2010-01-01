/*
  ocFileSys.h
  class encapulation of posix file system


*/
#ifndef OC_FILE_SYS_H
#define OC_FILE_SYS_H

#include "ocString.h"
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <dirent.h>
#include <fstream>
#include <vector>
#include <string>



/*
  make a directory entry class
  just complete enough
  class to work with STL
*/
struct ocDirEntry
{
  string name;
  ino_t fileno;
  unsigned char type;
    
  mode_t file_mode;
  uid_t file_uid;
  gid_t file_gid;
  off_t file_size;
  time_t last_access;
  time_t last_mod;
  time_t last_statchg;

  ocDirEntry():fileno(0),type(0),
  file_mode(0),file_uid(0),file_gid(0),file_size(0),last_access(0),last_mod(0),last_statchg(0)
  {;}
  ocDirEntry(dirent * pin, string path):fileno(0),type(0),
  file_mode(0),file_uid(0),file_gid(0),file_size(0),last_access(0),last_mod(0),last_statchg(0)  
  {
    if(pin)
    {
      name = pin->d_name;
      fileno = pin->d_fileno;
      type = pin->d_type;      
      struct stat buf;
      path +="/";
      path += name;
      int ret = stat(path.c_str(), &buf);
      if( ret==0 )
      {
        
        file_mode = buf.st_mode;    //mode of file (see below)
        file_uid = buf.st_uid;     //user ID of file
        file_gid = buf.st_gid;     //group ID of file
        file_size = buf.st_size;    //file size in bytes (if file is a regular file)
        last_access = buf.st_atime;   //time of last access
        last_mod = buf.st_mtime;   //time of last data modification
        last_statchg = buf.st_ctime;   //time of last status change
      }      
    }
  }
  ocDirEntry( const ocDirEntry & in )
  :name(in.name),fileno(in.fileno),type(in.type)
  ,file_mode( in.file_mode),file_uid( in.file_uid),file_gid(in.file_gid )
  ,file_size(in.file_size),last_access(in.last_access),last_mod(in.last_mod),last_statchg(in.last_statchg)
  {;}
  
  ocDirEntry & operator = ( const ocDirEntry & in )
  {
    name = in.name;
    fileno = in.fileno;
    type = in.type;
    file_mode = in.file_mode;
    file_uid = in.file_uid; 
    file_gid = in.file_gid; 
    file_size = in.file_size;
    last_access = in.last_access;
    last_mod = in.last_mod;
    last_statchg = in.last_statchg;
    return * this;
  } 
  virtual ~ocDirEntry(){;}
};



typedef vector<ocDirEntry> ocDirectory;

class namesort_ocDirEntry
{
  public:
    bool operator ( ) ( const ocDirEntry & d1, const ocDirEntry & d2 )
    {
      bool ret = rand()%2;
      if(  d1.name.length() && d2.name.length() )
        ret = d1.name < d2.name;
      return ret;
    }
};


class ocFileSys
{
  DIR *dp;
  struct dirent *ep;
  mode_t mode;
  ocDirectory m_directory;
  int rc;
  string error;
  string cwd;
  
public:
  ocFileSys():dp(NULL),ep(NULL)
  {
    // default mode is 773
    mode = S_IRUSR | S_IWUSR | S_IXUSR |
           S_IRGRP | S_IWGRP | S_IXGRP |
           S_IROTH | S_IXOTH;
  }
  ~ocFileSys()
  {

  }
  /*
    getCWD()
    Get current working directory
    returns the absolute path to the current working directory
  
  */
  string & getCWD( void )
  {
    char buf[1024];
    cwd = getcwd(buf, sizeof(buf));
    return cwd;
  }
  /*
    mode is used to set file attribute 'rwx'
     for user group and public
  */
  void setMode( mode_t in )
  {
    mode = in;
  }
  // remove a file
  bool remove( string path )
  {
    rc = ::remove( path.c_str() );    
    return rc == 0;
  }      
  
  string check( void )
  {
    error = "";
    if(rc)
    {
      if( errno == EFAULT )
      {
        error = "EFAULT pathname points outside your accessible address space.";
      }  
      if( errno == EACCES )
      {
        error = "EACCES Write access to the directory containing pathname is not allowed "
        "     for the process's effective uid, or one of  the  directories  in"
        "     pathname did not allow search (execute) permission.";
      } 
      if( errno == EPERM )
      { 
        error = "EPERM The  directory  containing pathname has the sticky-bit (S_ISVTX) "
        "   set and the process's effective uid is neither the  uid  of  the "
        "   file to be deleted nor that of the directory containing it. ";
      }
      if( errno == ENAMETOOLONG ) 
      {
        error = "ENAMETOOLONG pathname was too long.";
      }
      if(  errno == ENOENT )
      {
        error = "ENOENT A  directory  component  in pathname does not exist or is a dangling symbolic link.";
      } 
      if(  errno == ENOTDIR)
      {
        error = "ENOTDIR A component used as a directory in pathname is not, in  fact,  a directory.";
      }
      if(  errno == ENOMEM )
      {
          error = "ENOMEM Insufficient kernel memory was available.";
      }
      if(  errno == EROFS )
      {
        error = "EROFS pathname refers to a file on a read-only filesystem.";
      } 
    }
    return error;
  }
  // rename a file
  bool rename( string path, string newpath )
  {
    rc = ::rename( path.c_str(), newpath.c_str() );
    return rc == 0;
  }
  bool copy( string path, string newpath )
  {
    bool isOK = false;    
    ifstream src(path.c_str(), ifstream::in | ifstream::binary);
    ofstream tgt(newpath.c_str(), ifstream::trunc | ifstream::binary);    
    if( src.good() && tgt.good() )
    {
      // get length of file:
      src.seekg (0, ios::end);
      int length = src.tellg();
      src.seekg (0, ios::beg);
      // allocate memory:
      char * buffer = new char [length];            
      if( length > 0 && buffer )
      {
        // read data as a block:
        src.read (buffer,length);
        tgt.write(buffer,length);
        delete [] buffer;  // free memory  
        src.close();  // close files
        tgt.close();
        isOK = true;
      }   
    }    
    return isOK;
  }
  bool is( string path )
  {
    rc = access(path.c_str(), F_OK);
    return rc == 0;
  }
  /*
    open the directory and get all of the directory entries.
  */
  bool openDir( string path )
  {
    bool bRet = false;
    dp = opendir( path.c_str() );
    if(dp)
    {
      while (ep = readdir (dp))
      {
        ocDirEntry temp(ep,path);
        if( temp.type == DT_REG || temp.type == DT_DIR )
        {
          m_directory.push_back(temp);
        }
      }
      closedir (dp);
      bRet = true;
    }
    return bRet;
  }
  ocDirectory & getDirectoryEntries( void )
  {
    return m_directory;
  }
  bool isDir( string path )
  {
    bool bRet = false;
    struct stat buf;
    int ret = stat(path.c_str(), &buf);
    if( ret==0 && S_ISDIR(buf.st_mode) )
    {
      bRet = true;
    }
    return bRet;
  }
  off_t fileSize( string path )
  {
    off_t size = 0;
    struct stat buf;
    int ret = stat(path.c_str(), &buf);
    if( ret==0 )
    {
      size = buf.st_size;
    }
    return size;
  }
  bool makeDir( string dir )
  {
    bool bRet = false;
    bRet = (mkdir( dir.c_str(), mode) == 0);
    return bRet;
  }

  bool makePath( string path )
  {
    bool bRet = false;
    // make sure there is a path
    if( path.length() )
    {
      string buildpath("");
      // see if it is pathed from root
      if( path[0] == '/' ) buildpath = "/";
      ocString pathParts(path);
      while( !pathParts.endOfParse() )
      {
        buildpath += pathParts.parse("/");
        buildpath += "/";
        if( isDir( buildpath ) == false )
        {
           makeDir( buildpath );
        }
      }
      // break the path into sub elements
    }
    return bRet;
  }
};

#endif

#ifdef IN_T2_TESTHARNESS
// example usage and testing
{
  string kDir = "KILLERDIR";
  string testDir = "/var/www/html/testing/rad/CodeTemplates/";
  {

    cout << "Is " << kDir << " a Dir?" << endl;
    ocFileSys ocFS;
    cout << "YOU ARE HERE:  " << ocFS.getCWD() << endl;
    cout << (ocFS.isDir( kDir )?"YES!":"NO!") << endl;
    cout << "Is " << kDir << " There?" << endl;
    cout << (ocFS.is( kDir )?"YES!":"NO!") << endl;
    cout << "if not, make it a path..." << endl;
    if( ocFS.is(kDir) == false )ocFS.makePath( kDir );
  }
  {
    ocFileSys fs;
    if( fs.openDir(testDir) )
    {
      ocDirectory & entries = fs.getDirectoryEntries();

      namesort_ocDirEntry nmsrt;
      sort( entries.begin(), entries.end(), nmsrt );
      cout << testDir <<  " FILE LISTING: " << endl;
      for(int i = 0; i < entries.size(); i++ )
      {
        cout << " fNo: " << entries[i].fileno
             << " fType: " << (int)(entries[i].type)
             << " Name: " << entries[i].name << endl;
      }
      cout << entries.size() << endl;
    }
  }
  return 0;
}
#endif
/*
   - Data Type: struct dirent
     This is a structure type used to return information about directory
     entries.  It contains the following fields:

    `char d_name[]'
          This is the null-terminated file name component.  This is the
          only field you can count on in all POSIX systems.

    `ino_t d_fileno'
          This is the file serial number.  For BSD compatibility, you
          can also refer to this member as `d_ino'.  In the GNU system
          and most POSIX systems, for most files this the same as the
          `st_ino' member that `stat' will return for the file.  *Note
          File Attributes::.

    `unsigned char d_namlen'
          This is the length of the file name, not including the
          terminating null character.  Its type is `unsigned char'
          because that is the integer type of the appropriate size

    `unsigned char d_type'
          This is the type of the file, possibly unknown.  The
          following constants are defined for its value:

         `DT_UNKNOWN'
               The type is unknown.  On some systems this is the only
               value returned.

         `DT_REG'
               A regular file.

         `DT_DIR'
               A directory.

         `DT_FIFO'
               A named pipe, or FIFO.  *Note FIFO Special Files::.

         `DT_SOCK'
               A local-domain socket.
         `DT_CHR'
               A character device.

         `DT_BLK'
               A block device.

          This member is a BSD extension.  The symbol
          `_DIRENT_HAVE_D_TYPE' is defined if this member is available.
          On systems where it is used, it corresponds to the file type
          bits in the `st_mode' member of `struct statbuf'.  If the
          value cannot be determine the member value is DT_UNKNOWN.
          These two macros convert between `d_type' values and
          `st_mode' values:

           - Function: int IFTODT (mode_t MODE)
               This returns the `d_type' value corresponding to MODE.

           - Function: mode_t DTTOIF (int DTYPE)
               This returns the `st_mode' value corresponding to DTYPE.


    mode_t bit field values:
    =========================
    S_ISUID           04000 set user ID on execution
    S_ISGID           02000 set group ID on execution
    S_ISVTX           01000 sticky bit

    S_IRUSR (S_IREAD) 00400 read by owner
    S_IWUSR (S_IWRITE)00200 write by owner
    S_IXUSR (S_IEXEC) 00100 execute/search by owner

    S_IRGRP           00040 read by group
    S_IWGRP           00020 write by group
    S_IXGRP           00010 execute/search by group

    S_IROTH           00004 read by others
    S_IWOTH           00002 write by others
    S_IXOTH           00001 execute/search by others

   D_TYPE directory entry type...
   enum
    {
        DT_UNKNOWN = 0,
    # define DT_UNKNOWN DT_UNKNOWN
        DT_FIFO = 1,
    # define DT_FIFO  DT_FIFO
        DT_CHR = 2,
    # define DT_CHR   DT_CHR
        DT_DIR = 4,
    # define DT_DIR   DT_DIR
        DT_BLK = 6,
    # define DT_BLK   DT_BLK
        DT_REG = 8,
    # define DT_REG   DT_REG
        DT_LNK = 10,
    # define DT_LNK   DT_LNK
        DT_SOCK = 12,
    # define DT_SOCK  DT_SOCK
        DT_WHT = 14
    # define DT_WHT   DT_WHT
    };
    
*/
