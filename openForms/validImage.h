/*

  validImage.h

*/
bool isValidImage( string testImage )
{
  bool btrue=false;
  string::size_type enddot = testImage.find_last_of(".");
  if( enddot != string::npos )
  {
    enddot++;
    string sfx = testImage.substr(enddot);
    transform( sfx.begin(), sfx.end(), sfx.begin(), ::tolower );
    if( sfx == "png" || sfx == "gif"|| sfx == "jpg" ||
        sfx == "bmp" || sfx == "tif" || sfx == "jpeg" )
    {
      btrue=true;
    }
  }
  return btrue;
}
