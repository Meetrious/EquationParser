//#include "DrawLib.h"
#include "../includes/TextParseLib.h"
#include <stdlib.h>

void get_result_P03_D20();

int main() {
    get_result_P03_D20();
    return 0;
}

void get_result_P03_D20() {
    exprn_t initial;
    debug_output(&initial);
    SetInitialExpression(&initial.raw);
    // initial.raw->s = GetExprFromOutside();
    if (initial.raw.s == NULL) {
        initial.is_empty = 0;
    }
    debug_output(&initial);
    return;
}
