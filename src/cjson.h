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

struct JsonObj {
    int num;
    char *keys[MAX_NODE_NUM];
    JsonNode *vals[MAX_NODE_NUM];
};


JsonObj* open_json_file(char *path, JsonObj *jo);

void* json_to_struct(JsonObj jf, void* s);

#endif