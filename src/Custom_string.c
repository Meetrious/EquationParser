#include <stdio.h>
#include <stdlib.h>
#include "../includes/Custom_string.h"
#include "../includes/misc.h"

void Construct_by_def(dcstr_t *str) {
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
        } else {
            for (int i = 0; i < str->cap; i++) {
                str->s[i] = '\0';
            }
            str->l = 0;
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
                i++;
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
        cshrink_to_fit(str);
    }
    return;
}

void Construct(dcstr_t *str, const char *src) {
    Construct_by_def(str);
    Initialise(str, src);
    return;
}

void output(dcstr_t *str) {
    if (str->l > 0) {
        for (int i = 0; i < str->l; i++){
            printf("%c", str->s[i]);
        }
    }
    return;
}

void Invalidate(dcstr_t *str) {
    str->s = NULL;
    str->l = 0;
    str->cap = -1;
    return;
}

void Destruct(dcstr_t *str) {
    if (str->cap > 0) {
        free(str->s);
        Invalidate(str);
    }
}

// ============= вспомогательный сахар ================

int cstrlen(char const *str) {
    int length = 0;
    if (str != NULL) {
        // вычисляем длину строки
        while(str[length] != '\0')
            length++;
    } else {
        //
        // в случае, если в функцию попал NULL
        length = -1;
    }
    return length;
}

void set_dcstr_fract(
    dcstr_t *plant, dcstr_t const *src,
    int fh, int th) {
// определяем
    if (th - fh < 0) {
        // если позиционный интервал пришёл в перевёрнутом состоянии
        int tmp = th;
        th = fh;
        fh = tmp;
    }
    _Bool everything_is_okay = 
        src != NULL &&  // src пришла сюда не пустой
        src->s != NULL &&  // строка в src не пуста
        th - fh != 0 && // интервал копирования не нулевой
        src->l >= th-fh;  // в src найдётся подстрока требуемой длины
    if (everything_is_okay) {
        Initialise(plant, src->s);
        if (plant->cap > 0) {
            int i;
            for (i = 0; i < th-fh; i++) {
                plant->s[i] = plant->s[fh + i];
            }
            plant->s[i] = '\0';
            plant->l = i;
            cshrink_to_fit(plant);
        } else {
            printf("failed to allocate the memory for the string");
        }
    } else {
        Invalidate(plant);
        // её можно вызывать, ведь память на plant
        // не выделялась
    }
    return;
}

void cshrink_to_fit(dcstr_t *str) {
    if (str->l > 0 && str->l + 1 < str->cap) {
        char *buf = (char*)realloc(str->s, (str->l + 1) * sizeof(char));
        if (NULL == buf) {
            // мы скорее всего сюда никогда не зайдём, ибо
            // на уменьшение выделенной памяти realloc работает
            // почти наверное безотказно
            printf("\n failed to shrink custom string:");
            debug_str_output(str);
            printf("\n\t the string is to be cleared\n");
            Destruct(str);
        } else {
            str->s = buf;
            str->cap = str->l + 1;
        }
    }
    return;
}

void debug_str_output(dcstr_t *str) {
    printf("\n \t address : %p\n", str->s);
    printf("\n \t length : %d\n", str->l);
    printf("\n \t capacity: %d\n", str->cap);
    if (str->s != NULL && str->cap > 0) {
        printf("\n \t raw content : \n \t { ");
        for (int i = 0; i < str->cap; i++) {
            if (str->s[i] == '\0') {
                printf("\\o, ");
            } else if(str->s[i] == '\n') {
                printf("\n\t ");
            } else{
                printf("%c, ", str->s[i]);
            }
        }
        printf("\b\b } \n");
        printf("\n \t display content : \n \t  ");
        output(str);
        printf("\n");
    } else {
        printf("\n \t { !!_THERE_IS_NOTHING_TO_OUTPUT_!! }\n");
    }
    return;
}
