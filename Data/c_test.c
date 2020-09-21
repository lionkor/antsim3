#include "../Modules/GenericModule.h"

void on_create(C_Entity* entity) {
    o_warning("yay!");
}

static const char* version_str = "v0.1";

const char* version() {
    return version_str;
}
