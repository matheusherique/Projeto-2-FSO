#include <stdio.h>
#include <stdlib.h>
#define MAX_ADDRESSES 10000

void getPage(int address);
void binaryToText(char *inputFile, char *outputFile);

int main(int argc, char const *argv[]) {
    if(argc != 2) {
        printf("Error: should be ./proj02.out addresses.txt\n");
        exit(1);
    }
    FILE *address_file = fopen(argv[1], "r");
    if(address_file == NULL) {
        printf("Error: couldn't open the file addresses.txt\n");
    }
    for(int i = 0; i < MAX_ADDRESSES; ++i) {
        int address;
        fscanf(address_file, "%d", &address);
        getPage(address);
    }
    return 0;
}

void binaryToText(char *inputFile, char *outputFile) {
    unsigned char str[256];
    unsigned int num;  // assuming 32 bit ints
    int i, len;

    FILE *finp = fopen(inputFile, "rb");
    FILE *fout = fopen(outputFile, "w");

    while ((len = fgetc(finp)) != EOF) {
        fread(str, len, 1, finp);
        str[len] = '\0';
        num  = (unsigned int)fgetc(finp) << 24;
        num |= fgetc(finp) << 16;
        num |= fgetc(finp) << 8;
        num |= fgetc(finp);
        fprintf(fout, "%s %d\n", (char*)str, num);
    }
    fclose(finp);
    fclose(fout);
}

void getPage(int address) {
    int sprain, pageNumber;
    pageNumber = address >> 8;
    sprain = address & 0xFFFF >> 8;
    printf("Logical Address: %d\nPage Number: %d\nSprain: %d\n", address, pageNumber, sprain);
}