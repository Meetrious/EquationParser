#include "TextParseLib.h"
#include <math.h>

const char var = 'a';

char* CleanSpacer(char *str) {
int counter, i;
char *res;
counter = 0;
while (*str != '\0') {
    counter++, str++;
    }
for (i=0; i < counter; i++) str--;
res=malloc(counter*sizeof(char));
int j = 0;
for (int i = 0; str[i]; i++)
    if (str[i] != ' ') {
        res[j++] = str[i];
    }
    res[j] = '\0';
res=realloc(res, j*sizeof(char));
free(str);
str=res;
return str;
}