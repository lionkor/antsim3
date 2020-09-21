#include "../Modules/GenericModule.h"
#include <stdio.h>

void on_create() {
    puts(__func__);
}

void on_destroy() {
    puts(__func__);
}

void on_update() {
    puts(__func__);
}
