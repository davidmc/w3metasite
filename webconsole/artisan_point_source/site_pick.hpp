#include "siteLimit.h"

class site_pick {
protected:
  string selSite;
  string siteOptions;
public:
  site_pick(cgiScript & script)
  {
    selSite = currentSite( script.ClientArguments() );
  }
  ~site_pick()
  {
  }  
  string & getSiteOptions( cgiScript & script, openLogin & login )
  {
    string limits;
    string value,label;
    string sql = "select id, name from sites ";
    limits = siteLimits(login);
    if(limits.length())
    {
      sql += "  where id ";
      sql += limits;
    }
    quickQuery qq;
    openRS & rs = qq.getRS();
    siteOptions += "<!--";
    siteOptions += sql;
    siteOptions += "-->";
    int count=0;
    for(bool o=rs.open(sql);o;o=rs.next())
    {
      if( count == 0 )
      {
        value = rs.getField(0).format();
        label = rs.getField(1).format();
      }
      siteOptions += "<option value=\"";
      siteOptions += rs.getField(0).format();
      siteOptions += "\"";
      if( selSite == rs.getField(0).format() )
      {
        siteOptions += " selected";
      }
      siteOptions += ">";
      siteOptions += rs.getField(1).format();
      siteOptions += "</option>\n";
      count++;
    }
    rs.close();
    if( selSite.length() == 0 )
    {
      // set the site cookie to the first site
      cgiCookie cookie;
      cookie.setPath("/");
      cookie.set( "site_id", value.c_str() );
    }
    return siteOptions;
  }
};


