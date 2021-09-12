#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "../includes/misc.h"

float get_random(float a, float b) {
    float tmp = (rand() % 10001)/10000.0;
    return a + (b - a) * tmp;
}

_Bool ans_is_yes() {
    _Bool trigger;
    while (1) {
        if (is_bool (&trigger)) {
            break;
        } else {
            continue;
        }
    }
    return trigger;
}

_Bool is_bool(_Bool *bl) {
    int cur_input;
    char chr;
    _Bool answer;
    int aopi = scanf("%d%c", &cur_input, &chr);
    _Bool is_really_int = aopi != 0 && (chr == ' ' || chr == '\n');
    if (is_really_int && (cur_input == 0 || cur_input == 1)) {
        answer = 1;
        *bl = (_Bool)cur_input;
    } else {
        answer = 0;
    }
    return answer;
}
