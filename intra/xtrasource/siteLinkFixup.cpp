//
// sync_client.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <istream>
#include <ostream>
#include <string>

#include "ocString.h"

#include "openMyDB.h"
#include "openDB.hpp"
#include "read_write_base.hpp"
#include "connectstring"
#include "paragraphs.hpp"
#include "link_obj.hpp"



int main(int argc, char* argv[])
{

    cout << "Connect String: " << CONNECTSTRING << endl;
    paragraphs_Obj para;
    changeMap changep;
    changep.insert(make_pair("content","content"));
    bool isOpen = para.get_data("");// get all the paragraphs
    while( isOpen )
    {
       ocString fix = para.content;
       para.content =  fix.replaceAll("www.artisanpoint.com","www.woodhollowcabinets.com");
       para.db_update(changep);
       cout << endl << para.content << endl;
       isOpen = para.next();
    }
    
    link_obj linko;
    changeMap changel;
    changel.insert(make_pair("url","url"));
    isOpen = linko.get_data("");// get all the paragraphs
    while( isOpen )
    {
      ocString fix = linko.url;
      linko.url = fix.replaceAll("www.artisanpoint.com","www.woodhollowcabinets.com");
      linko.db_update(changel);
      cout << endl << linko.url << endl;
      isOpen = linko.next();
    }

  return 0;
}

// compile implementations here
#include "read_write_base.cpp"

