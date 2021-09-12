#include <stdio.h>
#include <stdlib.h>

void debug_output(char *str, int len);
void Destruct(char *cur);

int main() {
    int len = 3;
    int tmp;
    char *str;
    char *buf;
    str = (char *)malloc(len * sizeof(char));
    if (str != NULL) {
        char input = 10;
        int i = 0;
        while (1) {
            printf("\n str :");
            debug_output(str, len);
            while (i < len) {
                fflush(stdout);
                scanf("%c", &input);
                if (input == '0')
                {
                    str[i] = '\0';
                    break;
                } else if (input == '\n') {
                    str[i] = ' ';
                } else {
                    str[i] = input;
                }
                i++;
            }
            if (i == len && input != '0') {
                tmp = len * 1.5;
                buf = realloc(str, tmp * sizeof(char));
                if (NULL == buf) {
                    printf("\n failed to reallocate memory for the string \n");
                    printf("\n str :");
                    debug_output(str, len);
                } else {
                    printf("\n before realloc str: ");
                    debug_output(str, len);
                    printf("\n realloc-ed buf: ");
                    debug_output(buf, tmp);
                    len = tmp;
                    str = buf;
                    printf("\n realloc-ed str: ");
                    debug_output(str, len);
                }
            } else {
                break;
            }
        }
    } else {
        printf("\n initial malloc didn't work");
    }
    printf("\n final str: ");
    debug_output(str, len);
    printf("\n final buf: ");
    debug_output(buf, tmp);

    Destruct(str);
    Destruct(buf);
    return 0;
}

void debug_output(char *str, int len){
    printf("%p : ", str);
    for (int i = 0; i < len; i++) {
        if (str[i] == '\0') {
            printf("\\o");
        } else {
            printf("%c", str[i]);
        }
    }
    printf("\n");
    fflush(stdout);
    return;
}

void Destruct(char *cur){
    if (cur != NULL) {
        free(cur);
        printf("\n %p - adress is processed. Memory was freed", cur);
    }
    return;
}
