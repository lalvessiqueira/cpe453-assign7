#include<stdio.h> 
#include <stdlib.h> 

/**
 * TODO: Your program will read from a file containing 
 * logical addresses and, using a TLB and a page table, 
 * will translate each logical address to its corresponding 
 * physical address and output the value of the byte 
 * stored at the translated physical address.
*/ 

void pageFault(int page);

int pageTable[256];

//rows: 16 entries
//column: page number x frame number
int TLB[16][2];

//function to resolve page faults
void pageFault(int page){}

int main(int argc,char* argv[]) {
    int i, pageNumber = 0, pageOffset = 0;
    if(argc == 1){ 
        fprintf(stderr, "No file passed");
        return EXIT_FAILURE;
    }
    char *fileName = argv[1];
    FILE *fp = fopen(fileName, "r"); 

    if (fp == NULL) { 
        fprintf(stderr,"Cannot open file."); 
        return EXIT_FAILURE;
    }

    //initializing page table array
    for (i = 0; i < 256; i++) {
            pageTable[i] = -1;
    }

    /* Your program will read a file containing 
    several 32-bit integer numbers that represent logical
    addresses. However, you need only be concerned with 
    16-bit addresses, so you must mask the rightmost 16 bits 
    of each logical address. These 16 bits are divided 
    into:
    * (1) an 8-bit page number
    * (2) an 8-bit page offset. */

    /** TODO: Mask the rightmost 16 bits of each logical address
     * bit masking = which bits you want to keep, and which bits you want to clear
     */

// to use for masking!
    int maskPageNumber = 0;
    int maskPageOffset = 0;
    for (i = 0; i < 8; i++) {
        //the first 8 bits
        maskPageNumber = maskPageNumber | (1 << (i + 8));
        //last 8 bits
        maskPageOffset = maskPageOffset | (1 << i);
    }

    int savedLogicalAddress;
    //where we get the addresses from the file
    while (fscanf(fp, "%d", &savedLogicalAddress ) != EOF) {
        
        /** TODO: Perhaps the easiest way to do this is by 
         * using the operators for bit-masking and bit-shifting. */
        pageNumber = (maskPageNumber & savedLogicalAddress) >> 8;
        pageOffset = maskPageOffset & savedLogicalAddress;

    }


}