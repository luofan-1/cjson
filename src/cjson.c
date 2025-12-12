#include "cjson.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define N_DEBUG_

// #define PUNCTUATION_MATCH(ch) _Generic((ch), \
//     '{' : ')', \
//     '[' : ']', \
//     '\"' : '\"' \
// )

enum bool_or_null {
    JSON_NULL_E = -1,
    JSON_FALSE_E = 0,
    JSON_TRUE_E = 1,
};

int read_non_enclosed(FILE *fp);
int read_num(FILE *fp);
JsonObj* read_json_obj(FILE *);
JsonList* read_json_list(FILE *);
char pass_nullch(FILE *fp);
char* read_str(FILE *);

JsonObj* open_json_file(char *path) {
    FILE *json_file = fopen(path, "r");
    JsonObj *jobj = (JsonObj*)malloc(sizeof(JsonObj));
    if (json_file == NULL) {
        return NULL;
    }
    char ch = pass_nullch(json_file);
    // cur:'{'
    jobj = read_json_obj(json_file);

    //ch = fgetc(json_file);
    // cur:'}'
    return jobj;
}

JsonObj* read_json_obj(FILE *js) {
    JsonObj *jobj = (JsonObj*)malloc(sizeof(JsonObj));
    char ch = pass_nullch(js);
    // cur:'"l'

    //JsonNode *temp;
    int node_cnt = 0;
    while (ch!='}') {
        //jobj->keys[node_cnt] = (char*)malloc(sizeof(char[MAX_STR_LEN]));
        jobj->vals[node_cnt] = (JsonNode*)malloc(sizeof(JsonNode));
        
        jobj->keys[node_cnt] = read_str(js);
        // cur:'"r'
        
        ch = pass_nullch(js);
        // cur:':'

        ch = pass_nullch(js);
        // cur:某val开头

        // 以下函数都要回到 ',' 前
        if (isdigit(ch)) {
            jobj->vals[node_cnt]->i = read_num(js);
        } else if (ch == '\"') {
            //jobj->vals[node_cnt]->str = (char*)malloc(sizeof(char[MAX_STR_LEN]));
            jobj->vals[node_cnt]->str = read_str(js);
        } else if (ch == '{') {
            //jobj->vals[node_cnt]->obj = (JsonObj*)malloc(sizeof(JsonObj));
            jobj->vals[node_cnt]->obj = read_json_obj(js);
        } else if (ch == '[') {
            //jobj->vals[node_cnt]->list = (JsonList*)malloc(sizeof(JsonList));
            jobj->vals[node_cnt]->list = read_json_list(js);
        } else {
            int bool_or_nul = read_non_enclosed(js);
            if (bool_or_nul == -1) {
                jobj->vals[node_cnt] = NULL;
            } else {
                jobj->vals[node_cnt]->b = bool_or_nul;
            }
        }
        node_cnt ++;

        ch = pass_nullch(js);
        // cur:','

        if (ch == '}') {
            break;
        }

        ch = pass_nullch(js);
        // cur:'"l'
        //jobj->vals[node_cnt] = temp;
    }
    jobj->num = node_cnt;
    
    return jobj;
}

int read_num(FILE *js) {
    int i;
    fseek(js, -1, SEEK_CUR);
    fscanf(js, "%d", &i);
    return i;
}

int read_non_enclosed(FILE *js) {
    fseek(js, -1, SEEK_CUR);
    char *str = (char*)malloc(sizeof(char[MAX_STR_LEN]));
    // fscanf(js, "%s", str);
    
    int str_len = 0;
    char ch = fgetc(js);
    while (isalpha(ch)) {
        str[str_len++] = ch;
        ch = fgetc(js);
    }
    str[str_len] = '\0';
    fseek(js, -1, SEEK_CUR);
    if (!strcmp(str, "null")) {
        return JSON_NULL_E;
    } else if (!strcmp(str, "true")) {
        return JSON_TRUE_E;
    } else if (!strcmp(str, "false")) {
        return JSON_FALSE_E;
    }

    // error val:-2
    return -2;
}

char* read_str(FILE *js) {
    char *str = (char*)malloc(sizeof(char[MAX_STR_LEN]));
    int str_len = 0;
    char ch = fgetc(js);
    while (ch != '\"') {
        str[str_len++] = ch;
        ch = fgetc(js);
    }
    str[str_len] = '\0';
    return str;
}

JsonList* read_json_list(FILE *js) {
    JsonList *jl = (JsonList*)malloc(sizeof(JsonList));
    char ch = pass_nullch(js);
    // cur:list内第一个非空字符

    int node_cnt = 0;
    while (ch!=']') {
        jl->vals[node_cnt] = (JsonNode*)malloc(sizeof(JsonNode));

        if (isdigit(ch)) {
            jl->vals[node_cnt]->i = read_num(js);
        } else if (ch == '\"') {
            //jl->vals[node_cnt]->str = (char*)malloc(sizeof(char[MAX_STR_LEN]));/*---*/
            jl->vals[node_cnt]->str = read_str(js);
        } else if (ch == '{') {
            //jl->vals[node_cnt]->obj = (JsonObj*)malloc(sizeof(JsonObj));
            jl->vals[node_cnt]->obj = read_json_obj(js);
        } else if (ch == '[') {
            //jl->vals[node_cnt]->list = (JsonList*)malloc(sizeof(JsonList));
            jl->vals[node_cnt]->list = read_json_list(js);
        } else {
            int bool_or_nul = read_non_enclosed(js);
            if (bool_or_nul == -1) {
                jl->vals[node_cnt] = NULL;
            } else {
                jl->vals[node_cnt]->b = bool_or_nul;
            }
        }
        node_cnt ++;

        ch = pass_nullch(js);
        // cur:','

        if (ch == ']') {
            break;
        }
        
        ch = pass_nullch(js);
        // 
    }
    jl->num = node_cnt;
    return jl;
}

/**
 * @return 返回当前指向字符
 */
char pass_nullch(FILE *fp) {
    char ch;
    while (((ch=fgetc(fp))=='\n'||ch==' ') && ch!=EOF) {
        
    }
    // fseek(fp, -1, SEEK_CUR);
    return ch;
}

// void* json_to_struct(JsonObj jobj, void *s) {
    
// }