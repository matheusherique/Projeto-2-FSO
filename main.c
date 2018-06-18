#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "log.h"
#define MAX_ADDRESSES 10000
#define TLB_SIZE 16
#define MAX_FRAME 256
#define ADDRESS_MASK 65535
#define SPRAIN_MASK 255

void getPage(int address);
void readBinary(char *inputFile, char const *argv);
void insertTLB(int pageNumber, int frameNumber);
void readStore(int pageNumber);

FILE *backingStore;
FILE *addressFile;

int pageFaults = 0, virtualAdress;
int physicalMemory[MAX_FRAME][MAX_FRAME];
int pageNumberTLB[TLB_SIZE];
int frameNumberTLB[TLB_SIZE];
int pageNumbersTable[MAX_FRAME];
int framesTable[MAX_FRAME];
int numOfTLB = 0, hits = 0, firstFrame = 0, firstPage = 0;
char logicalAddress[10];
signed char memoryByteValue;
signed char buffer[256];

int main(int argc, char const *argv[]) {
    if(argc != 2) {
        log_error("Error: o arquivo addresses.txt deve ser passado como parametro\n", errno);
        log_error("Error open file: %s\n", strerror(errno));
        exit(1);
    }
    readBinary("BACKING_STORE.bin", argv[1]);
    printf("TLB Hits: %d\n", hits);
    printf("TLB Hit Percentual = %.4f\n", (double) hits / (double)MAX_ADDRESSES);
    printf("Page Faults: %d\n", pageFaults);
    printf("Page Faults Percentual: %.4f\n", (double) pageFaults / (double)MAX_ADDRESSES);
    fclose(addressFile);
    fclose(backingStore);

    return 0;
}

void readBinary(char *inputFile, char const *argv) {
    backingStore = fopen(inputFile, "rb");
    addressFile = fopen(argv, "r");
    if(addressFile == NULL) {
        log_error("Error no %d in opening file...\n", errno);
        log_error("Error open file: %s\n", strerror(errno));
        exit(1);
    }

    for(int i = 0; i < MAX_ADDRESSES; ++i) {
        fscanf(addressFile, "%d", &virtualAdress);
        getPage(virtualAdress);
    }
}

void getPage(int virtualAdress) {
    int sprain = virtualAdress & SPRAIN_MASK, pageNumber = (virtualAdress & ADDRESS_MASK) >> 8, frame = -1;
    for(int i = 0; i < TLB_SIZE; ++i){
        if(pageNumberTLB[i] == pageNumber){
            frame = frameNumberTLB[i];
            hits++;
        }
    }

    if(frame == -1){
        for(int i = 0; i < firstPage; ++i){
            if(pageNumbersTable[i] == pageNumber){
                frame = framesTable[i];
            }
        }
        if(frame == -1){
            readStore(pageNumber);
            pageFaults++;
            frame = firstFrame - 1;
        }
    }

    insertTLB(pageNumber, frame);
    memoryByteValue = physicalMemory[frame][sprain];
    printf("Page Number: %d\nLogical Address: %d\nSprain: %d\nFrame Number: %d\n", pageNumber, virtualAdress, sprain, frame);
}

void insertTLB(int pageNumber, int frameNumber) {
    int alreadyInsert = 0, i;
    for(i = 0; i < numOfTLB; ++i) {
        if(pageNumberTLB[i] == pageNumber) {
            alreadyInsert = 1;
            break;
        }
    }
    if(alreadyInsert == 1) return;
    if(i == numOfTLB) {
        if(numOfTLB < TLB_SIZE) {
            pageNumberTLB[numOfTLB] = pageNumber;
            frameNumberTLB[numOfTLB] = frameNumber;
        } else {
            for(i = 0; i < TLB_SIZE - 1; ++i) {
                pageNumberTLB[i] = pageNumberTLB[i + 1];
                frameNumberTLB[i] = frameNumberTLB[i + 1];
            }
            pageNumberTLB[numOfTLB - 1] = pageNumber;
            frameNumberTLB[numOfTLB - 1] = frameNumber;
        }
    } else {
        for(i = i; i < numOfTLB - 1; ++i) {
            pageNumberTLB[i] = pageNumberTLB[i + 1];
            frameNumberTLB[i] = frameNumberTLB[i + 1];
        }
        if(numOfTLB < TLB_SIZE) {
            pageNumberTLB[numOfTLB] = pageNumber;
            frameNumberTLB[numOfTLB] = frameNumber;
        } else {
            pageNumberTLB[numOfTLB - 1] = pageNumber;
            frameNumberTLB[numOfTLB - 1] = frameNumber;
        }
    }
    if(numOfTLB < TLB_SIZE) numOfTLB++;
}

void readStore(int pageNumber){
    for(int i = 0; i < 256; ++i){
        physicalMemory[firstFrame][i] = buffer[i];
    }

    pageNumbersTable[firstPage] = pageNumber;
    framesTable[firstPage] = firstFrame;

    firstFrame++;
    firstPage++;
}
