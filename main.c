#include <stdio.h>
#include <stdlib.h>
#define MAX_ADDRESSES 10000
#define TLB_SIZE 16

void getPage(int address);
void readBinary(char *inputFile, char const *argv);
void insertTLB(int pageNumber, int frameNumber);

FILE *backing_store;
FILE *address_file;

int pageNumberTLB[TLB_SIZE];
int frameNumberTLB[TLB_SIZE];
int numOfTLB = 0;
int hits = 0;

int main(int argc, char const *argv[]) {
    if(argc != 2) {
        printf("Error: o arquivo addresses.txt deve ser passado como parametro\n");
        exit(1);
    }
    readBinary("BACKING_STORE.bin", argv[1]);
    printf("TLB Hits: %d\n", hits);
    fclose(address_file);
    fclose(backing_store);

    return 0;
}

void readBinary(char *inputFile, char const *argv) {
    backing_store = fopen(inputFile, "rb");
    address_file = fopen(argv, "r");
    if(address_file == NULL) {
        printf("Error: nao foi possivel abrir o arquivo\n");
        exit(1);
    }

    for(int i = 0; i < MAX_ADDRESSES; ++i) {
        int address;
        fscanf(address_file, "%d", &address);
        getPage(address);
    }
}

void getPage(int address) {
    int sprain, pageNumber, frameNumber = -1;
    pageNumber = address >> 8;
    sprain = address & 0xFFFF >> 8;
    for(int i = 0; i < TLB_SIZE; ++i){
        if(pageNumberTLB[i] == pageNumber){
            frameNumber = frameNumberTLB[i];
            hits++;
        }
    }
    insertTLB(pageNumber, frameNumber);
    printf("Logical Address: %d\nPage Number: %d\nSprain: %d\n", address, pageNumber, sprain);
}

void insertTLB(int pageNumber, int frameNumber) {
    int alreadyInsert = 0;
    int i;
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
        for(i = 0; i < numOfTLB - 1; ++i) {
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
