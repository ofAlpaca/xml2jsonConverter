#include <stdio.h>
#include <stdlib.h>
#include "xml2jsonConv.h"

/* 
 * write2file - write the buffer to a file.
 * @buffer: the content to be writen.
 * @filename: the name of the file.
 */
void write2file(const char *buffer, const char *filename) {
    FILE * fp = fopen(filename, "w");
    fwrite(buffer, strlen(buffer), 1, fp);
    fclose(fp);
}

/*
 * xml2cJson - convert ezxml to cJson.
 * @s: the string in ezxml.
 * @return: the string in json.
 */
char *xml2cJson(const char* s){
    ezxml_t xml_root = ezxml_parse_str(s, strlen(s));

    cJSON *json_root = make2cJson(xml_root);
    char *str_cJson = cJSON_Print(json_root);
    
    ezxml_free(xml_root);
    cJSON_Delete(json_root);
    return str_cJson;
}

/*
 * cJson2xml - convert cJson to ezxml.
 * @s: the string in json.
 * @return: the string in xml.
 */
char *cJson2xml(const char* s){
    cJSON *json_root = cJSON_Parse(s);

    json_root = json_root->child;
    ezxml_t xml_root = make2ezxml(json_root, NULL);
    char *str_ezxml = ezxml_toxml(xml_root);

    cJSON_Delete(json_root);
    ezxml_free(xml_root);
    return str_ezxml;
}

/*
 * make2cJson - recursivly convert a pointer to ezxml root to cJSON pointer.
 * @xml_root: the pointer to the ezxml root.
 * @return: the converted pointer to cJSON root.
 */
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

/*
 * make2ezxml - recursivly convert a pointer to a child of cJSON root to ezxml pointer.
 * @json_root: the pointer to the child of the cJSON root.
 * @xml_root: the parent of the xml, NULL if the function is called for the first time.
 * @return: the converted pointer of ezxml.
 */
ezxml_t make2ezxml(cJSON *json_root, ezxml_t xml_root) {
    cJSON *element = json_root, *js_item = NULL;
    ezxml_t child_tag = NULL, item_tag = NULL;
    // if xml_root is null, then add pseudo tag.
    if(!xml_root)
        xml_root = ezxml_new("PseudoTag");

    while(element){

        puts("-------------------------------");
        if (element->string)
            printf("js name is '%s'\n", element->string);
        else
            puts("this js has no name");
        printf("js type is '%d'\n", element->type);

        if(element->type == cJSON_String || element->type == cJSON_Raw){
            // the element is string or raw text, create a child and add a text.
            if (element->string) {
                printf("js value is '%s'\n", element->valuestring);
                child_tag = ezxml_add_child(xml_root, element->string, 0);
                ezxml_set_txt(child_tag, element->valuestring);
            }else{
                ezxml_set_txt(xml_root, element->valuestring);
                break;
            }
        }else if(element->type == cJSON_Number){
            // the element is digit, convert the digit to string and create a child then add the string.
            char *digit_str = malloc(64);
            sprintf(digit_str, "%.2f", element->valuedouble);
             printf("js value is '%s'\n", digit_str);
            if (element->string) {
                child_tag = ezxml_add_child(xml_root, element->string, 0);
                ezxml_set_txt(child_tag, digit_str);
            }else{
                ezxml_set_txt(xml_root, digit_str);
                break;
            }
        }else if(element->type == cJSON_True || element->type == cJSON_False){
            // the element is boolean, create a child and add true or false.
            printf("js value is '%s'\n", cJSON_Print(element));
            if (element->string) {
                child_tag = ezxml_add_child(xml_root, element->string, 0);
                ezxml_set_txt(child_tag, cJSON_Print(element));
            }else{
                ezxml_set_txt(xml_root, cJSON_Print(element));
                break;
            }
        }else if(element->type == cJSON_Object){
            // the element is an object, recursivly call make2ezxml and insert back to root.
            printf("js value is 'object'\n");
            if (element->string){
                item_tag = ezxml_new(element->string);
                make2ezxml(element->child, item_tag);
                ezxml_insert(item_tag, xml_root, 0);
            }else{
                make2ezxml(element->child, xml_root);
                break;
            } 
        }else if(element->type == cJSON_Array){
            // the element is an array, loop the items inside the array.
            int arr_size = cJSON_GetArraySize(element);
            puts("js value is 'array'");
            for(int i = 0 ; i < arr_size ; ++i){
                js_item = cJSON_GetArrayItem(element, i);

                item_tag = ezxml_new(element->string);

                make2ezxml(js_item, item_tag);
                    
                ezxml_insert(item_tag, xml_root, 0);
                puts("js item of array done");
            }
        }

        // next element in json.
        element = element->next;
        puts("next->>>>");
    }
    puts("================================");
    return xml_root;
}