#include <stdio.h>
#include <stdlib.h>
#include "xml2jsonConv.h"

// write the file.
void write2file(const char *buffer, const char *filename) {
    FILE * fp = fopen(filename, "w");
    fwrite(buffer, strlen(buffer), 1, fp);
    fclose(fp);
}

// Input a string and parse it to ezxml format. 
char *xml2cJson(const char* s){
    ezxml_t xml_root = ezxml_parse_str(s, strlen(s));

    cJSON *json_root = make2cJson(xml_root);
    char *str_cJson = cJSON_Print(json_root);
    
    ezxml_free(xml_root);
    cJSON_Delete(json_root);
    return str_cJson;
}

// Input a string and parse it to cJSON format.
char *cJson2xml(const char* s){
    cJSON *json_root = cJSON_Parse(s);

    json_root = json_root->child;
    ezxml_t xml_root = make2ezxml(json_root, NULL);
    char *str_ezxml = ezxml_toxml(xml_root);

    cJSON_Delete(json_root);
    ezxml_free(xml_root);
    return str_ezxml;
}

// Recursivly call the function to parse it to cJson format,
// returning a cJson root.
cJSON * make2cJson(ezxml_t xml_root) {
    if (!xml_root) return NULL;

    int i = 0;
    char *array_element_name = NULL;
    char** tag_attr = xml_root->attr;
    ezxml_t elemets = xml_root->child, sibling_element = NULL;
    cJSON *json_root = cJSON_CreateObject(), *js_body = NULL, *js_list = NULL, *js_tmp = NULL;
    int hasChild = xml_root->child;
    int hasAttr = *tag_attr;
    
    if (hasAttr || hasChild) { // has child, so create elements
        cJSON_AddItemToObject(json_root, xml_root->name, js_body = cJSON_CreateObject());
        // create tag attributes
        while(tag_attr[i] != NULL){
            cJSON_AddStringToObject(js_body, tag_attr[i], tag_attr[i+1]);
            i += 2;
        }

        // if it has child, then create an element for it.
        if (hasChild) {
            // create elements
            while(elemets) {
                js_tmp = make2cJson(elemets);
                // if elements has 'next', then make it array.
                if(elemets->next || array_element_name){
                    if(!js_list){ // the first element in the array,
                        js_list = cJSON_CreateArray();
                        int len = strlen(elemets->name);
                        array_element_name = malloc(len);
                        strncpy(array_element_name, elemets->name, len);
                        sibling_element = elemets->sibling;
                    }

                    cJSON_AddItemToArray(js_list, js_tmp->child);
                    elemets = elemets->next;

                    if(!elemets){ // the last element in the array.
                        cJSON_AddItemToObject(js_body, array_element_name, js_list);
                        js_list = NULL;
                        free(array_element_name); // release the name of element
                        array_element_name = NULL;

                        elemets = sibling_element; // reset the element ptr to next different child.
                        sibling_element = NULL;
                    }
                } else {    
                    cJSON_AddItemToObject(js_body, elemets->name, js_tmp->child);
                    elemets = elemets->sibling;
                }
                free(js_tmp); // release its parent node
            }
        }else{ // no child, so create string
            cJSON_AddStringToObject(js_body, "#text", xml_root->txt);
        }   
    }else { // no attributes and childs, just string.
        cJSON_AddStringToObject(json_root, xml_root->name, xml_root->txt);
    }

    return json_root;
}

ezxml_t make2ezxml(cJSON *json_root, ezxml_t xml_root) {
    
    cJSON *element = json_root;
    ezxml_t child_tag = NULL;
    
    if(!xml_root)
        xml_root = ezxml_new(element->string);

    printf("root : %s\n",element->string);

    while(element){
        if(element->type == cJSON_Object){
            printf("object %s\n", element->string);
            child_tag = make2ezxml(element->child, xml_root);
            printf("insert \n");
            ezxml_insert(child_tag, xml_root, 0);
        }else if(element->type == cJSON_String){
            printf("string %s\n", element->string);
            child_tag = ezxml_add_child(xml_root, element->string, 0);
            ezxml_set_txt(child_tag, element->valuestring);
        }

        /*
        if(element->child){
            child = element->child;
            while(child){
                // it's an attribute.
                if(child->type == cJSON_Array){
                    
                }else{
                    ezxml_add_child(element, child->string, 0);
                    ezxml_set_txt()
                }
                
                child = child->next;
            }
        }*/

        element = element->next;
    }

    return xml_root;
}