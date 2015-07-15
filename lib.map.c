#include "lib.a.h"

LIB_BLOCK checkNode(LIB_BLOCK A, BOOLEAN *ok){
    LIB_NODE *P = FindNode(CurrentTable, A);
    if(P == NULL)
    {
        *ok = FALSE;
        return (LIB_BLOCK)NULL;
    }
    *ok = TRUE;
    return P->B + (A - P->A);
}

LIB_PNODE_ARRAY CheckInterval(LIB_PTABLE Table, LIB_BLOCK A, LIB_NUMBER k)
{
    int i;
    LIB_PNODE ptr;
    LIB_PNODE_ARRAY Buffer;
    
    ptr = FindNode(Table, A);
    if (ptr == NULL)
        ptr = NextNode(Table, A);
    
    for (i = 0; ptr != NULL && (A + k > ptr->A); i++)
    {
        ptr = NextNode(Table, ptr->A);
    }
    
    Buffer = CreatePNodeArray(i);
    
    ptr = FindNode(Table, A);
    if (ptr == NULL)
        ptr = NextNode(Table, A);
    
    while (ptr != NULL && (A + k > ptr->A))
    {
        AppendPNode(&Buffer, ptr);
        ptr = NextNode(Table, ptr->A);
    }
    
    return Buffer;
}

BOOLEAN mapNode(LIB_BLOCK A, LIB_BLOCK B, LIB_NUMBER k)
{
    BOOLEAN     ok;
    LIB_NODE    New, EdgeOuterLeft, EdgeOuterRight;
    LIB_PNODE   IterLeft, IterRight, EdgeLeft, EdgeRight, tmp, ptr;
    LIB_BLOCK   PointLeft, PointRight;

    //Find the edge intervals
    PointLeft = A;
    PointRight = A + k;
    
    if (existNode(PointLeft))
    {
        EdgeLeft = FindNode(CurrentTable, PointLeft);
        IterLeft = EdgeLeft;
    }
    else
    {
        EdgeLeft = NULL;
        IterLeft = NextNode(CurrentTable, PointLeft);
    }

    if (existNode(PointRight))
    {
        EdgeRight = FindNode(CurrentTable, PointRight);
        IterRight = NextNode(CurrentTable, PointRight);
    }
    else
    {
        EdgeRight = NULL;
        IterRight = NextNode(CurrentTable, PointRight);
    }
    
    //Split edge intervals
    if (EdgeLeft != NULL)
    {
        EdgeOuterLeft.A = EdgeLeft->A;
        EdgeOuterLeft.B = EdgeLeft->B;
        EdgeOuterLeft.k = PointLeft - EdgeLeft->A;
        EdgeOuterLeft.UsedBy = NULL;
        if (EdgeLeft->UsedBy != NULL)
        {
            EdgeOuterLeft.UsedBy = MemoryAllocate(sizeof(LIB_BITMASK));
            *EdgeOuterLeft.UsedBy = CopyBitmask(EdgeLeft->UsedBy);
        }
    }
    
    if (EdgeRight != NULL)
    {
        EdgeOuterRight.A = PointRight;
        EdgeOuterRight.B = EdgeRight->B + (PointRight - EdgeRight->A);
        EdgeOuterRight.k = (EdgeRight->A + EdgeRight->k) - PointRight;
        EdgeOuterRight.UsedBy = NULL;
        if (EdgeRight->UsedBy != NULL)
        {
            EdgeOuterRight.UsedBy = MemoryAllocate(sizeof(LIB_BITMASK));
            *EdgeOuterRight.UsedBy = CopyBitmask(EdgeRight->UsedBy);
        }
    }

    // Delete crossing intervals
    for(ptr = IterLeft; ptr != IterRight; )
    {
        tmp = ptr;
        ptr = NextNode(CurrentTable, ptr->A);
        DeleteNode(CurrentTable, tmp->A);
        //RtlDeleteElementGenericTableAvl(CurrentTable, tmp);
    }

    //Add the main interval
    New.A = A;
    New.B = B;
    New.k = k;
    New.UsedBy = NULL;
    AddNode(CurrentTable, New);
    //RtlInsertElementGenericTableAvl(CurrentTable, &New, sizeof(New), &ok);

    //Add the splitted intervals
    if (EdgeLeft != NULL)
        AddNode(CurrentTable, EdgeOuterLeft);
    if (EdgeRight != NULL)
        AddNode(CurrentTable, EdgeOuterRight);

    //return ok;
    return TRUE;
}

BOOLEAN unmapNode(LIB_BLOCK A, LIB_NUMBER k){
    BOOLEAN ok;
    mapNode(A, (LIB_BLOCK)NULL, k);
    ok = DeleteNode(CurrentTable, A);
    return ok;
}
