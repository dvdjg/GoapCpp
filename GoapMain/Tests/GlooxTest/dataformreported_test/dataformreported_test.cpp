/*
 *  Copyright (c) 2004-2017 by Jakob Schröter <js@camaya.net>
 *  This file is part of the gloox library. http://camaya.net/gloox
 *
 *  This software is distributed under a license. The full license
 *  agreement can be found in the file LICENSE in this distribution.
 *  This software may not be copied, modified, sold or distributed
 *  other than expressed in the named license agreement.
 *
 *  This software is distributed without any warranty.
 */
#include <gtest/gtest.h>

#include "dataformreported.h"
#include "dataformfield.h"
#include "dataform.h"
#include "tag.h"
using namespace gloox;

#include <stdio.h>
#include <string>
#include <cstdio> // [s]print[f]

//int main( int /*argc*/, char** /*argv*/ )
TEST(Gloox, DataFormReported)
{
  int fail = 0;
  std::string name;
  DataFormReported *f;
  Tag* t;

  // -------
  name = "empty form";
  f = new DataFormReported();
  t = f->tag();
  if( t->xml() != "<reported/>" )
  {
    ++fail;
    fprintf( stderr, "test '%s' failed\n", name.c_str() );
  }
  delete f;
  delete t;
  f = 0;
  t = 0;

  // -------
  name = "one child";
  f = new DataFormReported();
  f->addField( DataFormField::TypeTextSingle, "name", "value", "label" );
  t = f->tag();
  if( !t || t->xml() != "<reported><field type='text-single' var='name' label='label'>"
       "<value>value</value></field></reported>" )
  {
    ++fail;
    fprintf( stderr, "test '%s' failed: %s\n", name.c_str(), t->xml().c_str() );
  }
  delete f;
  delete t;
  f = 0;
  t = 0;

  // -------
  name = "many children";
  f = new DataFormReported();
  f->addField( DataFormField::TypeTextSingle, "name", "value", "label" );
  f->addField( DataFormField::TypeJidSingle, "name", "value", "label" );
  f->addField( DataFormField::TypeTextSingle, "name", "value", "label" );
  f->addField( DataFormField::TypeTextMulti, "name", "value", "label" );
  f->addField( DataFormField::TypeTextSingle, "name", "value", "label" );
  f->addField( DataFormField::TypeJidMulti, "name", "value", "label" );
  f->addField( DataFormField::TypeTextSingle, "name", "value", "label" );
  f->addField( DataFormField::TypeListSingle, "name", "value", "label" );
  f->addField( DataFormField::TypeTextSingle, "name", "value", "label" );
  f->addField( DataFormField::TypeListMulti, "name", "value", "label" );
  f->addField( DataFormField::TypeTextSingle, "name", "value", "label" );
  f->addField( DataFormField::TypeTextSingle, "name", "value", "label" );
  t = f->tag();
  if( !t || t->xml() != "<reported>"
       "<field type='text-single' var='name' label='label'><value>value</value></field>"
       "<field type='jid-single' var='name' label='label'><value>value</value></field>"
       "<field type='text-single' var='name' label='label'><value>value</value></field>"
       "<field type='text-multi' var='name' label='label'><value>value</value></field>"
       "<field type='text-single' var='name' label='label'><value>value</value></field>"
       "<field type='jid-multi' var='name' label='label'><value>value</value></field>"
       "<field type='text-single' var='name' label='label'><value>value</value></field>"
       "<field type='list-single' var='name' label='label'><value>value</value></field>"
       "<field type='text-single' var='name' label='label'><value>value</value></field>"
       "<field type='list-multi' var='name' label='label'><value>value</value></field>"
       "<field type='text-single' var='name' label='label'><value>value</value></field>"
       "<field type='text-single' var='name' label='label'><value>value</value></field>"
       "</reported>" )
  {
    ++fail;
    fprintf( stderr, "test '%s' failed: %s\n", name.c_str(), t->xml().c_str() );
  }
  delete f;
  delete t;
  f = 0;
  t = 0;








EXPECT_EQ(0, fail);

//    if( fail == 0 )
//  {
//    printf( "DataFormReported: OK\n" );
//    return 0;
//  }
//  else
//  {
//    fprintf( stderr, "DataFormReported: %d test(s) failed\n", fail );
//    return 1;
//  }

}
