#ifndef _LIB_MAP_H
#define _LIB_MAP_H

#include <ntddk.h>

#include "lib.avl.h"

appBLOCK        checkNode(appBLOCK A, BOOLEAN *ok);
//appTRIPLET**    checkArrNode(appBLOCK A, appNUMBER k);
BOOLEAN         mapNode(appBLOCK A, appBLOCK B, appNUMBER k);
BOOLEAN         unmapNode(appBLOCK A, appNUMBER k);

#endif //_LIB_MAP_H
