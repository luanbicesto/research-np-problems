/**
 * test_functions.h - Funções de teste para a busca binária contínua
 */

#ifndef TEST_FUNCTIONS_H
#define TEST_FUNCTIONS_H

/* Parâmetros da Gaussiana */
typedef struct {
    double mu;
    double sigma;
} gaussian_params_t;

/* Parâmetros de polinômio a*x^3 + b*x^2 + c*x + d */
typedef struct {
    double a, b, c, d;
} cubic_params_t;

/* Parâmetros de Gaussiana estreita */
typedef struct {
    double mu;
    double sigma;
    double amplitude;
} narrow_gaussian_params_t;

/* Funções de teste */
double fn_gaussian(double x, void *params);
double fn_cubic(double x, void *params);
double fn_exp_decay(double x, void *params);       /* x^3 * exp(-x) */
double fn_sigmoid(double x, void *params);          /* 1/(1+exp(-k*(x-x0))) */
double fn_sin_oscillatory(double x, void *params);  /* sin(ωx) */
double fn_narrow_gaussian(double x, void *params);  /* Gaussiana muito estreita */

#endif /* TEST_FUNCTIONS_H */
