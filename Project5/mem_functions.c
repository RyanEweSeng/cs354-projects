#include "mem.h"
extern BLOCK_HEADER* first_header;

void Print_Block_Data(BLOCK_HEADER* header) {
    printf("size allocated: %d\n", header->size_alloc);
    printf("payload: %d\n", header->payload);
}

int Is_Free(BLOCK_HEADER* header, int size) {
    if ((header->size_alloc & 1) == 0) {
        if (header->payload >= size) {
            return 1;   
        }
        return 0;
    }
    return 0;
}

void Set_Free(BLOCK_HEADER* header) {
    header->size_alloc = header->size_alloc & ~1;
}

void Set_Allocated(BLOCK_HEADER* header) {
    header->size_alloc = header->size_alloc | 1;
}

int Get_Block_Size(int size_alloc) {
    return size_alloc & ~1;
}

int Has_Space_For_Header(BLOCK_HEADER* header, BLOCK_HEADER* last_header) {
    if ( (unsigned long)header + Get_Block_Size(header->size_alloc) < (unsigned long)last_header ) 
        return 1;
    else
        return 0;
}

BLOCK_HEADER* Get_Last_Header() {
    BLOCK_HEADER *header = first_header;
    while (header->size_alloc != 1 && header->payload != 0) {
        header = (BLOCK_HEADER*) ((unsigned long)header + Get_Block_Size(header->size_alloc));
    }
    return header;
}

BLOCK_HEADER* Get_Next_Header(BLOCK_HEADER* header) {
    BLOCK_HEADER* last_header = Get_Last_Header();
    BLOCK_HEADER* next_header = (BLOCK_HEADER*) ((unsigned long)header + header->size_alloc);
    if (next_header < last_header) return next_header;
    
    return NULL;
}

BLOCK_HEADER* Get_Prev_Header(BLOCK_HEADER* target_header) {
    BLOCK_HEADER* curr_header = first_header;
    BLOCK_HEADER* next_header = NULL;
    BLOCK_HEADER* prev_header = NULL;

    while (curr_header->size_alloc != 1 && curr_header->payload != 0) {
        next_header = (BLOCK_HEADER*) ((unsigned long)curr_header + Get_Block_Size(curr_header->size_alloc));
        if (next_header == target_header) prev_header = curr_header;

        curr_header = next_header;  
    }

    return prev_header;
}

int Get_Padding(BLOCK_HEADER* header, int header_size, int payload_size) {
    int padding = 0;
    while (((unsigned long)header + header_size + payload_size + padding - 8) % 16) padding++;
    return padding;
}

void Merge(BLOCK_HEADER* header, BLOCK_HEADER* next_header, int header_size) {
    header->payload = header->size_alloc - header_size + next_header->size_alloc;
    header->size_alloc = header->size_alloc + next_header->size_alloc;
}

// return a pointer to the payload
// if a large enough free block isn't available, return NULL
void* Mem_Alloc(int size){
    // find a free block that's big enough
    int HEADER_SIZE = 8;
    BLOCK_HEADER *header = first_header;
    BLOCK_HEADER *last_header = Get_Last_Header();
    while (header < last_header) { // keep going till the last header
        if (Is_Free(header, size)) { // block is free
            void* user_pointer = (void*) ((unsigned long)header + HEADER_SIZE); // payload pointer

            header->payload = size; // update payload
            int padding = Get_Padding(header, HEADER_SIZE, size);
            int block_size = HEADER_SIZE + size + padding; // new block size
            header->size_alloc = block_size;
            Set_Allocated(header);

            // check if there is space for a new header
            if(Has_Space_For_Header(header, last_header)) {
                // Create next header
                BLOCK_HEADER* next_header = (BLOCK_HEADER*) ((unsigned long)header + block_size);
                next_header->size_alloc = (unsigned long)last_header - (unsigned long)next_header;
                Set_Free(next_header);
                next_header->payload = next_header->size_alloc - HEADER_SIZE;
            }

            return user_pointer;
        }
        
        header = (BLOCK_HEADER*) ((unsigned long)header + Get_Block_Size(header->size_alloc));
    }
    
    return NULL; // return NULL if we didn't find a block
}


// return 0 on success
// return -1 if the input ptr was invalid
int Mem_Free(void *ptr){
    // coalesce adjacent free blocks
    int HEADER_SIZE = 8;
    BLOCK_HEADER *header = first_header;
    BLOCK_HEADER *last_header = Get_Last_Header();
    while (header < last_header) {
        if ((unsigned long)header == (unsigned long)ptr - 8) { // back up 8 bytes to header
            Set_Free(header); // change the allocated bit to 0 (block is free)

            BLOCK_HEADER* next_header = Get_Next_Header(header);
            if (next_header != NULL && (next_header->size_alloc & 1) == 0) Merge(header, next_header, HEADER_SIZE);

            BLOCK_HEADER* prev_header = Get_Prev_Header(header);
            if (prev_header != NULL && (prev_header->size_alloc & 1) == 0) Merge(prev_header, header, HEADER_SIZE);
            
            if (next_header == NULL && prev_header == NULL) header->payload = header->size_alloc - HEADER_SIZE;

            return 0;
        }

        header = (BLOCK_HEADER*) ((unsigned long)header + Get_Block_Size(header->size_alloc));
    }

    return -1; // input ptr not found
}

