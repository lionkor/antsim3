#include "../Modules/GenericModule.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void on_create(C_Entity* entity) {
    C_vec_d delta = { 10, -10 };
    move_by(entity, delta);
    C_vec_d pos = get_position(entity);
    char msg[100];
    strcpy(msg, "created entity at position ");
    char buf[100];
    sprintf(buf, "(%f, %f)", pos.x, pos.y);
    strcat(msg, buf);
    o_info(msg);
}

void on_destroy(C_Entity* entity) {
    o_info("entity destroyed");
}

static const char* version_str = "v0.1";

const char* version() {
    return version_str;
}
