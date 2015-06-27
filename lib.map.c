﻿#include "lib.a.h"


appBLOCK checkNode(appBLOCK A, BOOLEAN *ok){
    appTRIPLET *P = findNode(A);
    if(P == NULL){
        *ok = FALSE;
        return (appBLOCK)NULL;
    }
    *ok = TRUE;
    return P->B + (A - P->A);
}

// Not finished
/*appTRIPLET** checkArrNode(appBLOCK A, appNUMBER k){
    ULONG       i;
    appBLOCK    checkPoint;
    appTRIPLET  PointLeft, PointRight,
                *IterLeft, *IterRight,
                *EdgeLeft, *EdgeRight,
                *EdgeInnerLeft, *EdgeInnerRight,
                *tmp, *ptr,
                **Buffer;
    PVOID       restart;
    BOOLEAN     ok;
    
    //Searching points
    PointLeft.A     = A;
    PointLeft.k     = 1;
    PointRight.A    = A + k - 1;
    PointRight.k    = 1;
    
    //Check edge points
    if(!existNode(PointLeft.A) || !existNode(PointRight.A - 1))
    {
        return NULL;
    }
    
    //Iterate crossing intervals, check interruptions
    IterLeft    = findNode(PointLeft.A);
    IterRight   = nextNode(PointRight.A);
    EdgeLeft    = IterLeft;
    EdgeRight   = findNode(PointRight.A);
    
    restart = (PVOID)((char*)(IterLeft) - 0x20);
    ptr = IterLeft;
    checkPoint = ptr->A;
    for(i = 0; ptr != IterRight; i++)
    {
        if(ptr->A != checkPoint)
        {
            return NULL;
        }
        checkPoint += ptr->k;
        ptr = nextNode(ptr->A);
    }
    
    //Allocate memory, insert pointers
    Buffer          = (appTRIPLET**) MemoryAllocate(sizeof(appTRIPLET*) * (i + 3));
    EdgeInnerLeft   = (appTRIPLET* ) MemoryAllocate(sizeof(appTRIPLET ));
    EdgeInnerRight  = (appTRIPLET* ) MemoryAllocate(sizeof(appTRIPLET ));
    
    //Left Edge
    restart = (PVOID)((char*)(IterLeft) - 0x20);
    ptr = IterLeft;
    if(IterLeft->A == A && IterLeft->k == k) // case 1
    {
        MemoryFree(EdgeInnerLeft);
        printf("Buffer[%d] = NULL\n", i); Buffer[i] = NULL;
        printf("Buffer[%d] = ptr\n", i+1); Buffer[i+1] = ptr;
        printf("Buffer[%d] = NULL\n", i+2); Buffer[i+2] = NULL;
        printf("Buffer[%d] = NULL\n", i+3); Buffer[i+3] = NULL;
        return Buffer;
    }
    else
    if(A < IterLeft->A + IterLeft->k || A + k < IterLeft->A + IterLeft->k) // case 2 3 4
    {
        EdgeInnerLeft->A = A;
        EdgeInnerLeft->B = IterLeft->B + (A - IterLeft->A);
        EdgeInnerLeft->k = k;
        printf("Buffer[%d] = NULL\n", i); Buffer[i] = EdgeInnerLeft;
        printf("Buffer[%d] = NULL\n", i+1); Buffer[i+1] = NULL;
        printf("Buffer[%d] = NULL\n", i+2); Buffer[i+2] = NULL;
        printf("Buffer[%d] = NULL\n", i+3); Buffer[i+3] = NULL;
        return Buffer;
    }

    // case 5 6 7 8
    EdgeInnerLeft->A = A;
    EdgeInnerLeft->B = IterLeft->B + (A - IterLeft->A);
    EdgeInnerLeft->k = IterLeft->k - (A - IterLeft->A);
    printf("Buffer[%d] = EdgeInnerLeft\n", i); Buffer[i] = EdgeInnerLeft;
    i++;
    printf("Buffer[%d] = NULL\n", i); Buffer[i] = NULL;

    
    //Middle
    for(i = 0; ptr != IterRight; i++, ptr = nextNode(ptr->A))
    {
        if(nextNode(ptr->A) == IterRight)
        {
            printf("Buffer[%d] = NULL\n", i); Buffer[i] = NULL;
            i++;
            if(A == ptr->A)
            {
                MemoryFree(EdgeInnerRight);
            }
            else
            if(A + k == ptr->A + ptr->k)
            {
                MemoryFree(EdgeInnerRight);
                printf("Buffer[%d] = ptr\n", i); Buffer[i] = ptr;
                i++;
            }
            else
            {
                EdgeInnerRight->A = ptr->A;
                EdgeInnerRight->B = ptr->B;
                EdgeInnerRight->k = A + k - ptr->A;
                printf("Buffer[%d] = EdgeInnerRight\n", i); Buffer[i] = EdgeInnerRight;
                i++;
            }
            printf("Buffer[%d] = NULL\n", i); Buffer[i] = NULL;
            break;
        }
        else
        {
            printf("Buffer[%d] = ptr\n", i); Buffer[i] = ptr;
        }
    }
    
    //Right Edge
    
    
    
    printf("---Buffer[0]->A = %d\n", Buffer[0]->A);
    printf("return Buffer\n\n"); return Buffer;
}*/

BOOLEAN mapNode(appBLOCK A, appBLOCK B, appNUMBER k){
    BOOLEAN     ok;
    appTRIPLET  New,
                PointLeft, PointRight,
                *IterLeft, *IterRight,
                *EdgeLeft, *EdgeRight,
                EdgeOuterLeft, EdgeOuterRight,
                *tmp, *ptr;
    PVOID       restart;

    //Find the edge intervals
    PointLeft.A = A;
    PointLeft.k = 1;
    PointRight.A = A + k;
    PointRight.k = 1;

    if (existNode(PointLeft.A))
    {
        IterLeft = findNode(PointLeft.A);
        EdgeLeft = IterLeft;
    }
    else
    {
        IterLeft = nextNode(PointLeft.A);
        EdgeLeft = NULL;
    }

    if (existNode(PointRight.A))
    {
        EdgeRight = findNode(PointRight.A);
        IterRight = nextNode(PointRight.A);
    }
    else
    {
        IterRight = nextNode(PointRight.A);
        EdgeRight = NULL;
    }
    
    //Split edge intervals
    if (EdgeLeft != NULL)
    {
        EdgeOuterLeft.A = EdgeLeft->A;
        EdgeOuterLeft.B = EdgeLeft->B;
        EdgeOuterLeft.k = PointLeft.A - EdgeLeft->A;
        // EdgeLeft = &EdgeOuterLeft;
    }
    
    if (EdgeRight != NULL)
    {
        EdgeOuterRight.A = PointRight.A;
        EdgeOuterRight.B = EdgeRight->B + (PointRight.A - EdgeRight->A);
        EdgeOuterRight.k = (EdgeRight->A + EdgeRight->k) - PointRight.A;
        // EdgeRight = &PointRight;
    }

    // Delete crossing intervals
    restart = (PVOID)((char*)(IterLeft) - 0x20);
    for(ptr = IterLeft; ptr != IterRight; )
    {
        tmp = ptr;
        ptr = nextNode(ptr->A);
        RtlDeleteElementGenericTableAvl(Table, tmp);
    }

    //Add the main interval
    New.A = A;
    New.B = B;
    New.k = k;
    RtlInsertElementGenericTableAvl(Table, &New, sizeof(New), &ok);

    //Add the splitted intervals
    if (EdgeLeft != NULL)
        addNode(EdgeOuterLeft.A, EdgeOuterLeft.B, EdgeOuterLeft.k);
    if (EdgeRight != NULL)
        addNode(EdgeOuterRight.A, EdgeOuterRight.B, EdgeOuterRight.k);

    return ok;
}

BOOLEAN unmapNode(appBLOCK A, appNUMBER k){
    BOOLEAN ok;
    mapNode(A, (appBLOCK)NULL, k);
    ok = deleteNode(A);
    return ok;
}
