#include "../includes/operations.h"
#include <math.h>

// ======= ниже определения операций в унифицированной сигнатуре ===========
// ================ (double, double) --> double ============================

double get_id(double a, double b) {
    (void)b;
    return a;
}

double get_addition(double a, double b) {
    return a + b;
}

double get_product(double a, double b) {
    return a * b;
}

double get_fraction(double a, double b) {
    return a/b;
}

double get_sin(double a, double b) {
    (void)b;
    return sin(a);
}

double get_cos(double a, double b) {
    (void)b;
    return cos(a);
}

double get_tg(double a, double b) {
    (void)b;
    return tan(a);
}

double get_ctg(double a, double b) {
    (void)b;
    return 1.0/tan(a);
}

double get_ln(double a, double b) {
    (void)b;
    return log(a);
}

double get_log10(double a, double b) {
    (void)b;
    return log10(a);
}

double get_sqrt(double a, double b) {
    (void)b;
    return sqrt(a);
}

double get_pow(double a, double b) {
    return pow(a,b);
}
