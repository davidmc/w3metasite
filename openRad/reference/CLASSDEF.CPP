#include "makeclas.hpp"

const char * pszType      = "type";
const char * pszInit      = "init";
const char * pszAccessImp = "access.ctp";
const char * pszAccessDef = "access.htp";
const char * pszAssignImp = "assign.ctp";
const char * pszAssignDef = "assign.htp";

// tokens
//
const char * tokUName = "<UNAME>";  
const char * tokLName = "<LNAME>";  
const char * tokName = "<NAME>";  
const char * tokClass = "<CLASS>";  


// to parse input file
//---------------------------------------------------------------------------
const char * tokInclude        = "include";         // level 0 preprocessor
//---------------------------------------------------------------------------
const char * tokClassHeader    = "CLASS";           // level 1 set class name
//---------------------------------------------------------------------------
const char * tokPrivate        = "PRIVATE";         // level 2 private data
                                                    //  and methods
//---------------------------------------------------------------------------
const char * tokPublic         = "PUBLIC";          // level 2 protected data
                                                    //  public methods
//---------------------------------------------------------------------------    
const char * tokData           = "DATA";            // level 3 data
const char * tokMethods        = "METHODS";         // level 3 methods
const char * tokParents        = "PARENTS";         // level 3 parents 
//---------------------------------------------------------------------------


// more tokens -- used by cpp file for member replacement.
//
const char * tokSubParent  = "<<PARENTS>>";
const char * tokSubInit    = "<<INITIALIZERS>>";
const char * tokSubDestroy = "<<DESTROYERS>>";
const char * tokSubAssign  = "<<ASSIGNS>>";


const char * delimiter         = "-";

const char * destroyMark       = "~";

const char * indent            = "  ";


// CONSTRUCTOR

ClassDef::ClassDef( char * defFile ):openErr(0)
{

  // allocate for the input line from the class defination file.
  //
  char * classDefLine = new char[256];

  // open the needed files:
  //
  ifstream classDef(defFile);
  if( ! classDef )
  {
     cerr << "Could not open class definition file: " << defFile << endl;
     openErr = true;
  }

  cppFile = defFile;
  hppFile = defFile;
  cppFile += ".cpp";
  hppFile += ".hpp";

  if( !openErr )
  {
    char * result;
    const char * Level2;
    const char * Level3;

    // expect the first line in the class def to contain the CLASS
    // keyword.
    //
    if ( classDef.getline( classDefLine, 80 ) )
    {

      cout << classDefLine << endl;

      result = strtok( classDefLine," " );

      cout << result << endl;

      if( !strcmp( result, tokClassHeader ) )
      {
	result = strtok( '\0', " " );
	cout << result << endl;
	className = result;
        className.trim();
      }
    }

    while ( classDef.getline( classDefLine, 80 ) && className.max() )
    {
      trim( classDefLine );

      if( strlen( classDefLine ) )
      {
        cout << classDefLine << endl;

        // level 2
        if( strstr( classDefLine, tokPrivate ) )
        {
		 Level2 = tokPrivate;
        }
        else if( strstr( classDefLine, tokPublic ) )
        {
		 Level2 = tokPublic;
        }
        else if( strstr( classDefLine, tokData ) )
        {
		 Level3 = tokData;
        }
        else if( strstr( classDefLine, tokMethods ) )
        {
		 Level3 = tokMethods;
        }
        else if( strstr( classDefLine, tokParents ) )
        {
		 Level3 = tokParents;
        }
        else if( strstr( classDefLine, tokInclude ) )
        {
	  hString * incLine = new hString;
	  *incLine = classDefLine;
	  includes.add(incLine);
        }
        else
        {
	  if( Level3 == tokData )
	  {
	    classMember * member = new classMember;

	    if( parseMember( *member, classDefLine ) )
	    {

	      findType( *member );
	      accessDefs( *member );
              accessImps( *member );
              initAndDestroy( *member );

	      if( Level2 == tokPublic )
	      {
	        publicMembers.add(member);
	      }
	      else if( Level2 == tokPrivate )
	      {
	        privateMembers.add(member);
	      }
              if( member->type.matchIn("*") )
              {
	        classMember * sizMember = new classMember;
                sizMember->name = "size";
                sizMember->name += member->name.upperS();
                sizMember->type = "unsigned long";
	        findType( *sizMember );
	        accessDefs( *sizMember );
                accessImps( *sizMember );
                initAndDestroy( *sizMember );
	        sizeMembers.add( sizMember );

              }

	    }
	  }
	  else if ( Level3 == tokMethods )
	  {

	    hString * method = new hString;
	    *method = classDefLine;
	    if( Level2 == tokPublic )
	    {
	      publicMethods.add(method);
	    }
	    else if( Level2 == tokPrivate )
	    {
	      privateMethods.add(method);
	    }

	  }
	  else if ( Level3 == tokParents )
	  {

	    hString * dad = new hString;
	    *dad = classDefLine;
	    if( Level2 == tokPublic )
	    {
	      publicParents.add(dad);
	    }
	    else if( Level2 == tokPrivate )
	    {
	      privateParents.add(dad);
	    }

	  }

        }

      } // end if length

    } // end while

  } // end if

  delete [] classDefLine;

}


int ClassDef::parseMember( classMember & cls, char * classDefLine )
{

  char * pszToken;
  char * pszType = new char[80];
  char * pszName = new char[80];
  memset(pszType,0,80);
  memset(pszName,0,80);

  pszToken = strtok( classDefLine, " " );

  while( pszToken )
  {
    char * holder = pszToken;
    pszToken = strtok('\0'," ");
    if( pszToken )
    {
      strcat( pszType, holder );
      strcat( pszType, " " );
    }
    else
    {
      strcat( pszName, holder );
    }
  }

  int typeLen = strlen( pszType );
  int nameLen = strlen( pszName );

  if( typeLen && nameLen )
  {
    cls.type = pszType;
    cls.name = pszName;

    cls.type.trim();
    cls.name.trim();

    cout << "type: <" << cls.type.psz() << "> name: <";
    cout << cls.name.psz() << ">" << endl;
  }

  delete [] pszType;
  delete [] pszName;

  return( typeLen && nameLen );

}


void ClassDef::findType( classMember & cls )
{

  char buffer [256];
  int typeCount = 1;
  Boolean found = false;

  ifstream types( pszType );
  if( ! types )
  {
    cerr << "Could not open type definition file. " << endl;
    return;
  }
  while( types.getline( buffer, 256 ) )
  {

    if( !strncmp( buffer, delimiter, 1 ) )
    {
      typeCount++;
    }
    else if( cls.type.match( buffer ) )
    {
      found = true;
      break;
    }

  }// end while

  if( found == true )
  {
    cls.iType = typeCount;
    cout << "found: " << typeCount << endl;
  }

  types.close();

}


void ClassDef::accessDefs( classMember & cls )
{

  char buffer [256];
  int typeCount = 1;

  ifstream accessDef( pszAccessDef );

  if( ! accessDef )
  {
    cerr << "Could not open access method definition file. " << endl;
    return;
  }

  while( accessDef.getline( buffer, 256 ) )
  {
    if( !strncmp( buffer, delimiter, 1 ) )
    {
      typeCount++;
    }
    else if( cls.iType == typeCount )
    {
      hString * newStr = new hString;
      *newStr = buffer;

      newStr->searchAndReplaceWith( tokName, cls.name.upperS().psz() );

      cls.AccessDefs.add( newStr );
    }

  }

  accessDef.close();

}


void ClassDef::accessImps( classMember & cls )
{

  char buffer [256];
  int typeCount = 1;

  ifstream accessImp( pszAccessImp );

  if( ! accessImp )
  {
    cerr << "Could not open access method implementation file. " << endl;
    return;
  }

  while( accessImp.getline( buffer, 256 ) )
  {
    if( !strncmp( buffer, delimiter, 1 ) )
    {
      typeCount++;
    }
    else if( cls.iType == typeCount )
    {

      hString * newStr = new hString;
      *newStr = buffer;

      newStr->searchAndReplaceWith( tokLName, cls.name.lowerS().psz() );

      newStr->searchAndReplaceWith( tokUName, cls.name.upperS().psz() );

      newStr->searchAndReplaceWith( tokClass, className.psz() );

      cls.AccessImps.add( newStr );

    }

  } // end while

  accessImp.close();

}

void ClassDef::initAndDestroy( classMember & cls )
{

  char buffer [256];

  int typeCount = 1;

  Boolean destruct = false;

  ifstream iniDest( pszInit );

  if( ! iniDest )
  {
    cerr << "Could not open access method implementation file. " << endl;
    return;
  }

  while( iniDest.getline( buffer, 256 ) )
  {
    if( !strncmp( buffer, delimiter, 1 ) )
    {
      destruct = false;
      typeCount++;
    }
    else if( !strncmp( buffer, destroyMark, 1 ) )
    {
      destruct = true;
    }
    else if( cls.iType == typeCount )
    {

      hString * newStr = new hString;
      *newStr = buffer;

      newStr->searchAndReplaceWith( tokName, cls.name.lowerS().psz() );
      newStr->trim();

      if( destruct )
      {
        cls.Destroy.add( newStr );
      }
      else
      {
        cls.Init.add( newStr );
      }
    }

  } // end while

  iniDest.close();

}

void ClassDef::WriteCpp( void )
{

  int i, m;

  cout << cppFile.psz() << endl;

  ofstream cpp( cppFile );

  if( !cpp  )
  {
     cerr << "Could not open cpp file: " << cppFile.psz() << endl;
     openErr = 1;
  }

  // cpp must include the hpp file.
  //
  cpp << "#include \"" << hppFile.psz() << "\"" << endl << endl;

  // Constructor, Destructor, Assignment, copy constructor.
  //
  // Allocate for the input line from the class defination file.
  //
  char * GenLine = new char[256];

  // open the needed files:
  //
  ifstream classGen( pszAssignImp );

  if( ! classGen )
  {
    cerr << "Couldn't open implement file: " << pszAssignImp << endl;
    openErr = true;
  }

  while ( classGen.getline( GenLine, 255 ) && className.max() )
  {

    hString * newStr = new hString;
    *newStr = GenLine;
    static char sep = ':'; 

    // Inherited by..
    //
    if( newStr->matchIn( tokSubParent ) )
    {

      for( i=0; i < privateParents.max(); i++ )
      {
        cpp << sep << privateParents[i]->psz() << "()" <<endl;
        if( sep == ':' ) sep = ',';
      }

      for( i=0; i < publicParents.max(); i++ )
      {
        cpp << sep << publicParents[i]->psz() << "()" << endl;  
        if( sep == ':' ) sep = ',';
      }
         // Member Initializers
    }    //
    else if( newStr->matchIn( tokSubInit ) )
    {

      for( i=0; i < publicMembers.max(); i++ )
      {
        for( m=0; m < publicMembers[i]->Init.max(); m++ )
        {
          cpp << sep << publicMembers[i]->Init[m]->psz() << endl;
          if( sep == ':' ) sep = ',';
        }
      }

      for( i=0; i < privateMembers.max(); i++ )
      {
        for( m=0; m < privateMembers[i]->Init.max(); m++ )
        {
          cpp << sep << privateMembers[i]->Init[m]->psz() << endl;
          if( sep == ':' ) sep = ',';
        }
      }
      for( i=0; i < sizeMembers.max(); i++ )
      {
        for( m=0; m < sizeMembers[i]->Init.max(); m++ )
        {
          cpp << sep << sizeMembers[i]->Init[m]->psz() << endl;
          if( sep == ':' ) sep = ',';
        }
      }

    }  // Destructor parts
    else if( newStr->matchIn(tokSubDestroy ) )
    {

      for( i=0; i < publicMembers.max(); i++ )
      {
        for( m=0; m < publicMembers[i]->Destroy.max(); m++ )
        {
          cpp << publicMembers[i]->Destroy[m]->psz() << endl;
        }
      }

      for( i=0; i < privateMembers.max(); i++ )
      {
        for( m=0; m < privateMembers[i]->Destroy.max(); m++ )
        {
          cpp << privateMembers[i]->Destroy[m]->psz() << endl;
        }
      }

    }  // assignement
    else if( newStr->matchIn( tokSubAssign ) )
    {

      // Handle level of indenting.
      // Indents are different for assignment and copy construction.
      //
      int numIndents = 1;
      if( newStr->matchIn( "+2" ) )
      {
        numIndents = 2;
      }

      hString assignStr;

      for( i=0; i < publicMembers.max(); i++ )
      {
        for( m=0; m<numIndents; m++ ) cpp << indent;
        cpp << publicMembers[i]->AccessDefs[2]->psz() << endl;
      }

      for( i=0; i < privateMembers.max(); i++ )
      {
        for( m=0; m<numIndents; m++ ) cpp << indent;
        cpp << privateMembers[i]->AccessDefs[2]->psz() << endl;
      }

    }
    else 
    {
      newStr->searchAndReplaceWith( tokClass, className.psz() );
      newStr->trim();
      cpp << newStr->psz() << endl;
    }

    delete newStr;

  } // end while

  delete GenLine;


  // Assignment
  //
  for( i=0; i<privateMembers.max(); i++ )
  {

    for ( int j = 0; j<privateMembers[i]->AccessImps.max(); j++ )
    {
      cpp << privateMembers[i]->AccessImps[j]->psz() << endl;
    }
    cpp << endl;

  }

  for( i=0; i<publicMembers.max(); i++ )
  {

    for ( int j = 0; j<publicMembers[i]->AccessImps.max(); j++ )
    {
      cpp << publicMembers[i]->AccessImps[j]->psz() << endl;
    }
    cpp << endl;

  }

  for( i=0; i<sizeMembers.max(); i++ )
  {

    for ( int j = 0; j<sizeMembers[i]->AccessImps.max(); j++ )
    {
      cpp << sizeMembers[i]->AccessImps[j]->psz() << endl;
    }
    cpp << endl;

  }

  // All Other Methods
  //
  hString nmdblcol;
  nmdblcol = className;
  nmdblcol += "::";

  for( i=0; i < privateMethods.max(); i++)
  {
    hString current;
    current = privateMethods[i]->psz(); 
    current.searchAndReplaceWith( "`", nmdblcol );
    cpp << current.psz() << endl;
    // default implementation body
    cpp << "{" << endl << indent << ";" << endl << "}" << endl << endl;
  }

  for( i=0; i < publicMethods.max(); i++)
  {
    hString current;
    current = publicMethods[i]->psz(); 
    current.searchAndReplaceWith( "`", nmdblcol );
    cpp << current.psz() << endl;
    // default implementation body
    cpp << "{" << endl << indent << ";" << endl << "}" << endl << endl;
  }




}


void ClassDef::WriteHpp( void )
{

  int i;
  int width = 10;

  cout << hppFile.psz() << endl;

  ofstream hpp( hppFile );
  if( !hpp  )
  {
     cerr << "Could not open hpp file: " << hppFile.psz() << endl;
     openErr = 1;
  }

  // write any #includes
  //
  for( i=0; i < includes.max(); i++ )
  {
    hpp << includes[i]->psz() << endl; 
  }

  // write the class name
  //
  hpp << endl << "class " << className.psz(); 

  char sep = ':'; 

  for( i=0; i < privateParents.max(); i++ )
  {
    hpp << sep << " private " << privateParents[i]->psz();
    if( sep == ':' ) sep = ',';
  }

  for( i=0; i < publicParents.max(); i++ )
  {
    hpp << sep << " public " << publicParents[i]->psz();  
    if( sep == ':' ) sep = ',';
  }

  hpp << endl << "{" << endl;

  // now go through the found variables
  //
  hpp << endl << "private:" << endl << endl;

  for( i=0; i<privateMembers.max(); i++ )
  {
    width = privateMembers[i]->type.max() > width?
	    privateMembers[i]->type.max():
	    width;
    hpp.setf(ios::left,ios::adjustfield);
    hpp << indent << setw(width) << privateMembers[i]->type.psz() << " ";
    hpp << indent << privateMembers[i]->name.lowerS().psz() << ";" << endl;
  }


  // Auto members for the size of pointer variables
  //
  for( i=0; i<sizeMembers.max(); i++ )
  {
    width = sizeMembers[i]->type.max() > width?
	    sizeMembers[i]->type.max():
	    width;
    hpp.setf(ios::left,ios::adjustfield);
    hpp << indent << setw(width) << sizeMembers[i]->type.psz() << " ";
    hpp << indent << sizeMembers[i]->name.lowerS().psz() << ";" << endl;
  }


  // private method output goes here.
  //
  hpp << endl << "protected:" << endl << endl;

  for( i=0; i < privateMethods.max(); i++)
  {
    hString current;
    current = privateMethods[i]->psz();
    current.searchAndReplaceWith( "`", "" );
    hpp << indent << current.psz() << ";" << endl;
  }

  width = 10;

  for( i=0; i<publicMembers.max(); i++ )
  {
    width = publicMembers[i]->type.max() > width?
	    publicMembers[i]->type.max():
	    width;
    hpp.setf(ios::left,ios::adjustfield);
    hpp << indent << setw(width) << publicMembers[i]->type.psz() << " ";
    hpp << indent << publicMembers[i]->name.lowerS().psz() << ";" << endl;
  }

  // size sets go in the protected region
  //
  for( i=0; i<sizeMembers.max(); i++ )
  {

    // only do the set here.
    hpp << indent << sizeMembers[i]->AccessDefs[1]->psz() << endl;
    hpp << endl;

  }

  for( i=0; i<privateMembers.max(); i++ )
  {

    for ( int j = 0; j<2 && j<privateMembers[i]->AccessDefs.max(); j++ )
    {
      hpp << indent << privateMembers[i]->AccessDefs[j]->psz() << endl;
    }
    hpp << endl;

  }

  hpp << endl << "public:" << endl << endl;

  //
  // The universal public stuff goes here
  //

  //
  // Constructor, destructor, assignment and copy constructor
  //

  // allocate for the input line from the class defination file.
  //
  char * GenLine = new char[256];

  // open the needed files:
  //
  ifstream classGen( pszAssignDef );
  if( ! classGen )
  {
     cerr << "Couldn't open assign def. file: " << pszAssignDef << endl;
     openErr = true;
  }

  while ( classGen.getline( GenLine, 255 ) && className.max() )
  {

    hString * newStr = new hString;
    *newStr = GenLine;

    newStr->searchAndReplaceWith( tokClass, className.psz() );
    newStr->trim();
    
    hpp << indent << newStr->psz() << endl;

    delete newStr;

  } // end while

  delete GenLine;

  // accessors go here...
  //


  for( i=0; i<publicMembers.max(); i++ )
  {

    for ( int j = 0; j<2 && j<publicMembers[i]->AccessDefs.max(); j++ )
    {
      hpp << indent << publicMembers[i]->AccessDefs[j]->psz() << endl;
    }
    hpp << endl;

  }

  for( i=0; i<sizeMembers.max(); i++ )
  {

    // only do the get here.
    hpp << indent << sizeMembers[i]->AccessDefs[0]->psz() << endl;
    hpp << endl;

  }


  for( i=0; i < publicMethods.max(); i++)
  {
    hString current;
    current = publicMethods[i]->psz();
    current.searchAndReplaceWith( "`", "" );
    hpp << indent << current.psz() << ";" << endl;
  }

  hpp << endl << "};" << endl;

}
