#include "lib.a.h"


LIB_BLOCK checkNode(LIB_BLOCK A, BOOLEAN *ok){
    LIB_NODE *P = findNode(A);
    if(P == NULL){
        *ok = FALSE;
        return (LIB_BLOCK)NULL;
    }
    *ok = TRUE;
    return P->B + (A - P->A);
}

LIB_PNODE_ARRAY checkArrNode(LIB_BLOCK A, LIB_NUMBER k)
{
    int i;
    LIB_PNODE ptr;
    LIB_PNODE_ARRAY Buffer;
    
    ptr = findNode(A);
    if (ptr == NULL)
        ptr = nextNode(A);
    
    for (i = 0; ptr != NULL && (A + k > ptr->A); i++)
    {
        ptr = nextNode(ptr->A);
    }
    
    Buffer = CreateNodeArray(i);
    
    ptr = findNode(A);
    if (ptr == NULL)
        ptr = nextNode(A);
    
    while (ptr != NULL && (A + k > ptr->A))
    {
        AppendNode(&Buffer, ptr);
        ptr = nextNode(ptr->A);
    }
    
    return Buffer;
}

BOOLEAN mapNode(LIB_BLOCK A, LIB_BLOCK B, LIB_NUMBER k){
    BOOLEAN     ok;
    LIB_NODE  New,
                PointLeft, PointRight,
                *IterLeft, *IterRight,
                *EdgeLeft, *EdgeRight,
                EdgeOuterLeft, EdgeOuterRight,
                *tmp, *ptr;

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
        addNode(EdgeOuterLeft);
    if (EdgeRight != NULL)
        addNode(EdgeOuterRight);

    return ok;
}

BOOLEAN unmapNode(LIB_BLOCK A, LIB_NUMBER k){
    BOOLEAN ok;
    mapNode(A, (LIB_BLOCK)NULL, k);
    ok = deleteNode(A);
    return ok;
}
