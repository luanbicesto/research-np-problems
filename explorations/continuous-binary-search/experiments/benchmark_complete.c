/**
 * benchmark_complete.c - Teste do algoritmo completo (seleção de ramo + CBS+ITP)
 *
 * Testa com funções NÃO-MONÓTONAS onde a seleção de ramo é necessária.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "cbs_complete.h"
#include "test_functions.h"

#define EPS 1e-10

/* --- Funções não-monótonas adicionais --- */

/* Gaussiana completa (unimodal, não-monótona) */
double fn_gaussian_full(double x, void *params)
{
    gaussian_params_t *p = (gaussian_params_t *)params;
    double z = (x - p->mu) / p->sigma;
    return exp(-0.5 * z * z);
}

/* Parábola invertida: f(x) = -(x-c)^2 + h */
typedef struct { double c; double h; } parabola_params_t;
double fn_parabola(double x, void *params)
{
    parabola_params_t *p = (parabola_params_t *)params;
    return -(x - p->c) * (x - p->c) + p->h;
}

/* Seno (múltiplas oscilações) */
double fn_sin(double x, void *params)
{
    (void)params;
    return sin(x);
}

/* Função com múltiplos picos: sin(x) * exp(-x^2/20) */
double fn_multi_peak(double x, void *params)
{
    (void)params;
    return sin(3.0 * x) * exp(-x * x / 20.0);
}

/* Cúbica não-monótona: x^3 - 3x (extremos em x=-1 e x=1) */
double fn_cubic_nonmono(double x, void *params)
{
    (void)params;
    return x * x * x - 3.0 * x;
}

/* --- Estrutura de teste --- */
typedef struct {
    const char *name;
    func_t f;
    void *params;
    double T;
    double a, b;
    double x_exact;  /* NAN se desconhecido */
} test_case_t;

static void run_test(test_case_t *tc)
{
    printf("  %-40s T=%8.4f  [%6.2f, %6.2f]\n", tc->name, tc->T, tc->a, tc->b);

    cbs_complete_result_t cr = cbs_complete_search(
        tc->f, tc->params, tc->T,
        tc->a, tc->b, EPS
    );

    if (!cr.search_result.converged) {
        printf("    FALHOU: não convergiu (branch_evals=%d)\n\n", cr.branch_evals);
        return;
    }

    double err_exact = isnan(tc->x_exact) ? -1.0 : fabs(cr.search_result.x_star - tc->x_exact);

    printf("    Resultado: x*=%.12f\n", cr.search_result.x_star);
    printf("    f(x*)=%.12f  (T=%.12f)  err=%.2e\n",
           cr.search_result.f_x_star, tc->T, cr.search_result.error);
    printf("    Ramo: [%.6f, %.6f] dir=%s\n",
           cr.branch_a, cr.branch_b,
           cr.branch_dir == MONOTONE_INCREASING ? "crescente" : "decrescente");
    printf("    Custo: branch_evals=%d  search_evals=%d  total=%d  iters=%d\n",
           cr.branch_evals, cr.search_result.f_evals,
           cr.total_evals, cr.search_result.iterations);
    if (err_exact >= 0) printf("    |x*-exact|=%.2e\n", err_exact);

    /* Verificar que f(x*) ≈ T */
    double verification = fabs(cr.search_result.f_x_star - tc->T);
    printf("    Verificação: |f(x*)-T| = %.2e  %s\n",
           verification, verification < 1e-6 ? "✓ OK" : "✗ ERRO");
    printf("\n");
}

int main(void)
{
    printf("================================================================\n");
    printf("  ALGORITMO COMPLETO: Seleção de Ramo + CBS+ITP\n");
    printf("  Teste com funções NÃO-MONÓTONAS\n");
    printf("  Precisão: eps = %.0e\n", EPS);
    printf("================================================================\n\n");

    /* --- Teste 1: Gaussiana completa (unimodal) --- */
    /* f(x) = exp(-x²/2), max em x=0, buscar T=0.5 em [-5, 5] */
    /* Duas soluções: x ≈ ±1.1774 — aceitar qualquer uma */
    gaussian_params_t g1 = {.mu = 0.0, .sigma = 1.0};
    test_case_t t1 = {
        .name = "Gaussian(0,1) completa",
        .f = fn_gaussian_full, .params = &g1,
        .T = 0.5, .a = -5.0, .b = 5.0,
        .x_exact = NAN  /* ambas ±1.1774 são válidas */
    };

    /* --- Teste 2: Gaussiana, T alto (perto do pico) --- */
    test_case_t t2 = {
        .name = "Gaussian(0,1) completa T=0.9",
        .f = fn_gaussian_full, .params = &g1,
        .T = 0.9, .a = -5.0, .b = 5.0,
        .x_exact = NAN
    };

    /* --- Teste 3: Gaussiana, T baixo (longe do pico) --- */
    test_case_t t3 = {
        .name = "Gaussian(0,1) completa T=0.1",
        .f = fn_gaussian_full, .params = &g1,
        .T = 0.1, .a = -5.0, .b = 5.0,
        .x_exact = NAN
    };

    /* --- Teste 4: Parábola invertida --- */
    /* f(x) = -(x-3)^2 + 5, max em x=3 (f(3)=5) */
    /* Buscar T=3: soluções x=3±√2 ≈ 1.586, 4.414 */
    parabola_params_t p1 = {.c = 3.0, .h = 5.0};
    test_case_t t4 = {
        .name = "Parabola -(x-3)^2+5, T=3",
        .f = fn_parabola, .params = &p1,
        .T = 3.0, .a = -2.0, .b = 8.0,
        .x_exact = NAN  /* 3-√2 ou 3+√2 */
    };

    /* --- Teste 5: Seno (múltiplas soluções) --- */
    /* f(x) = sin(x), T=0.5 em [0, 2π] → soluções π/6 e 5π/6 */
    test_case_t t5 = {
        .name = "sin(x) T=0.5, [0, 2pi]",
        .f = fn_sin, .params = NULL,
        .T = 0.5, .a = 0.0, .b = 2.0 * M_PI,
        .x_exact = NAN  /* π/6 ≈ 0.5236 ou 5π/6 ≈ 2.618 */
    };

    /* --- Teste 6: Seno, T negativo --- */
    /* f(x) = sin(x), T=-0.7 em [0, 2π] → soluções em (π, 2π) */
    test_case_t t6 = {
        .name = "sin(x) T=-0.7, [0, 2pi]",
        .f = fn_sin, .params = NULL,
        .T = -0.7, .a = 0.0, .b = 2.0 * M_PI,
        .x_exact = NAN
    };

    /* --- Teste 7: Multi-pico --- */
    /* f(x) = sin(3x)*exp(-x²/20), oscila com amplitude decrescente */
    test_case_t t7 = {
        .name = "sin(3x)*exp(-x^2/20) T=0.3",
        .f = fn_multi_peak, .params = NULL,
        .T = 0.3, .a = -5.0, .b = 5.0,
        .x_exact = NAN
    };

    /* --- Teste 8: Cúbica não-monótona --- */
    /* f(x) = x³-3x, extremos em x=±1. f(-1)=2, f(1)=-2 */
    /* Buscar T=0 em [-3, 3]: soluções x=0, ±√3 */
    test_case_t t8 = {
        .name = "x^3-3x T=0, [-3,3]",
        .f = fn_cubic_nonmono, .params = NULL,
        .T = 0.0, .a = -3.0, .b = 3.0,
        .x_exact = NAN  /* 0, ±√3 */
    };

    /* --- Teste 9: Cúbica não-monótona, T=1 --- */
    /* f(x) = x³-3x, T=1 → soluções perto de x≈-1.53, x≈-0.35, x≈1.88 */
    test_case_t t9 = {
        .name = "x^3-3x T=1, [-3,3]",
        .f = fn_cubic_nonmono, .params = NULL,
        .T = 1.0, .a = -3.0, .b = 3.0,
        .x_exact = NAN
    };

    /* --- Teste 10: Gaussiana deslocada --- */
    /* f com pico longe do centro do intervalo */
    gaussian_params_t g2 = {.mu = 4.5, .sigma = 0.5};
    test_case_t t10 = {
        .name = "Gaussian(4.5,0.5) T=0.6 [0,10]",
        .f = fn_gaussian_full, .params = &g2,
        .T = 0.6, .a = 0.0, .b = 10.0,
        .x_exact = NAN
    };

    /* --- Teste 11: Intervalo grande [-500,500], cúbica --- */
    /* f(x) = x³ - 3x, T = 1000000 (1 milhão) */
    /* Solução: x³ ≈ 1e6 → x ≈ 100 */
    test_case_t t11 = {
        .name = "x^3-3x T=1000000 [-500,500]",
        .f = fn_cubic_nonmono, .params = NULL,
        .T = 1000000.0, .a = -500.0, .b = 500.0,
        .x_exact = NAN
    };

    /* --- Teste 12: Intervalo grande, T = 5 milhões --- */
    /* f(x) = x³ - 3x, T = 5000000 */
    /* Solução: x ≈ cbrt(5e6) ≈ 170.99 */
    test_case_t t12 = {
        .name = "x^3-3x T=5000000 [-500,500]",
        .f = fn_cubic_nonmono, .params = NULL,
        .T = 5000000.0, .a = -500.0, .b = 500.0,
        .x_exact = NAN
    };

    /* --- Teste 13: Intervalo grande, T negativo grande --- */
    /* f(x) = x³ - 3x, T = -2000000 */
    test_case_t t13 = {
        .name = "x^3-3x T=-2000000 [-500,500]",
        .f = fn_cubic_nonmono, .params = NULL,
        .T = -2000000.0, .a = -500.0, .b = 500.0,
        .x_exact = NAN
    };

    /* --- Teste 14: Polinômio de grau alto, intervalo grande --- */
    /* f(x) = x^5 / 1000 (monótona, valores enormes) */
    /* T = 3125000 → x^5 = 3.125e9 → x ≈ 500*... não, x^5/1000=3125000 → x^5=3.125e9 → x≈315.9 */
    /* Usar cúbica que é mais manejável */

    /* --- Teste 14: Parábola em intervalo grande --- */
    /* f(x) = -(x-100)^2 + 2000000, max em x=100 (f(100)=2e6) */
    /* T = 1500000 → -(x-100)^2 = -500000 → x = 100 ± √500000 ≈ 100 ± 707 */
    /* Em [-500,500]: solução esquerda = 100 - 707 = -607 (fora!), direita = 100+707=807 (fora!) */
    /* Ajustar: T = 1999000 → -(x-100)^2 = -1000 → x = 100 ± √1000 ≈ 100 ± 31.6 */
    parabola_params_t p2 = {.c = 100.0, .h = 2000000.0};
    test_case_t t14 = {
        .name = "-(x-100)^2+2e6 T=1999000 [-500,500]",
        .f = fn_parabola, .params = &p2,
        .T = 1999000.0, .a = -500.0, .b = 500.0,
        .x_exact = NAN  /* 100 - √1000 ≈ 68.38 ou 100 + √1000 ≈ 131.62 */
    };

    /* --- Teste 15: Parábola, T baixo (solução longe do pico) --- */
    /* T = 1000000 → -(x-100)^2 = -1000000 → x = 100 ± 1000 */
    /* Em [-500,500]: solução esquerda = -900 (fora), direita = 1100 (fora) */
    /* T = 1750000 → -(x-100)^2 = -250000 → x = 100 ± 500 → x=-400 ou x=600 */
    /* x=-400 está em [-500, 500]! */
    test_case_t t15 = {
        .name = "-(x-100)^2+2e6 T=1750000 [-500,500]",
        .f = fn_parabola, .params = &p2,
        .T = 1750000.0, .a = -500.0, .b = 500.0,
        .x_exact = NAN  /* 100 - 500 = -400 ou 100 + 500 = 600 (fora) → x=-400 */
    };

    /* --- Teste 16: Gaussiana escalada (amplitude > 1 milhão) --- */
    /* f(x) = 2000000 * exp(-x²/200), max em x=0 (f(0)=2e6) */
    /* T = 1000000 → exp(-x²/200) = 0.5 → x² = 200*ln(2) → x ≈ ±11.77 */
    narrow_gaussian_params_t g_big = {.mu = 0.0, .sigma = 10.0, .amplitude = 2000000.0};
    test_case_t t16 = {
        .name = "2e6*Gauss(0,10) T=1e6 [-500,500]",
        .f = fn_narrow_gaussian, .params = &g_big,
        .T = 1000000.0, .a = -500.0, .b = 500.0,
        .x_exact = NAN  /* ±10*√(2*ln2) ≈ ±11.77 */
    };

    /* --- Teste 17: Gaussiana escalada, T alto (perto do pico) --- */
    /* T = 1900000 → exp(-x²/200) = 0.95 → x² = 200*ln(1/0.95) → x ≈ ±3.2 */
    test_case_t t17 = {
        .name = "2e6*Gauss(0,10) T=1.9e6 [-500,500]",
        .f = fn_narrow_gaussian, .params = &g_big,
        .T = 1900000.0, .a = -500.0, .b = 500.0,
        .x_exact = NAN
    };

    /* --- Teste 18: sin em intervalo grande --- */
    /* f(x) = sin(x), T=0.5 em [-500, 500] — centenas de soluções */
    test_case_t t18 = {
        .name = "sin(x) T=0.5 [-500,500]",
        .f = fn_sin, .params = NULL,
        .T = 0.5, .a = -500.0, .b = 500.0,
        .x_exact = NAN
    };

    test_case_t tests[] = {t1, t2, t3, t4, t5, t6, t7, t8, t9, t10,
                           t11, t12, t13, t14, t15, t16, t17, t18};
    int n_tests = sizeof(tests) / sizeof(tests[0]);

    printf("Executando %d testes com funções não-monótonas...\n\n", n_tests);

    int passed = 0;
    for (int i = 0; i < n_tests; i++) {
        printf("[Teste %d/%d]\n", i + 1, n_tests);
        run_test(&tests[i]);

        /* Verificar resultado */
        cbs_complete_result_t cr = cbs_complete_search(
            tests[i].f, tests[i].params, tests[i].T,
            tests[i].a, tests[i].b, EPS
        );
        if (cr.search_result.converged &&
            fabs(cr.search_result.f_x_star - tests[i].T) < 1e-6) {
            passed++;
        }
    }

    printf("================================================================\n");
    printf("  Resultado: %d/%d testes passaram\n", passed, n_tests);
    printf("================================================================\n");

    return (passed == n_tests) ? 0 : 1;
}
