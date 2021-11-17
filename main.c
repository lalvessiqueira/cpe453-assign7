#include<stdio.h> 
#include <stdlib.h> 
#include <string.h>

int pageTable[256];
char physicalMemory[256][256];
int TLB[16][2];
int counter = 0;

void pageFault(int page, int logicalAddress){
  char buffer[256];
  FILE * fp = fopen("BACKING_STORE.bin", "rb");
  fseek(fp, logicalAddress, SEEK_SET);
  fread(buffer, 256, 1, fp);
  strcpy(physicalMemory[counter],buffer);
  pageTable[page] = counter;
  fclose(fp);
}
int main(int argc,char* argv[]) {
    int i, pageNumber = 0, pageOffset = 0, flag, frameNumber, fifo_bit=0;
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

    for (i = 0; i < 256; i++) {
            pageTable[i] = -1;
    }
    for(i = 0; i < 16; i++){
        TLB[i][0]=-1;
        TLB[i][1]=-1;
    }
    int maskPageNumber = 0;
    int maskPageOffset = 0;
    for (i = 0; i < 8; i++) {
        maskPageNumber = maskPageNumber | (1 << (i + 8));
        maskPageOffset = maskPageOffset | (1 << i);
    }

    int savedLogicalAddress;
    while (fscanf(fp, "%d", &savedLogicalAddress ) != EOF) {
        flag =0;
        pageNumber = (maskPageNumber & savedLogicalAddress) >> 8;
        pageOffset = maskPageOffset & savedLogicalAddress;
        for(i = 0; i < 16; i++){
              if(TLB[i][0] == pageNumber){
                  frameNumber = TLB[i][1];
                  flag = 1;
                  break;
              }
           }
           if(!flag){
              frameNumber = pageTable[pageNumber];
              int j = 0;
              while(TLB[j][0] != -1){
                 j++;
                 if(j >= 16){
                    break;
                 }
              }
              if(j < 16 && frameNumber != -1){
                 TLB[j][0] = pageNumber;
                 TLB[j][1] = frameNumber;           
              }
              else if(j >= 16 && frameNumber != -1){
                 TLB[fifo_bit][0] = pageNumber;
                 TLB[fifo_bit][1] = frameNumber;
                 if(fifo_bit == 15)
                    fifo_bit = 0;
                 else
                    fifo_bit++;
              }
           }
        if(frameNumber == -1){
           pageFault(pageNumber, savedLogicalAddress);
           int physicalAddress = (counter << 8) + pageOffset;
           char val = physicalMemory[counter][0];
           counter++;
           printf("Virtual Address: %d Physical Address: %d Value: %d\n", savedLogicalAddress, physicalAddress, val); 
        }else{
           int physicalAddress = (frameNumber << 8)+ pageOffset;
           char val = physicalMemory[frameNumber][0]; 
           printf("Virtual Address: %d Physical Address: %d Value: %d\n", savedLogicalAddress, physicalAddress, val);
        }
    }

    return 0;
}
