/*
# makesite.h
# sets up a directory structure for a metasite

# get the one input argument (the name of the site)
echo 'making ' $1

mkdir $1
mkdir $1/Templates
mkdir $1/pg_images
mkdir $1/images
mkdir $1/small_images
mkdir $1/resources
mkdir $1/styles
mkdir $1/intra
mkdir $1/intra/Templates
mkdir $1/intra/reportTemplates
mkdir $1/intra/pg_images
mkdir $1/intra/images
mkdir $1/intra/small_images
mkdir $1/intra/resources
mkdir $1/intra/styles

echo 'changing directory permissions'
chmod 775 $1 -R

echo 'making sys link and program links:'
cd $1
ln -s  /var/www/html/testing/pages.meta pages.meta
ln -s  /var/www/html/testing/show.meta show.meta
ln -s  /var/www/html/testing/contactus.meta contactus.meta
ln -s /var/www/html/testing/sys sys
cd ..

echo 'Done making ' $1

*/
#include <fstream>
#include <iomanip>
#include "ocString.h"
#include "ocFileSys.h"
class siteMaker
{
  public:
    string httpDomain;
    string httpBaseDir;
    string httpVsiteConfigPath;
    string httpCloneFrom;
    string httpAlias;
    ocString vsiteConfigTemplate;

    siteMaker( string domain, string basedir,
               string vCfgPath, string cloneFrom ):httpDomain(domain),
               httpBaseDir(basedir),httpVsiteConfigPath(vCfgPath),
               httpCloneFrom(cloneFrom)
    {

      httpAlias = ocString(httpDomain).remove("www.");
    }

    bool makeSite( string vhostTemplatePath = "" )
    {
      if( vhostTemplate.length() )
      {
        if( loadVHost ( vhostTemplatePath ) )
        {
          string vHostConf = vsiteConfigTemplate
            .replaceAll("{{Directory}}",httpBaseDir)
            .replaceAll("{{ServerName}}",httpDomain)
            .replaceAll("{{ServerAlias}}",httpAlias);
          string newPath = httpVsiteConfigPath;
          newPath +=  httpDomain;
          newPath += ".conf";
          ofstream tgt(newPath.c_str(), ifstream::trunc | ifstream::binary);
          tgt.write(vHostConf.c_str(),vHostConf.length());
        }
      }
      ocFileSys fsys;
      string createdir = httpBaseDir;
      fsys.makePath( createdir );
      createdir               += "/Templates";
      fsys.makeDir(createdir);
      createdir  =  httpBaseDir + "/pg_images";
      fsys.makeDir(createdir);
      createdir  =  httpBaseDir + "/images";
      fsys.makeDir(createdir);
      createdir  =  httpBaseDir + "/small_images";
      fsys.makeDir(createdir);
      createdir  =  httpBaseDir + "/resources";
      fsys.makeDir(createdir);
      createdir  =  httpBaseDir + "/styles";
      fsys.makeDir(createdir);
      createdir  =  httpBaseDir + "/scripts";
      fsys.makeDir(createdir);
      createdir  =  httpBaseDir + "/media";
      fsys.makeDir(createdir);
      createdir  =  httpBaseDir + "/reportTemplates";
      fsys.makeDir(createdir);
    }
    bool loadVHost ( string filename )
    {
      bool bRet = false;
      vsiteConfigTemplate = "";
      ifstream templateFile( filename.c_str() );
      if( templateFile )
      {
        char buffer[1024];
        templateFile.getline( buffer, sizeof(buffer) );
        while( templateFile.rdstate() == ios::goodbit )
        {
          if( strlen(buffer) )
          {
            vsiteConfigTemplate += buffer;
            vsiteConfigTemplate += "\n";
          }
          templateFile.getline( buffer, sizeof(buffer) );
        }
      }
      bRet = vsiteConfigTemplate.length() > 0;
      return bRet;
    }
};