//#include "DrawLib.h"
#include "TextParseLib.h"
#include <stdlib.h>

void get_result_P03_D20();

int main() {
    get_result_P03_D20();
    return 0;
}

void get_result_P03_D20() {
    exprn initial;
    debug_output(&initial);
    initial.raw = GetExprFromOutside();
    if (initial.raw != NULL) {
        initial.is_empty = 0;
    }
    debug_output(&initial);
    return;
}
