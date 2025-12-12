#include <stdlib.h>
#include <stdio.h>
#include "cjson.h"

int main(int argc, char **argv) {
    JsonObj *jobj = open_json_file("D:\\cjson\\json\\test.json");
    printf("%d\n", jobj->vals[1]->i);
    return 0;
}
