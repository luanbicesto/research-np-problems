/**
 * test_functions.c - Implementação das funções de teste
 */

#include "test_functions.h"
#include <math.h>

double fn_gaussian(double x, void *params)
{
    gaussian_params_t *p = (gaussian_params_t *)params;
    double z = (x - p->mu) / p->sigma;
    return exp(-0.5 * z * z);
}

double fn_cubic(double x, void *params)
{
    cubic_params_t *p = (cubic_params_t *)params;
    return p->a * x * x * x + p->b * x * x + p->c * x + p->d;
}

double fn_exp_decay(double x, void *params)
{
    (void)params;
    /* f(x) = x^3 * exp(-x) para x >= 0 */
    /* Máximo em x=3: f(3) = 27*exp(-3) ≈ 1.344 */
    return x * x * x * exp(-x);
}

double fn_sigmoid(double x, void *params)
{
    (void)params;
    /* Sigmoid padrão com k=1, x0=0 */
    return 1.0 / (1.0 + exp(-x));
}

double fn_sin_oscillatory(double x, void *params)
{
    (void)params;
    /* sin(20x) - altamente oscilatória */
    return sin(20.0 * x);
}

double fn_narrow_gaussian(double x, void *params)
{
    narrow_gaussian_params_t *p = (narrow_gaussian_params_t *)params;
    double z = (x - p->mu) / p->sigma;
    return p->amplitude * exp(-0.5 * z * z);
}
