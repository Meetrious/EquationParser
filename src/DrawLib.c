// все описания функций и зачем они нужны лежит в хедере
#include <stdio.h>
#include "DrawLib.h"

// левый отступ от края терминала
const int INDT = 5;  // +1 = левая граница, +1 = начало ареала

// верхний отступ от края терминала
const int UP_INDT = 3;  // +1 = верхняя граница, +1 = начало ареала

// ширина поля для отрисовки графика
const int WID = 80;

// высота поля для отрисовки графика
const int HEI = 25;

// символ отрисовки прямоугольной границы
const char BORDER_SYMB = '#';

void PutCursorAway() {
    // переносим курсор для корректной отрисовки за поле действий
    printf("\x1b[%d;%dH", UP_INDT + HEI + 3, 0);
    return;
}

void DrawSymb(int line, int pos, char Symb) {
    if (pos >= 0 && line >= 0) {
        printf("\x1b[%d;%dH%c", line, pos, Symb);
    } else {
        printf("(%d = pos, %d = line) coordinates are unvaliable \n", pos, line);
        printf("try something positive, for Christ's sake! \n");
        printf("unable to proceed: ..");
        getchar();
        return;
    }
    PutCursorAway();
    return;
}

void EraseSymb(int line, int pos) {
    DrawSymb(line, pos, ' ');
}

void EasyDrawMargins() {
    // перевели курсор в исходное положение
    printf("\x1b[0;0H");
    // стёрли всё к чертям
    printf("\033[2J");

    // отрисовывае поля
    for (int i = UP_INDT + 1; i < UP_INDT + HEI + 3; i++) {
        // на текущей строке рисуем символ из left
        DrawSymb(i, INDT + 1 , BORDER_SYMB);
        DrawSymb(i, WID + INDT + 1, BORDER_SYMB);
    }
    // верхняя граница
    for (int i = INDT + 2; i <= INDT + WID; i++) {
        DrawSymb(UP_INDT + 1, i, BORDER_SYMB);
    }
    // нижняя граница
    for (int i = INDT + 2; i <= INDT + WID; i++) {
        DrawSymb(UP_INDT + HEI + 2 , i, BORDER_SYMB);
    }

    // дополнительный сдвиг каретки для корректного отрисовывания
    printf("\x1b[33;0H");
    return;
}
