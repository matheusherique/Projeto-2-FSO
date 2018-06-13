#include <stdio.h>
#include <stdlib.h>

void binaryToText(char *inputFile, char *outputFile);

int main(int argc, char const *argv[]) {
  address_file = fopen(argv[1], "r");
  binaryToText("BACKING_STORE.bin", "out.txt");
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
