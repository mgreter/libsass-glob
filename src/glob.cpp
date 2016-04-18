#include <cstring>
#include <iostream>
#include <stdint.h>
#include "sass.h"

// return version of libsass we are linked against
extern "C" const char* ADDCALL libsass_get_version() {
  return libsass_version();
}

// create a custom header to define to variables
Sass_Import_List glob_importer(const char* cur_path, Sass_Importer_Entry cb, struct Sass_Compiler* comp)
{
  std::cerr << "import globs [" << cur_path << "]\n";
  // get the cookie from importer descriptor
  // void* cookie = sass_importer_get_cookie(cb);
  // create a list to hold our import entries
  Sass_Import_List incs = sass_make_import_list(3);
  // create the resolved import entries (paths to be loaded)
  incs[0] = sass_make_import("foo.scss", "foo.scss", 0, 0);
  incs[1] = sass_make_import("bar.scss", "bar.scss", 0, 0);
  incs[2] = sass_make_import("baz.scss", "baz.scss", 0, 0);
  // return imports
  return incs;
}

// entry point for libsass to request custom headers from plugin
extern "C" Sass_Importer_List ADDCALL libsass_load_importers()
{
  // allocate a custom function caller
  Sass_Importer_Entry c_header =
    sass_make_importer(glob_importer, 3000, (void*) 0);
  // create list of all custom functions
  Sass_Importer_List imp_list = sass_make_importer_list(1);
  // put the only function in this plugin to the list
  sass_importer_set_list_entry(imp_list, 0, c_header);
  // return the list
  return imp_list;
}