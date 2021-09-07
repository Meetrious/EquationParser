#include <stdio.h>  // printf
#include <stdlib.h>  // здесь лежит malloc

// здесь лежат объявления функций, которые
// будут определены ниже
#include "../includes/TextParseLib.h"

// здесь находятся операции, относительно которых
// будет разбиваться выражение
#include "../includes/operations.h" 

// объявляем в глобальном поле константный массив
// из указателей на функции с сигнатурами 
// (double x double) -> double
//
// Модификатор static в данном случае означает, что
// область видимости объекта oper_list не покидает границ
// текущего си-файла. Мы не можем обратиться к oper_list 
// в си-файле с точкой входа int main
static const double (*oper_list[])(double, double) = {
    get_addition, 
    get_addition,
    get_product,
    get_fraction,
    get_sin,
    get_cos,
    get_tg,
    get_ctg,
    get_ln,
    get_log10,
    get_sqrt,
    get_id
};


// объявляем в глобальном поле константный статический массив
// из идентификаторов операций, которые могут встретиться внутри
// наших raw - математических выражений
//
// Он является статическим по аналогичным причинам
static const char *oper_names[] = {
    "+", 
    "-",
    "*", 
    "/", 
    "sin", 
    "cos", 
    "tg", 
    "ctg", 
    "ln", 
    "log10", 
    "sqrt", 
    "\0"  // соответствует тождественному оператору
};

// объявляем в глобальном поле константный массив из строк
const char *expr_name = "expression.txt";


void debug_output(exprn_t *knot) {
    printf("\n");
    printf("current adress = %p\n", knot);
    printf("current raw expression = %s \n", knot->raw.s);
    printf("current operation = %s \n", knot->oper.name);
    printf("left sub_expression adress = %p\n", knot->ttl);
    printf("right sub_expression adress = %p\n", knot->ttr);
    printf("is_empty flag state = %d\n", knot->is_empty);
    printf("current stack = { %lf, %lf }\n", knot->stack[0], knot->stack[1]);
    printf("\n");
    return;
}

char *GetExprFromOutside() {
    char *str = NULL;
    FILE *instream = NULL;
    int len = 10;
    instream = fopen(expr_name, "rt");
    if (instream != NULL) {
        str = (char*)malloc(len * sizeof(char));
        if (str != NULL) {
            int i = 0;
            str[i] = '\0';
            while (1) {
                while (i < len - 1 && !feof(instream)) {
                    str[i+1] = '\0';
                    fscanf(instream, "%c", &str[i]);
                    i++;
                }
                if (i == len - 1 && !feof(instream)) {
                    len *= 2;
                    str = (char*)realloc(str, len * sizeof(char));
                    if (str == NULL) {
                        printf("\n failed to reallocate memory for the expression");
                        break;
                    }
                } else {
                    break;
                }
            }
        }
    } else {
        printf("\n failed to open stream for input");
    }
    if (instream != NULL)
        fclose(instream);
    len = 0;
    while (str[len]!='\0')
        len++;
    str = (char*)realloc(str, (len + 1) * sizeof(char));
    if (str == NULL) 
        printf("\n failed to reallocate memory for the expression");
    return str;
}

void ExprDestruct(exprn_t *knot) {
    free(knot->ttl);
    knot->ttl = NULL;
    free(knot->ttr);
    knot->ttr = NULL;
    Destruct(&knot->raw);
    Destruct(&knot->oper.name);
    knot->is_empty = 1;
    return;
}


int get_find_operation(dcstr_t *eq, char *oper) {
    //
    // сюда поместим результат поиска операции
    int res;
    //
    // буфер для хранения длины идентификатора операции
    int oper_len = cstrlen(oper);
    int i;
    for (i = 0; i < eq->l - oper_len; i++) {
        int j = 0;
        if (eq->s[i] == '(') {
            //
            // если в выражении наткнулись на скобку
            while (eq->s[i] != ')') {
                i++;  // пропускаем содержимое скобки
            }
        } else if (i < eq->l - oper_len) {
            //
            // если до конца строки с уравнением осталось больше позиций,
            // чем длина идентификатора операции
            for (j = 0; j < oper_len; j++) {
                //
                // ищем вхождение идентификатора операции в строке с уравнением
                if (eq->s[i+j] == oper[j] ) {
                    continue;
                } else {
                    break;
                }
            }
        }
        if (j == oper_len) {
            //
            // если цикл перебора символов в операции в поисках вхождения
            // хоть раз дошёл до конца длины операции
            break; // выходим из цикла на i преждевременно,
            // ведь операция была обнаружена
        }
    }
    if (i >= eq->l - oper_len) {
        //
        // если индекс перебора символов в строке с уравнением
        // пересёк границу eq->l - oper_len
        res = -1;  // указываем на терминатор
        // ибо операция не была найдена
    } else {
        //
        // иначе указываем на позицию, с которой начинается операция
        res = i;
    }
    return res;
}

void define_operation (exprn_t *cur) {
    int i;
    for (i = 0; *oper_names[i] != '\0'; i++) {
        cur->oper.pos = get_find_operation(&cur->raw, oper_names[i]);
        if (cur->oper.pos != -1) {
            //
            // если попали сюда, то на текущий момент в i
            // лежит индекс нужной операции, относительно которой
            // будет производиться разбиение сur->raw.s
            break;
        }
    }
    // определяем имя операции в cur->oper
    Construct(&cur->oper.name, oper_names[i]);

    // назначаем фактическую get_операцию для текущего узла
    cur->oper.action = oper_list[i];
    return;
}

void filter_brackets(char *expr) {
    if (expr[0] == '(') {
        int i = 0;
        while (expr[i] != ')') {
            i++;
        }
        if (expr[i+1] == '\0') {
            //
            // если скобка закрылась прямо перед концом выражения
            for (int j = 0; expr[j] != '\0'; j++) {
                expr[j] = expr[j+1];
            }
        } 
    }
    return;
}

void set_subexpressions(exprn_t *cur) {
    
    // длина левого подвыражения = позиции на которой начинается
    // операция + позиция для терминатора
    cur->ttl->raw = get_dyn_str(cur->oper.pos + 1);
    if (cur->ttl->raw != NULL) {
        for (int i = 0; i <= cur->oper.pos; i++) {
            cur->ttl->raw[i] = cur->raw[i];
        }
    }
    int oper_len = 0;
    while (cur->oper.name[oper_len] != '\0')
        oper_len++;

    int i = cur->oper.pos + oper_len;

    int eq_end = 0;
    while (cur->raw[eq_end]!='\0')
        eq_end++;
    
    cur->ttr->raw = get_dyn_str(eq_end - i + 1);
     if (cur->ttl->raw != NULL) {
        for (int j = 0; i <= eq_end - i; j++) {
            cur->ttr->raw[j] = cur->raw[i+j];
        }
    }
    
}

void DivideExpression(exprn_t *cur) {
    
    // определяем следующую операцию в cur->raw
    define_operation(cur);
    if (cur->oper.name[0] != '\0') {
        // выделяем динамическую память на новые элементы 
        // для подвыражений текущего cur->raw
        cur->ttl = (exprn_t*)malloc( 1 * sizeof(exprn_t));
        cur->ttr = (exprn_t*)malloc( 1 * sizeof(exprn_t));

        // и если выделилось, то заполняем поля
        // cur->ttl->raw, cur-ttr->raw
        if (cur->ttl != NULL && cur->ttr != NULL) {
           set_subexpressions(cur);
        }
    } else {
        //
        // случай, когда операцию найти не получилось
        // и для текущего узла была инициализирована
        // тождественным оператором
        
        // здесь никаких действий не требуется
        // не на что разделять конструкцию
    }
    return;
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
