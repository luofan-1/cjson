#include <stdlib.h>
#include <stdio.h>
#include "cjson.h"

int main(int argc, char **argv) {
    JsonObj *jo = (JsonObj*)malloc(sizeof(JsonObj));
    // printf("Hello");
    open_json_file("D:\\cjson\\json\\test.json", jo);
    printf("%d\n", jo->vals[1]->i);
    return 0;
}
