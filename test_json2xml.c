#include <stdio.h>
#include <stdlib.h>
#include "xml2jsonConv.h"

int main(int argc, char* argv[]) {
    if (argc < 2){
        printf("At least one argument\n");
        return 0;
    }

    FILE *fp = fopen(argv[1], "r");
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);
    char *fcontent = malloc(size);
    fread(fcontent, size, 1, fp);
    
    char *xml_content = cJson2xml(fcontent);
    write2file(xml_content, "json2xml_output.xml");

    free(xml_content);
    free(fcontent);
    fclose(fp);
    return 0;
}