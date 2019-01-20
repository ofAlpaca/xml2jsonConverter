#include "ezxml.h"
#include "cJSON.h"

#define CDR_IPDR_CONV "CDR2IPDR.xls"

typedef char *(*funct)(const char **values, const int cnt);

typedef struct CDRFunct{
    char *name;
    funct operation;
} CDRFunct;

static CDRFunct *CDRfuncts;

void initCDRFunct();
void xml2cJson(const char* s);
cJSON * make2cJson(ezxml_t xml_root); 
void cJson_obj2array( cJSON * root, const char * obj_name );
void parse_cdr_key( cJSON * root);
char * get_obj_value( cJSON * obj, const char * name );
void fields_free();
void readCDR2IPDRConversion();