#include <math.h>

typedef enum {
    CalculatorFunctionTypeOneInput, // Real functions like sqrt, sinx, squared, etc
    CalculatorFunctionTypeTwoInput, // Operations multiplying and dividing
    CalculatorFunctionTypePassive, // Add and subtract
} CalculatorFunctionType;

typedef double(CalculatorCalculateFunctionOneInput)(double a);
typedef double(CalculatorCalculateFunctionTwoInput)(double a, double b);
// typedef double(CalculatorFunctionCalculate)

// typedef union {
// double one_param_func(double a);
// double two_param_func(double a, double b);
// } CalculatorFunctionUnion;

typedef struct {
    CalculatorFunctionType type;
    CalculatorCalculateFunctionOneInput* one_param_func;
    CalculatorCalculateFunctionTwoInput* two_param_func;
} CalculatorFunction;

double add(double a) {
    return a;
}

double subtract(double a) {
    return -a;
}

double multiply(double a, double b) {
    return a * b;
}

double divide(double a, double b) {
    return a / b;
}

double square(double a) {
    return pow(a, 2);
}

const CalculatorFunction CalculatorFunctionAdd = {
    .type = CalculatorFunctionTypeOneInput,
    .one_param_func = add};

const CalculatorFunction CalculatorFunctionSubtract = {
    .type = CalculatorFunctionTypeOneInput,
    .one_param_func = subtract};

const CalculatorFunction CalculatorFunctionMultiply = {
    .type = CalculatorFunctionTypeTwoInput,
    .two_param_func = multiply};

const CalculatorFunction CalculatorFunctionDivide = {
    .type = CalculatorFunctionTypeTwoInput,
    .two_param_func = divide};

const CalculatorFunction CalculatorFunctionSquare = {
    .type = CalculatorFunctionTypeOneInput,
    .one_param_func = square};