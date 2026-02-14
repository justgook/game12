#include <stdio.h>
#include <stddef.h>
#include "../vendor/sokol_app.h"

int main() {
    printf("sizeof(sapp_touchpoint) = %zu\n", sizeof(sapp_touchpoint));
    printf("offsetof(identifier) = %zu\n", offsetof(sapp_touchpoint, identifier));
    printf("offsetof(pos_x) = %zu\n", offsetof(sapp_touchpoint, pos_x));
    printf("offsetof(pos_y) = %zu\n", offsetof(sapp_touchpoint, pos_y));
    printf("offsetof(android_tooltype) = %zu\n", offsetof(sapp_touchpoint, android_tooltype));
    printf("offsetof(changed) = %zu\n", offsetof(sapp_touchpoint, changed));
    return 0;
}
