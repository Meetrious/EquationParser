#ifndef SRC_TEXTPARSELIB_H_
#define SRC_TEXTPARSELIB_H_

#include "../includes/Custom_string.h"

struct operation {
    // указатель на функцию, которая реализует операцию
    dcstr_t name;  // имя операции
    double (*action)(double, double);  // указатель на операцию
    int pos;  // позиция первой буквы в raw;
};

// говорим программе, что к структуре operation_t мы будем
// краткости ради обращаться по идентификатору "exprn"
typedef struct operation oper_t;

//
// объявляем пользовательский тип expression_t
// тобишь "тип - выражение"
struct expression {
    //
    // указатель на правое подвыражение относительно текущей операции
    struct expression *ttl;  // to the left
    //
    // указатель на левое подвыражение отностельно текущей операции 
    struct expression *ttr;  // to the right
    //
    // сырая строка с выражением, требующим вычисление
    dcstr_t raw;  // raw expression
    //
    // пара аргументов для помещения в operation
    double stack[2];
    //
    // результат операций текущего выражения
    double result;
    //
    // операция над текущим стеком
    oper_t oper;
    //
    // флаг пустого выражения
    _Bool is_empty;
    //
    // флаг отрицательного значения
    _Bool is_negative;
};

// говорим программе, что к структуре expression мы будем
// краткости ради обращаться по идентификатору "exprn_t"
typedef struct expression exprn_t;

// ф-ция определяет raw и состояние is_empty
void ConstructExprn(exprn_t *knot);

// ф-ция-деструктор, которая освобождает память
// по адресам в полях текущего узла
void DestructExprn(exprn_t *knot);

// ф-ция, которая определяет следующую операцию,
// которая будет разбивать выражение,
// объявленное в cur->raw строке.
// Целиком инициализирует объект cur->oper.
void define_operation (exprn_t *cur);

// ф-ция, которая убирает обрамляющие скобки
// c текущего состояния выражения
void filter_brackets(char *expr);

// ф-ция, которая доходит до первой операции в cur->raw
// и создаёт два экземпляра в куче типа exprn
// инициализируя raw-строки в них и определяет is_empty
// флаг в зависимости от кол-ва аргументов и операции в cur
// и если находит операцию, то возвращает 1.
// Если не находит операцию, то возвращает 0,
// что сигнализирует о том, что перед нами осталась переменная
void divide_expression(exprn_t *cur);

// ф-ция инициализации исходного выражение
// по содержимому внешнего файла
void SetInitialExpression(exprn_t *root);

char* get_init_exprn();

// ф-ция инициализации исходного выражения
// по вводу с клавиатуры
void WriteInitialExpression(dcstr_t *str);

// ф-ция передаёт указатель на позицию в строке eq,
// с которой начинается данная операция в строке oper
int get_find_operation(dcstr_t *eq, char const *oper);

// ф-ция вывода информации о
// текущем узле разбиения
// математического выражения
void debug_output(exprn_t *knot);

#endif  // SRC_TEXTPARSELIB_H_
