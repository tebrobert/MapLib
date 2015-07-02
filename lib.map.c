#include "lib.a.h"


LIB_BLOCK checkNode(LIB_BLOCK A, BOOLEAN *ok){
    LIB_NODE *P = FindNode(CurrentTable, A);
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
    
    ptr = FindNode(CurrentTable, A);
    if (ptr == NULL)
        ptr = NextNode(CurrentTable, A);
    
    for (i = 0; ptr != NULL && (A + k > ptr->A); i++)
    {
        ptr = NextNode(CurrentTable, ptr->A);
    }
    
    Buffer = CreatePNodeArray(i);
    
    ptr = FindNode(CurrentTable, A);
    if (ptr == NULL)
        ptr = NextNode(CurrentTable, A);
    
    while (ptr != NULL && (A + k > ptr->A))
    {
        AppendPNode(&Buffer, ptr);
        ptr = NextNode(CurrentTable, ptr->A);
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
        IterLeft = FindNode(CurrentTable, PointLeft.A);
        EdgeLeft = IterLeft;
    }
    else
    {
        IterLeft = NextNode(CurrentTable, PointLeft.A);
        EdgeLeft = NULL;
    }

    if (existNode(PointRight.A))
    {
        EdgeRight = FindNode(CurrentTable, PointRight.A);
        IterRight = NextNode(CurrentTable, PointRight.A);
    }
    else
    {
        IterRight = NextNode(CurrentTable, PointRight.A);
        EdgeRight = NULL;
    }
    
    //Split edge intervals
    if (EdgeLeft != NULL)
    {
        EdgeOuterLeft.A = EdgeLeft->A;
        EdgeOuterLeft.B = EdgeLeft->B;
        EdgeOuterLeft.k = PointLeft.A - EdgeLeft->A;
    }
    
    if (EdgeRight != NULL)
    {
        EdgeOuterRight.A = PointRight.A;
        EdgeOuterRight.B = EdgeRight->B + (PointRight.A - EdgeRight->A);
        EdgeOuterRight.k = (EdgeRight->A + EdgeRight->k) - PointRight.A;
    }

    // Delete crossing intervals
    for(ptr = IterLeft; ptr != IterRight; )
    {
        tmp = ptr;
        ptr = NextNode(CurrentTable, ptr->A);
        RtlDeleteElementGenericTableAvl(CurrentTable, tmp);
    }

    //Add the main interval
    New.A = A;
    New.B = B;
    New.k = k;
    RtlInsertElementGenericTableAvl(CurrentTable, &New, sizeof(New), &ok);

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
