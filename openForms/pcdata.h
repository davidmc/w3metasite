/* pcdata.h */

#ifndef W3_VALIDPCDATA
#define W3_VALIDPCDATA


string & validPCDATA( string & in )
{
  ocString cndtn = in;
  in = cndtn
      .replaceAll("&","&amp;")
      .replaceAll("<=","&lte;")
      .replaceAll(">=","&gte;")
      .replaceAll("<","&lt;")
      .replaceAll(">","&gt;")
      .replaceAll("'","&apos;")
      .replaceAll("\"","&quot;")
      ;
  return in;
}

#endif
