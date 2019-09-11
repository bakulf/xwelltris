#ifndef COMMONFUNCS_H
#define COMMONFUNCS_H

#include "globals.h"

enum FMode { ReadOnly, ReadWrite };

extern bool find_full_path_for_file(const char* fname, char* ret_full_pathname, 
				    FMode fm=ReadOnly);

#endif
