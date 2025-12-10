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
JsonObj* read_json_obj(FILE *, JsonObj *);
JsonList* read_json_list(FILE *, JsonList *);
char pass_nullch(FILE *fp);
char* read_str(FILE *, char *);

JsonObj* open_json_file(char *path, JsonObj *jo) {
    FILE *json_file = fopen(path, "r");
    if (json_file == NULL) {
        return NULL;
    }
    char ch = pass_nullch(json_file);
    // cur:'{'
    read_json_obj(json_file, jo);

    //ch = fgetc(json_file);
    // cur:'}'
    return jo;
}

JsonObj* read_json_obj(FILE *js, JsonObj *jo) {
    char ch = pass_nullch(js);
    // cur:'"l'

    //JsonNode *temp;
    int node_cnt = 0;
    while (ch!='}') {
        jo->keys[node_cnt] = (char*)malloc(sizeof(char[MAX_STR_LEN]));
        jo->vals[node_cnt] = (JsonNode*)malloc(sizeof(JsonNode));
        
        read_str(js, jo->keys[node_cnt]);
        // cur:'"r'
        
        ch = pass_nullch(js);
        // cur:':'

        ch = pass_nullch(js);
        // cur:某val开头

        // 以下函数都要回到 ',' 前
        if (isdigit(ch)) {
            #ifdef _DEBUG_
            printf("\n----------------1---------------\n");
            printf("%c", ch);
            printf("\n----------------1---------------\n");
            #endif
            jo->vals[node_cnt]->i = read_num(js);
        } else if (ch == '\"') {
            #ifdef _DEBUG_
            printf("\n----------------2---------------\n");
            printf("%c", ch);
            printf("\n----------------2---------------\n");
            #endif
            jo->vals[node_cnt]->str = (char*)malloc(sizeof(char[MAX_STR_LEN]));
            read_str(js, jo->vals[node_cnt]->str);
        } else if (ch == '{') {
            #ifdef _DEBUG_
            printf("\n----------------3---------------\n");
            printf("%c", ch);
            printf("\n----------------3---------------\n");
            #endif
            jo->vals[node_cnt]->obj = (JsonObj*)malloc(sizeof(JsonObj));
            read_json_obj(js, jo->vals[node_cnt]->obj);
        } else if (ch == '[') {
            #ifdef _DEBUG_
            printf("\n----------------4---------------\n");
            printf("%c", ch);
            printf("\n----------------4---------------\n");
            #endif
            jo->vals[node_cnt]->list = (JsonList*)malloc(sizeof(JsonList));
            read_json_list(js, jo->vals[node_cnt]->list);
        } else {
            #ifdef _DEBUG_
            printf("\n----------------5---------------\n");
            printf("%c", ch);
            printf("\n----------------5---------------\n");
            #endif
            int bool_or_nul = read_non_enclosed(js);
            if (bool_or_nul == -1) {
                jo->vals[node_cnt] = NULL;
            } else {
                jo->vals[node_cnt]->b = bool_or_nul;
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
        //jo->vals[node_cnt] = temp;
    }
    jo->num = node_cnt;
    
    return jo;
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

char* read_str(FILE *js, char *str) {
    int str_len = 0;
    char ch = fgetc(js);
    while (ch != '\"') {
        str[str_len++] = ch;
        ch = fgetc(js);
    }
    str[str_len] = '\0';
    return str;
}

JsonList* read_json_list(FILE *js, JsonList *jl) {
    char ch = pass_nullch(js);
    // cur:list内第一个非空字符

    int node_cnt = 0;
    while (ch!=']') {
        jl->vals[node_cnt] = (JsonNode*)malloc(sizeof(JsonNode));

        if (isdigit(ch)) {
            jl->vals[node_cnt]->i = read_num(js);
        } else if (ch == '\"') {
            jl->vals[node_cnt]->str = (char*)malloc(sizeof(char[MAX_STR_LEN]));/*---*/
            read_str(js, jl->vals[node_cnt]->str);
        } else if (ch == '{') {
            jl->vals[node_cnt]->obj = (JsonObj*)malloc(sizeof(JsonObj));
            read_json_obj(js, jl->vals[node_cnt]->obj);
        } else if (ch == '[') {
            jl->vals[node_cnt]->list = (JsonList*)malloc(sizeof(JsonList));
            read_json_list(js, jl->vals[node_cnt]->list);
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

// void* json_to_struct(JsonObj jf, void *s) {
    
// }