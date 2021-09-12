#ifndef SRC_CUSTOM_STRING_H_
#define SRC_CUSTOM_STRING_H_

// структура для удобной работы строками
// (памятью выделенными в кучу)
struct dynamic_custom_string {
    char *s;  // string: хранит указатель на первую букву
    int cap;  // capacity: хранит выделенной памяти на строку
    int l;  // length: хранит кол-во позиций от 0 до нуль-терминатора
};  

// объявляя экземпляр типа dynamic_custom_string
// мы хотим обращаться к нему короче: dcstr_t
// инструкция описанная ниже для нас это сделает
typedef struct dynamic_custom_string dcstr_t;

// ф-ция выделяющая память для строки
void Construct_by_def(dcstr_t *str);

// ф-ция инициализирующая строку в str, если она была выделена
void Initialise(dcstr_t *str, const char *src);

// ф-ция, которая делает вызывает Construct_by_def и Initialise
void Construct(dcstr_t *str, const char *src);

// ф-ция для вывода чистой кастомной строки
// до нуль-терминатора
void output(dcstr_t *str);

// ф-ция, которая сбрасывает кастомную строку
// в мёртвое состояние. Пользоваться с осторожностью,
// и вызывать только в случаях, если память из str->s освобождена,
// или же не была выделена.
void Invalidate(dcstr_t *str);

// ф-ция освобождающая память в кастомной строке
void Destruct(dcstr_t *str);

// ======= вспомогательный сахар ==========

// custom_strlen - моя ф-ция вывода длины строки
// возвращает индекс с нуль-терминатором
int cstrlen(char const *str);

// ф-ция, которая поместит в plant часть src
// с позиции fh(from_here) до позиции th(to_here)
// (в ней есть дин. выделение на char* для поля внутри plant)
void set_dcstr_fract(
    dcstr_t *plant, dcstr_t const *src,
    int fh, int th);

// ф-ция, которая сбрасывает cap до l = length
void cshrink_to_fit(dcstr_t* str);

// ф-ция для debug_целей, которая выводит
// состояние кастомной строки
void debug_str_output(dcstr_t *str);

#endif  // SRC_CUSTOM_STRING_H_
