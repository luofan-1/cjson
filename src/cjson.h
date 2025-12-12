#ifndef _CJSON_H_
#define _CJSON_H_

#define MAX_STR_LEN 64
#define MAX_NODE_NUM 64
#define MAX_JSON_TREE_DEPTH 32

#include <stdio.h>

// #define null NULL;

typedef struct JsonObj JsonObj;
typedef struct JsonList JsonList;
typedef union JsonNode JsonNode;

union JsonNode {
    int i;
    int b;
    char *str;
    JsonObj *obj;
    JsonList *list;
};

struct JsonList {
    int num;
    JsonNode *vals[MAX_NODE_NUM];
};

typedef struct Dict {
    //keys;
    //val_idx;
} Dict;

struct JsonObj {
    int num;
    char *keys[MAX_NODE_NUM];
    JsonNode *vals[MAX_NODE_NUM];
    Dict *pair;
};

// for JsonObj
JsonNode* find_node_by_key(JsonObj *jobj, char *key, JsonNode *target);
int print_json_tree(FILE *stream, JsonObj *jobj);
void* json_to_struct(JsonObj jobj, void* s);


// for JsonList
// JL JsonList*
// TARGET TYPE*
// 感觉没什么用，删掉了
// #define jsonlist_to_array(TYPE, JL, TARGET) _Generic( \
//     (TYPE), \
//     (int):(jsonlist_to_iarr(JL, TARGET)), \
//     (bool):(jsonlist_to_barr(JL, TARGET)), \
//     (string):(jsonlist_to_sarr(JL, YARGET)), \
//     (JsonList):(jsonlist_to_larr(JL, TARGET)), \
//     (JsonObj):(jsonlist_to_oarr(JL, TARGET)) \
// )

JsonObj* open_json_file(char *path);


#endif