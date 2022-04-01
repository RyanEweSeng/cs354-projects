#include "mem.h"
#include <stdio.h>

int main() {
    Initialize_Memory_Allocator(1600);
    //Mem_Dump();
   
    char *p1 = Mem_Alloc(1591);
    if (p1 == NULL) {
        printf("Allocation 1 failed\n");
        exit(0);
    }
    Mem_Dump();
    Mem_Free(p1);
    Mem_Dump();
    
    int *p2 = Mem_Alloc(1584);
    if (p2 == NULL) {
        printf("Allocation 2 failed\n");
        exit(0);
    }
    Mem_Dump();
    Mem_Free(p2);
    Mem_Dump();
    
    int *p3 = Mem_Alloc(1585);
    if (p3 == NULL) {
        printf("Allocation 3 failed\n");
        exit(0);
    }
    Mem_Dump();
    Mem_Free(p3);
    Mem_Dump();

    int *p4 = Mem_Alloc(1583);
    if (p4 == NULL) {
        printf("Allocation 4 failed\n");
        exit(0);
    }
    Mem_Dump();
    Mem_Free(p4);
    Mem_Dump();

    Free_Memory_Allocator();
    return 0;
}
