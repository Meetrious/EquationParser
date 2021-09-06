#include <stdio.h>
#include "Custom_string.h"


void Construct(dcstr_t *str) {
    str->cap = 0;
    while (str->cap == 0) {
        str->cap = 10;
        str->s = (char*)malloc(str->cap * sizeof(char));
        if (str->s == NULL) {
            printf("\n Failed to allocate memory for some string\n ");
            printf("Do you wish to try again ? \n 0. NO;\n 1. YES;\n ans: ");
            if (ans_is_yes()) {
                str->cap = 0;
            } else {
                str->cap = -1;
            }
        }
    }
    return;
}

void Initialise(dcstr_t *str, const char *src) {
    if (str->cap > 0) {
        int i = 0;
        while (1) {
            while (i < str->cap) {
                str->s[i] = src[i];
                if (src[i] == '\0') {
                    str->l = i;
                    break;
                }
            }
            while (i == str->cap && src[i] != '\0') {
                int tmp = str->l * 1.5;
                char *buf = (char*)realloc(str->s, tmp * sizeof(char));
                if (buf == NULL) {
                    printf("\n Failed to reallocate memory for the string");
                    printf("\n Do you wish to try again ? \n 0. NO;\n 1. YES;\n ans: ");
                    if (ans_is_yes()) {
                        // ничего не меняем, выходим во внешний цикл;
                        continue;  // пробуем снова;
                    } else {
                        str->l = 0;
                        free(str->s);
                    }
                } else {
                    str->s = buf;
                    str->cap = tmp;
                }
            }
            if (src[i] == '\0' || str->l <= 0) {
                // если мы закончили в какой угодно форме
                break;  // выходим из бесконечного цикла
            }
        }  // while(true)
    }
    return;
}

void Construct(dcstr_t *str, const char *src) {
    Construct(str);
    Initialise(str, src);
    return;
}

void Destruct(dcstr_t *str) {
    if (str->l > 0) {
        free(str->s);
        str->s = NULL;
    }
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