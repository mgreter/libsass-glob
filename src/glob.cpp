#include <cstring>
#include <cstdlib>
#include <iostream>
#include <stdint.h>
#include <string>
#include "FS.hpp"
#include "sass.h"
#include <libgen.h>

// return version of libsass we are linked against
extern "C" const char* ADDCALL libsass_get_version() {
  return libsass_version();
}

// create a custom importer to resolve glob-based includes
struct SassImportList* glob_importer(const char* cur_path, struct SassImporter* cb, struct SassCompiler* comp)
{

  // get the base directory from previous import
  const struct SassImport* imp = sass_compiler_get_last_import(comp);
  char* prev = strdup(sass_import_get_abs_path(imp));
  std::string pattern(dirname(prev)); std::free(prev);
  if (strcmp(pattern.c_str(), "stream:") == 0) pattern = "";
  else pattern += std::string("/");
  pattern += cur_path;

  // instantiate the matcher instance
  FS::Match matcher(pattern);
  // get vector of matches (results are cached)
  const std::vector<FS::Entry*> matches = matcher.getMatches();

  // propagate error back to libsass
  if (matches.empty()) return NULL;

  // get the cookie from importer descriptor
  // void* cookie = sass_importer_get_cookie(cb);
  // create a list to hold our import entries
  struct SassImportList* incs = sass_make_import_list();

  // iterate over the list and print out the results
  for (size_t i = 0; i < matches.size(); ++i) {
    // create intermediate string object
    std::string path = matches[i]->path();
    // create the resolved import paths to load
    sass_import_list_push(incs, sass_make_import(
      path.c_str(), path.c_str(),
      0, 0, SASS_IMPORT_AUTO));
  }

  // return imports
  return incs;
}

// entry point for libsass to request custom functions from plugin
extern "C" void ADDCALL libsass_init_plugin(struct SassCompiler* compiler)
{

  // Add constants via custom headers
  sass_compiler_add_custom_importer(compiler,
    sass_make_importer(glob_importer, 3000, (void*)0));

}
