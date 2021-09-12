#include <stdio.h>
#include "../../includes/TextParseLib.h"

int main() {
    // объект исходного выражения
    exprn_t initial;

    // выделяем память
    ConstructExprn(&initial);
    printf("\n initially constructed expr-obj");
    debug_output(&initial);

    printf("\n raw initialised expression");
    SetInitialExpression(&initial);
    debug_output(&initial);

    printf("\n operation defined & memory for branches");
    divide_expression(&initial);
    debug_output(&initial);


    debug_output(initial.ttl);
    debug_output(initial.ttr);

    DestructExprn(initial.ttl);
    DestructExprn(initial.ttr);
    DestructExprn(&initial);

    printf("\n Destroyed initial expresion");
    debug_output(&initial);

    return 0;
}