#ifndef OPEN_DB_HPP
#define OPEN_DB_HPP

// Always include the base interfaces
#include "openDB.h"

/*
  Include all implementation files to be used by the client program
  these definitions are set by an environment variable IMPL_DEFS.
  Ex: IMPL_DEFS=-DDEFINE_OPENMYDB
  If any or all of these are defined they are included in the build.
*/
#ifdef DEFINE_OPENDBPG
#define JUST_PG_FIELD_INTERFACES
#include "openDBPG.h"
#endif
#ifdef DEFINE_OPENDBODBC
#include "openODBC.h"
#endif
#ifdef DEFINE_OPENMYDB
#include "openMyDB.h"
#endif

#endif
