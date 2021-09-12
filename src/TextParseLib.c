#include <stdio.h>  // printf
#include <stdlib.h>  // здесь лежит malloc

// здесь лежат объявления функций, которые
// будут определены ниже
#include "../includes/TextParseLib.h"

// здесь находятся операции, относительно которых
// будет разбиваться выражение
#include "../includes/operations.h" 

// объявляем в глобальном поле константный массив
// из указателей на функции с сигнатурами а
// (double x double) -> double
//
// Модификатор static в данном случае означает, что
// область видимости объекта oper_list не покидает границ
// текущего си-файла. Мы не можем обратиться к oper_list 
// в си-файле с точкой входа int main
static double (*oper_list[])(double, double) = {
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

void ConstructExprn(exprn_t *knot) {
    Construct_by_def(&knot->raw);
    Construct_by_def(&knot->oper.name);
    knot->stack[0] = 0.0;
    knot->stack[1] = 0.0;
    knot->is_empty = 0; // память выделена.
    knot->is_negative = 0;
    knot->ttl = knot->ttr = NULL;
    return;
}

void DestructExprn(exprn_t *knot) {
    if (knot->ttl != NULL) {
        printf("\n we came here1");
        free(knot->ttl);
        knot->ttl = NULL;
    }
    if (knot->ttr != NULL) {
        printf("\n we came here2");
        free(knot->ttr);
        knot->ttr = NULL;
    }
    if (!knot->is_empty) {
        printf("\n we came here3");
        Destruct(&knot->raw);
        Destruct(&knot->oper.name);
        knot->is_empty = 1;
    }
    return;
}

void debug_output(exprn_t *knot) {
    printf("\n\n\n//======================================\n");
    printf("\n knot address = %p\n", knot);
    printf("\n current raw expression: \n");
    debug_str_output(&knot->raw);
    printf("\n current operation: \n");
    debug_str_output(&knot->oper.name);
    printf("\n left sub_expression address = %p\n", knot->ttl);
    printf("\n right sub_expression address = %p\n", knot->ttr);
    printf("\n is_empty flag state = %d\n", knot->is_empty);
    printf("\n is_negative flag state = %d\n", knot->is_negative);
    printf("\n current stack = { %lf, %lf }\n", knot->stack[0], knot->stack[1]);
    printf("\n//--------------------------------------\n\n\n");
    fflush(stdout);
    return;
}

char* get_init_exprn() {
    FILE *instream = NULL;
    char *buffer = NULL;
    int len = 10;
    instream = fopen(expr_name, "rt");
    if (instream != NULL) {
        buffer = (char*)malloc(len * sizeof(char));
        if (NULL != buffer) {
            //  если память выделилась конструктором
            int i = 0;
            buffer[i] = '\0';
            while (1) {
                while (i < len - 1 && !feof(instream)) {
                    buffer[i+1] = '\0';
                    fscanf(instream, "%c", &buffer[i]);
                    i++;
                }
                if (i == len - 1 && !feof(instream)) {
                    int tmp = len * 1.5;
                    char *buf = (char*)realloc(buffer, tmp * sizeof(char));
                    if (NULL == buf) {
                        printf("\n failed to reallocate memory for the expression");
                        printf("\n\t the string is to be cleared\n");
                        free(buffer);
                        buffer = NULL;
                        len = -1;
                        break;
                    } else {
                        buffer = buf;
                        len = tmp;
                    }
                } else {
                    break;
                }
            }
        } else {
            printf("\n failed to construct string");
        }
    } else {
        printf("\n failed to open stream for input");
    }
    if (instream != NULL){
        fclose(instream);
    }
    return buffer;
}

void SetInitialExpression(exprn_t *root) {
    char *b_str = NULL;
    b_str = get_init_exprn();
    if (NULL != b_str) {
        Initialise(&root->raw, b_str);
    } else {
        printf("\n failed to initialise init_expr \n");
    }
    free(b_str);
    b_str = NULL;
    return;
}

int get_find_operation(dcstr_t *eq, char const *oper) {
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
    // Construct(&cur->oper.name, oper_names[i]);
    Initialise(&cur->oper.name, oper_names[i]);

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
    //
    // определяем строку в левом подвыражении
    set_dcstr_fract(&cur->ttl->raw, &cur->raw,
                     0, cur->oper.pos);
    //
    // определяем строку в правом подвыражении
    set_dcstr_fract(&cur->ttr->raw, &cur->raw,
                     cur->oper.pos + cur->oper.name.l, cur->raw.l);

    // - нужны ли здесь какие-нибудь проверки?
    // -- пока не придумал.
    return;
}
void divide_expression(exprn_t *cur) {
    
    // определяем наименее приоритетную операцию в cur->raw
    define_operation(cur);

    if (cur->oper.name.s[0] != '\0') {
        // выделяем динамическую память на новые элементы 
        // для подвыражений текущего cur->raw
        cur->ttl = (exprn_t*)malloc( 1 * sizeof(exprn_t));
        cur->ttr = (exprn_t*)malloc( 1 * sizeof(exprn_t));

        // и если выделилось, то заполняем поля
        // cur->ttl->raw, cur-ttr->raw
        if (cur->ttl != NULL && cur->ttr != NULL) {
            ConstructExprn(cur->ttl);
            ConstructExprn(cur->ttr);
            set_subexpressions(cur);
        }
    } else {
        //
        // случай, когда операцию в cur->raw найти не получилось
        // и для текущего узла cur->oper.action был инициализирован
        // тождественным оператором
        cur->ttl = NULL;
        cur->ttr = NULL;
        // память выделять не нужно, 
    }
    return;
}
