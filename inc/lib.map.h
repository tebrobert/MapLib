#ifndef _LIB_MAP_H
#define _LIB_MAP_H

#include "lib.a.h"

LIB_BLOCK       CheckBlock(LIB_BLOCK A, BOOLEAN *ok);
LIB_PNODE_ARRAY CheckInterval(LIB_PTABLE Table, LIB_BLOCK A, LIB_BLOCK k);
BOOLEAN         MapInterval(LIB_BLOCK A, LIB_BLOCK B, LIB_BLOCK k);
BOOLEAN         UnmapInterval(LIB_BLOCK A, LIB_BLOCK k);

#endif //_LIB_MAP_H
