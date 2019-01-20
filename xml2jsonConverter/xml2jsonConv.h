#include "ezxml.h"
#include "cJSON.h"

char *xml2cJson(const char* s);
char *cJson2xml(const char* s);
cJSON * make2cJson(ezxml_t xml_root); 
ezxml_t make2ezxml(cJSON *json_root, ezxml_t xml_root) ;
void write2file(const char *buffer, const char *filename);
