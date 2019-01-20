#include <stdio.h>
#include <stdlib.h>
#include "ezxml2cJson.h"

int main(int argc, char* argv[]) {
    if (argc < 2){
        printf("At least one argument\n");
        return 0;
    }

    // initCDRFunct();

    FILE *fp = fopen(argv[1], "r");
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);
    char *fcontent = malloc(size);
    fread(fcontent, size, 1, fp);
    
    xml2cJson(fcontent);
    fclose(fp);
    
    puts("finished");
    return 0;
}