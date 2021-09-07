#ifndef SRC_CUSTOM_STRING_H_
#define SRC_CUSTOM_STRING_H_

// структура для удобной работы строками
// (памятью выделенными в кучу)
struct dynamic_custom_string {
    char *s;  // string: хранит указатель на первую букву
    int cap;  // capacity: хранит выделенной памяти на строку
    int l  // length: хранит размер строки до gthdjuj нуль-терминатора
};  

// объявляя экземпляр типа dynamic_custom_string
// мы хотим обращаться к нему короче: dcstr_t
typedef struct dynamic_custom_string dcstr_t;

// ф-ция выделяющая память для строки
void Construct(dcstr_t *str);

// ф-ция инициализирующая строку в str, если она была выделена
void Initialise(dcstr_t *str, const char *src);

// ф-ция, котороя делает вызывает Construct и Initialise
void Construct(dcstr_t *str, const char *src);

// ф-ция освобождающая память строки
void Destruct(dcstr_t *str);

// служебная функция для считывания 0 или 1
_Bool ans_is_yes();

// ф-ция проверяющая на _Bool
_Bool is_bool(_Bool *bl);

#endif  // SRC_CUSTOM_STRING_H_