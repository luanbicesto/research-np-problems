/**
 * benchmark.c - Programa principal de benchmarks
 *
 * Compara busca binária contínua com pivô por ponto médio vs mediana
 * para diferentes funções de teste.
 *
 * Saída: resultados em formato CSV para posterior visualização.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include "cbs_algorithm.h"
#include "test_functions.h"
#include "itp_method.h"
#include "cbs_itp.h"

#define EPS         1e-10
#define MAX_ITER    200

/* --- Estrutura para um caso de teste --- */
typedef struct {
    const char *name;
    func_t f;
    void *params;
    double T;           /* Alvo */
    double a, b;        /* Intervalo (já monótono) */
    double x_exact;     /* Solução exata (se conhecida), NAN se desconhecida */
} test_case_t;

/* --- Execução de um teste --- */
static void run_test(test_case_t *tc, FILE *csv_summary, FILE *csv_history)
{
    printf("  %-35s T=%.6f  [%.2f, %.2f]\n", tc->name, tc->T, tc->a, tc->b);

    /* Executar com ponto médio */
    cbs_history_t *hist_mid = cbs_history_create(MAX_ITER);
    cbs_result_t res_mid = cbs_search(
        tc->f, tc->params, tc->T,
        tc->a, tc->b, EPS, MAX_ITER,
        PIVOT_MIDPOINT, hist_mid
    );

    /* Executar com mediana (peso |f|) */
    cbs_history_t *hist_med = cbs_history_create(MAX_ITER);
    cbs_result_t res_med = cbs_search(
        tc->f, tc->params, tc->T,
        tc->a, tc->b, EPS, MAX_ITER,
        PIVOT_MEDIAN, hist_med
    );

    /* Executar com mediana inversa (peso 1/|f-T|) */
    cbs_history_t *hist_inv = cbs_history_create(MAX_ITER);
    cbs_result_t res_inv = cbs_search(
        tc->f, tc->params, tc->T,
        tc->a, tc->b, EPS, MAX_ITER,
        PIVOT_MEDIAN_INV, hist_inv
    );

    /* Executar com ITP (estado da arte) */
    cbs_history_t *hist_itp = cbs_history_create(MAX_ITER);
    cbs_result_t res_itp = itp_search(
        tc->f, tc->params, tc->T,
        tc->a, tc->b, EPS,
        0.1, 2.0, 1,  /* kappa1, kappa2, n0 padrão */
        hist_itp
    );

    /* Executar com CBS+ITP (híbrido) */
    cbs_history_t *hist_hybrid = cbs_history_create(MAX_ITER);
    cbs_itp_params_t hp = CBS_ITP_DEFAULT;
    cbs_result_t res_hybrid = cbs_itp_search(
        tc->f, tc->params, tc->T,
        tc->a, tc->b, EPS,
        &hp, hist_hybrid
    );

    /* Erro em relação à solução exata (se conhecida) */
    double err_exact_mid = isnan(tc->x_exact) ? -1.0 : fabs(res_mid.x_star - tc->x_exact);
    double err_exact_med = isnan(tc->x_exact) ? -1.0 : fabs(res_med.x_star - tc->x_exact);
    double err_exact_inv = isnan(tc->x_exact) ? -1.0 : fabs(res_inv.x_star - tc->x_exact);
    double err_exact_itp = isnan(tc->x_exact) ? -1.0 : fabs(res_itp.x_star - tc->x_exact);
    double err_exact_hyb = isnan(tc->x_exact) ? -1.0 : fabs(res_hybrid.x_star - tc->x_exact);

    /* Imprimir resumo */
    printf("    Midpoint:   x*=%.10f  iters=%3d  evals=%5d  err=%.2e",
           res_mid.x_star, res_mid.iterations, res_mid.f_evals, res_mid.error);
    if (err_exact_mid >= 0) printf("  |x*-exact|=%.2e", err_exact_mid);
    printf("\n");

    printf("    Median1/d:  x*=%.10f  iters=%3d  evals=%5d  err=%.2e",
           res_inv.x_star, res_inv.iterations, res_inv.f_evals, res_inv.error);
    if (err_exact_inv >= 0) printf("  |x*-exact|=%.2e", err_exact_inv);
    printf("\n");

    printf("    ITP:        x*=%.10f  iters=%3d  evals=%5d  err=%.2e",
           res_itp.x_star, res_itp.iterations, res_itp.f_evals, res_itp.error);
    if (err_exact_itp >= 0) printf("  |x*-exact|=%.2e", err_exact_itp);
    printf("\n");

    printf("    CBS+ITP:    x*=%.10f  iters=%3d  evals=%5d  err=%.2e",
           res_hybrid.x_star, res_hybrid.iterations, res_hybrid.f_evals, res_hybrid.error);
    if (err_exact_hyb >= 0) printf("  |x*-exact|=%.2e", err_exact_hyb);
    printf("\n\n");

    /* Escrever resumo no CSV */
    fprintf(csv_summary, "%s,midpoint,%.15e,%d,%d,%.15e,%.15e\n",
            tc->name, res_mid.x_star, res_mid.iterations, res_mid.f_evals,
            res_mid.error, err_exact_mid);
    fprintf(csv_summary, "%s,median_inv,%.15e,%d,%d,%.15e,%.15e\n",
            tc->name, res_inv.x_star, res_inv.iterations, res_inv.f_evals,
            res_inv.error, err_exact_inv);
    fprintf(csv_summary, "%s,itp,%.15e,%d,%d,%.15e,%.15e\n",
            tc->name, res_itp.x_star, res_itp.iterations, res_itp.f_evals,
            res_itp.error, err_exact_itp);
    fprintf(csv_summary, "%s,cbs_itp,%.15e,%d,%d,%.15e,%.15e\n",
            tc->name, res_hybrid.x_star, res_hybrid.iterations, res_hybrid.f_evals,
            res_hybrid.error, err_exact_hyb);

    /* Escrever históricos no CSV */
    for (int i = 0; i < hist_mid->count; i++) {
        cbs_iteration_t *e = &hist_mid->entries[i];
        fprintf(csv_history, "%s,midpoint,%d,%.15e,%.15e,%.15e,%.15e,%.15e,%.15e\n",
                tc->name, i + 1, e->a, e->b, e->pivot, e->f_pivot, e->error, e->interval_width);
    }
    for (int i = 0; i < hist_inv->count; i++) {
        cbs_iteration_t *e = &hist_inv->entries[i];
        fprintf(csv_history, "%s,median_inv,%d,%.15e,%.15e,%.15e,%.15e,%.15e,%.15e\n",
                tc->name, i + 1, e->a, e->b, e->pivot, e->f_pivot, e->error, e->interval_width);
    }
    for (int i = 0; i < hist_itp->count; i++) {
        cbs_iteration_t *e = &hist_itp->entries[i];
        fprintf(csv_history, "%s,itp,%d,%.15e,%.15e,%.15e,%.15e,%.15e,%.15e\n",
                tc->name, i + 1, e->a, e->b, e->pivot, e->f_pivot, e->error, e->interval_width);
    }
    for (int i = 0; i < hist_hybrid->count; i++) {
        cbs_iteration_t *e = &hist_hybrid->entries[i];
        fprintf(csv_history, "%s,cbs_itp,%d,%.15e,%.15e,%.15e,%.15e,%.15e,%.15e\n",
                tc->name, i + 1, e->a, e->b, e->pivot, e->f_pivot, e->error, e->interval_width);
    }

    cbs_history_free(hist_mid);
    cbs_history_free(hist_med);
    cbs_history_free(hist_inv);
    cbs_history_free(hist_itp);
    cbs_history_free(hist_hybrid);
}

/* --- Main --- */
int main(void)
{
    printf("==========================================================\n");
    printf("  BUSCA BINÁRIA CONTÍNUA - BENCHMARK\n");
    printf("  Comparação: Midpoint vs Median|f| vs Median(1/|f-T|) vs ITP\n");
    printf("  Precisão: eps = %.0e | Max iter: %d\n", EPS, MAX_ITER);
    printf("==========================================================\n\n");

    /* Abrir CSVs de saída */
    FILE *csv_summary = fopen("results_summary.csv", "w");
    FILE *csv_history = fopen("results_history.csv", "w");

    if (!csv_summary || !csv_history) {
        fprintf(stderr, "Erro ao abrir arquivos de saída\n");
        return 1;
    }

    fprintf(csv_summary, "test,method,x_star,iterations,f_evals,error,error_exact\n");
    fprintf(csv_history, "test,method,iteration,a,b,pivot,f_pivot,error,interval_width\n");

    /* --- Definir casos de teste --- */

    /* Teste 1: Gaussiana padrão, ramo esquerdo */
    gaussian_params_t gauss1 = {.mu = 0.0, .sigma = 1.0};
    /* f(x) = exp(-x²/2), buscar T=0.5 em [-5, 0] (crescente) */
    /* Solução exata: x* = -sqrt(2*ln(2)) ≈ -1.17741 */
    test_case_t t1 = {
        .name = "Gaussian(0,1) T=0.5",
        .f = fn_gaussian, .params = &gauss1,
        .T = 0.5, .a = -5.0, .b = 0.0,
        .x_exact = -sqrt(2.0 * log(2.0))
    };

    /* Teste 2: Gaussiana, T mais baixo (solução longe do centro) */
    test_case_t t2 = {
        .name = "Gaussian(0,1) T=0.1",
        .f = fn_gaussian, .params = &gauss1,
        .T = 0.1, .a = -5.0, .b = 0.0,
        .x_exact = -sqrt(-2.0 * log(0.1))
    };

    /* Teste 3: Gaussiana, T alto (solução perto do centro) */
    test_case_t t3 = {
        .name = "Gaussian(0,1) T=0.9",
        .f = fn_gaussian, .params = &gauss1,
        .T = 0.9, .a = -5.0, .b = 0.0,
        .x_exact = -sqrt(-2.0 * log(0.9))
    };

    /* Teste 4: Gaussiana larga σ=5 */
    gaussian_params_t gauss_wide = {.mu = 0.0, .sigma = 5.0};
    test_case_t t4 = {
        .name = "Gaussian(0,5) T=0.5",
        .f = fn_gaussian, .params = &gauss_wide,
        .T = 0.5, .a = -25.0, .b = 0.0,
        .x_exact = -5.0 * sqrt(2.0 * log(2.0))
    };

    /* Teste 5: Sigmoid (já monótona, crescente) */
    /* f(x) = 1/(1+exp(-x)), buscar T=0.7 em [-10, 10] */
    /* Solução: x* = -ln(1/0.7 - 1) = ln(7/3) ≈ 0.8473 */
    test_case_t t5 = {
        .name = "Sigmoid T=0.7",
        .f = fn_sigmoid, .params = NULL,
        .T = 0.7, .a = -10.0, .b = 10.0,
        .x_exact = log(7.0 / 3.0)
    };

    /* Teste 6: Sigmoid, T extremo (solução no canto) */
    test_case_t t6 = {
        .name = "Sigmoid T=0.99",
        .f = fn_sigmoid, .params = NULL,
        .T = 0.99, .a = -10.0, .b = 10.0,
        .x_exact = log(99.0)
    };

    /* Teste 7: Cúbica monótona f(x) = x³ em [-2, 2], T=0.5 */
    cubic_params_t cubic_mono = {.a = 1.0, .b = 0.0, .c = 0.0, .d = 0.0};
    test_case_t t7 = {
        .name = "Cubic x^3 T=0.5",
        .f = fn_cubic, .params = &cubic_mono,
        .T = 0.5, .a = 0.0, .b = 2.0,
        .x_exact = cbrt(0.5)
    };

    /* Teste 8: Cúbica monótona f(x) = x³, T pequeno */
    test_case_t t8 = {
        .name = "Cubic x^3 T=0.001",
        .f = fn_cubic, .params = &cubic_mono,
        .T = 0.001, .a = 0.0, .b = 2.0,
        .x_exact = cbrt(0.001)
    };

    /* Teste 9: Gaussiana estreita (massa concentrada) */
    /* Aqui a mediana deve mostrar vantagem: a massa está perto de x=0.7 */
    narrow_gaussian_params_t narrow = {.mu = 0.7, .sigma = 0.01, .amplitude = 1.0};
    test_case_t t9 = {
        .name = "NarrowGauss(0.7,0.01) T=0.5",
        .f = fn_narrow_gaussian, .params = &narrow,
        .T = 0.5, .a = 0.0, .b = 0.7,  /* ramo crescente */
        .x_exact = 0.7 - 0.01 * sqrt(-2.0 * log(0.5))
    };

    /* Teste 10: Gaussiana assimétrica - intervalo grande, solução num canto */
    gaussian_params_t gauss_asym = {.mu = 9.0, .sigma = 1.0};
    test_case_t t10 = {
        .name = "Gaussian(9,1) T=0.5 [0,9]",
        .f = fn_gaussian, .params = &gauss_asym,
        .T = 0.5, .a = 0.0, .b = 9.0,  /* ramo crescente, solução perto de 9 */
        .x_exact = 9.0 - sqrt(2.0 * log(2.0))
    };

    /* Array de testes */
    test_case_t tests[] = {t1, t2, t3, t4, t5, t6, t7, t8, t9, t10};
    int n_tests = sizeof(tests) / sizeof(tests[0]);

    /* Executar todos */
    printf("Executando %d testes...\n\n", n_tests);

    clock_t start = clock();
    for (int i = 0; i < n_tests; i++) {
        printf("[Teste %d/%d]\n", i + 1, n_tests);
        run_test(&tests[i], csv_summary, csv_history);
    }
    clock_t end = clock();

    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("==========================================================\n");
    printf("  Tempo total: %.4f ms\n", elapsed * 1000.0);
    printf("  Resultados salvos em:\n");
    printf("    - results_summary.csv\n");
    printf("    - results_history.csv\n");
    printf("==========================================================\n");

    fclose(csv_summary);
    fclose(csv_history);

    return 0;
}
