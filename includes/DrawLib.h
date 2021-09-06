#ifndef SRC_DRAWLIB_H_
#define SRC_DRAWLIB_H_

// сброс курсора без потери картинки
void PutCursorAway();

// ф-ция, которая рисует на посланной позиции терминала символ
void DrawSymb(int line, int pos, char Symb);

// ф-ция, которая рисует на посланной позиции пробел
void EraseSymb(int line, int pos);

// ф-ция, которая рисует границы поля
void EasyDrawMargins();

#endif  // SRC_DRAWLIB_H_
