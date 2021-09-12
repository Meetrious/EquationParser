// здесь тестируем библиотеку со строками
#include <stdio.h>
#include "../../includes/Custom_string.h"

int main() {
    dcstr_t test;
    Construct(&test, "azaza");
    //output(&test);
    debug_str_output(&test);
    printf("\n \t %d \n",cstrlen(test.s));
    Destruct(&test);
    debug_str_output(&test);
    printf("\n \t %d \n",cstrlen(test.s));
    return 0;
}
