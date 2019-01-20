#include <stdio.h>
#include <stdlib.h>
#include "xml2jsonConv.h"

int main(int argc, char* argv[]) {
    
    FILE *fp = fopen(argv[1], "r");
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);
    char *fcontent = malloc(size);
    fread(fcontent, size, 1, fp);
    
    char *json_content = cJson2xml(fcontent);
    printf("%s\n", json_content);

    free(json_content);
    fclose(fp);
    return 0;
}