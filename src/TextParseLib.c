#include <stdio.h>  // printf
#include <stdlib.h>  // здесь лежит malloc

// здесь лежат объявления функций, которые
// будут определены ниже
#include "TextParseLib.h"

// здесь находятся операции, относительно которых
// будет разбиваться выражение
#include "operations.h" 

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
static const char *operations_names[] = {
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


int get_find_operation(dcstr_t *eq, dcstr_t *oper) {
    int res;
    int oper_len = 0;
    int eq_len = 0;
    while(oper[oper_len]!= '\0')
        oper_len++;
    while(eq[eq_len]!= '\0')
        oper_len++;

    int i;
    for (i = 0; i < eq_len - oper_len; i++) {
        if (eq[i] == '(') {
            while (eq[i]!= ')')
                i++;
        }
        if (i < eq_len - oper_len){
            for (int j = 0; j < oper_len; j++) {
                if (eq[i+j] == oper[j] ) {
                    continue;
                } else {
                    break;
                }
            }
        }
    }
    if (i >= eq_len - oper_len) {
        res = &eq[eq_len];  // указываем на терминатор
    } else {
        res = &eq[i];  // указываем на позицию, с которой начинается операция
    }
    return res;
}

// ф-ция, которая определяет следующую операцию,
// которая будет разбивать выражение,
// объявленное в cur->raw строке.
// Целиком инициализирует объект cur->oper.
void define_operation (exprn_t *cur) {
    int i;
    for (i = 0; *operations_names[i] != '\0'; i++) {
        cur->oper.pos = get_find_operation(cur->raw, operations_names[i]);
        if (cur->oper.pos != '\0') {
            break;
        }
    }
    int oper_len = 0;
    while (operations_names[i][oper_len]!='\0')
        oper_len++;

    // назначаем узлу cur определённую выше операцию
    for (int j = 0; operations_names[i][j] != '\0'; j++) {
        cur->oper.name[j] = operations_names[i][j];
    }
    // назначаем фактическую get_операцию для текущего узла
    cur->oper.action = oper_list[i];

    return;
}

void apply_brackets_filter(exprn_t *cur) {
    if (cur->raw[0] == '(') {
        int i = 0;
        while (cur->raw[i] != ')') {
            i++;
        }
        if (cur->raw[i+1] == '\0') {
            for (int j = 0; cur->raw[j] != '\0'; j++) {
                cur->raw[j] = cur->raw[j+1];
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
