#include "ezxml.h"
#include "cJSON.h"

/*
 * xml2cJson - convert ezxml to cJson.
 * @s: the string in ezxml.
 * @return: the string in json.
 */
char *xml2cJson(const char* s);

/*
 * cJson2xml - convert cJson to ezxml.
 * @s: the string in json.
 * @return: the string in xml.
 */
char *cJson2xml(const char* s);

/*
 * make2cJson - recursivly convert a pointer to ezxml root to cJSON pointer.
 * @xml_root: the pointer to the ezxml root.
 * @return: the converted pointer to cJSON root.
 */
cJSON * make2cJson(ezxml_t xml_root); 

/*
 * make2ezxml - recursivly convert a pointer to a child of cJSON root to ezxml pointer.
 * @json_root: the pointer to the child of the cJSON root.
 * @xml_root: the parent of the xml, NULL if the function is called for the first time.
 * @return: the converted pointer of ezxml.
 */
ezxml_t make2ezxml(cJSON *json_root, ezxml_t xml_root);

/* 
 * write2file - write the buffer to a file.
 * @buffer: the content to be writen.
 * @filename: the name of the file.
 */
void write2file(const char *buffer, const char *filename);
