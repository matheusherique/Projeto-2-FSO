#include <stdio.h>
#include <stdlib.h>
#define MAX_ADDRESSES 10000

void getPage(int address);
void readBinary(char *inputFile, char const *argv);

FILE *backing_store;
FILE *address_file;

int pageNumberTBL[16];
int frameNumberTBL[16];
int hits = 0;

int main(int argc, char const *argv[]) {

    if(argc != 2) {
        perror("Error: ");
        return(-1);
    }
    readBinary("BACKING_STORE.bin", argv[1]);
    if(address_file == NULL) {
        perror("Error: ");
        return(-1);
    }

    fclose(address_file);
    fclose(backing_store);

    return 0;
}

void readBinary(char *inputFile, char const *argv) {
    unsigned char str[256];

    backing_store = fopen(inputFile, "rb");
    address_file = fopen(argv, "r");

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
    for(int i = 0; i < 16; ++i){
        if(pageNumberTBL[i] == pageNumber){
            frameNumber = frameNumberTBL[i];
                hits++;
        }
    }
    printf("Logical Address: %d\nPage Number: %d\nSprain: %d\n Hits: %d\n", address, pageNumber, sprain, hits);
}
