/**
 * benchmark_nd.c — Benchmark do CBS-nD
 *
 * Testa o algoritmo em funções convexas e monótonas por coordenada
 * para n = 2, 5, 10, 20.
 *
 * Funções de teste:
 *   1. Esfera: f(x) = Σ xᵢ²                    (convexa, separável)
 *   2. Elipsóide: f(x) = Σ iᵢ·xᵢ²              (convexa, ill-conditioned)
 *   3. Rosenbrock sum: f(x) = Σ (1-xᵢ)²+100(xᵢ₊₁-xᵢ²)²  (não-convexa!)
 *   4. Sum of abs: f(x) = Σ |xᵢ|               (convexa, não-diferenciável)
 *   5. Linear: f(x) = Σ (i+1)·xᵢ              (linear, trivial)
 *   6. Quadrática geral: f(x) = xᵀAx + bᵀx    (convexa)
 *   7. Max coordenada: f(x) = max_i xᵢ         (convexa, não-diferenciável)
 *   8. Log-sum-exp: f(x) = log(Σ exp(xᵢ))     (convexa, smooth)
 */

#include "cbs_nd.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

/* ============================================================
 * Funções de teste
 * ============================================================ */

/* 1. Esfera: f(x) = Σ xᵢ² */
static double f_sphere(const double *x, int n, void *params) {
    (void)params;
    double sum = 0.0;
    for (int i = 0; i < n; i++) sum += x[i] * x[i];
    return sum;
}

/* 2. Elipsóide: f(x) = Σ (i+1)·xᵢ² */
static double f_ellipsoid(const double *x, int n, void *params) {
    (void)params;
    double sum = 0.0;
    for (int i = 0; i < n; i++) sum += (i + 1.0) * x[i] * x[i];
    return sum;
}

/* 3. Rosenbrock sum (não-convexa) */
static double f_rosenbrock(const double *x, int n, void *params) {
    (void)params;
    double sum = 0.0;
    for (int i = 0; i < n - 1; i++) {
        double a = 1.0 - x[i];
        double b = x[i + 1] - x[i] * x[i];
        sum += a * a + 100.0 * b * b;
    }
    return sum;
}

/* 4. Sum of abs: f(x) = Σ |xᵢ| */
static double f_sum_abs(const double *x, int n, void *params) {
    (void)params;
    double sum = 0.0;
    for (int i = 0; i < n; i++) sum += fabs(x[i]);
    return sum;
}

/* 5. Linear: f(x) = Σ (i+1)·xᵢ */
static double f_linear(const double *x, int n, void *params) {
    (void)params;
    double sum = 0.0;
    for (int i = 0; i < n; i++) sum += (i + 1.0) * x[i];
    return sum;
}

/* 6. Quadrática com termos cruzados: f(x) = Σᵢ xᵢ² + 0.5·Σᵢ xᵢ·xᵢ₊₁ */
static double f_quadratic_cross(const double *x, int n, void *params) {
    (void)params;
    double sum = 0.0;
    for (int i = 0; i < n; i++) sum += x[i] * x[i];
    for (int i = 0; i < n - 1; i++) sum += 0.5 * x[i] * x[i + 1];
    return sum;
}

/* 7. Max coordenada: f(x) = max_i xᵢ */
static double f_max_coord(const double *x, int n, void *params) {
    (void)params;
    double mx = x[0];
    for (int i = 1; i < n; i++) {
        if (x[i] > mx) mx = x[i];
    }
    return mx;
}

/* 8. Log-sum-exp: f(x) = log(Σ exp(xᵢ)) */
static double f_logsumexp(const double *x, int n, void *params) {
    (void)params;
    /* Numerically stable */
    double mx = x[0];
    for (int i = 1; i < n; i++) {
        if (x[i] > mx) mx = x[i];
    }
    double sum = 0.0;
    for (int i = 0; i < n; i++) sum += exp(x[i] - mx);
    return mx + log(sum);
}

/* ============================================================
 * Estrutura de teste
 * ============================================================ */

typedef struct {
    const char *name;
    func_nd_t func;
    double T;
    double lower_val;   /* Bounds uniformes: [lower_val, upper_val]^n */
    double upper_val;
    const char *properties;  /* "convex", "non-convex", "linear", etc */
} test_case_t;

static test_case_t test_cases[] = {
    {"Sphere",          f_sphere,          5.0,   -5.0,  5.0,  "convex, separable"},
    {"Ellipsoid",       f_ellipsoid,       10.0,  -5.0,  5.0,  "convex, ill-cond"},
    {"Rosenbrock",      f_rosenbrock,      1.0,   -2.0,  2.0,  "non-convex"},
    {"Sum|x|",          f_sum_abs,         3.0,   -5.0,  5.0,  "convex, non-smooth"},
    {"Linear",          f_linear,          15.0,  -5.0,  5.0,  "linear"},
    {"Quad+cross",      f_quadratic_cross, 8.0,   -5.0,  5.0,  "convex"},
    {"Max coord",       f_max_coord,       2.0,   -5.0,  5.0,  "convex, non-smooth"},
    {"LogSumExp",       f_logsumexp,       3.0,   -5.0,  5.0,  "convex, smooth"},
};

#define N_TESTS (sizeof(test_cases) / sizeof(test_cases[0]))

/* ============================================================
 * Gradient Descent baseline para comparação
 * ============================================================ */

typedef struct {
    double *x_star;
    double error;
    int evals;
    int converged;
} gd_result_t;

static gd_result_t gradient_descent_level(
    func_nd_t f, void *params, int n, double T,
    const double *lower, const double *upper, double eps, int max_iter)
{
    gd_result_t res;
    res.x_star = (double *)malloc(n * sizeof(double));
    res.evals = 0;
    res.converged = 0;

    /* Ponto inicial: centro do domínio */
    double *x = (double *)malloc(n * sizeof(double));
    double *grad = (double *)malloc(n * sizeof(double));

    for (int i = 0; i < n; i++)
        x[i] = (lower[i] + upper[i]) / 2.0;

    for (int iter = 0; iter < max_iter; iter++) {
        double fx = f(x, n, params);
        res.evals++;

        double residual = fx - T;
        if (fabs(residual) < eps) {
            res.converged = 1;
            res.error = fabs(residual);
            memcpy(res.x_star, x, n * sizeof(double));
            free(x); free(grad);
            return res;
        }

        /* Gradiente de (f(x)-T)² = 2(f(x)-T)·∇f */
        for (int i = 0; i < n; i++) {
            double h = (upper[i] - lower[i]) * 1e-7;
            if (h < 1e-15) h = 1e-15;
            double xp = x[i] + h;
            double xm = x[i] - h;
            if (xp > upper[i]) xp = upper[i];
            if (xm < lower[i]) xm = lower[i];

            double old_xi = x[i];
            x[i] = xp;
            double fp = f(x, n, params);
            x[i] = xm;
            double fm = f(x, n, params);
            x[i] = old_xi;
            res.evals += 2;

            grad[i] = 2.0 * residual * (fp - fm) / (xp - xm);
        }

        /* Norm do gradiente */
        double norm = 0.0;
        for (int i = 0; i < n; i++) norm += grad[i] * grad[i];
        norm = sqrt(norm);
        if (norm < 1e-15) break;

        /* Step size: backtracking line search simples */
        double alpha = 0.1 / norm;

        /* Update com clamp */
        for (int i = 0; i < n; i++) {
            x[i] -= alpha * grad[i];
            if (x[i] < lower[i]) x[i] = lower[i];
            if (x[i] > upper[i]) x[i] = upper[i];
        }
    }

    double fx = f(x, n, params);
    res.evals++;
    res.error = fabs(fx - T);
    res.converged = (res.error < eps);
    memcpy(res.x_star, x, n * sizeof(double));
    free(x); free(grad);
    return res;
}

/* ============================================================
 * Main — Benchmark
 * ============================================================ */

int main(void) {
    int dimensions[] = {2, 5, 10, 20};
    int n_dims = 4;
    double eps = 1e-8;

    printf("==========================================================\n");
    printf("  CBS-nD Benchmark: Busca de Level Set em R^n\n");
    printf("  Precisão: eps = %.0e\n", eps);
    printf("==========================================================\n\n");

    int total_tests = 0, total_pass = 0;

    for (int di = 0; di < n_dims; di++) {
        int n = dimensions[di];
        printf("──────────────────────────────────────────────────────────\n");
        printf("  DIMENSÃO n = %d\n", n);
        printf("──────────────────────────────────────────────────────────\n");
        printf("%-14s │ %6s │ %6s │ %10s │ %10s │ %s\n",
               "Função", "Evals", "Iters", "Erro", "Status", "Propriedades");
        printf("───────────────┼────────┼────────┼────────────┼────────────┼─────────────\n");

        for (size_t ti = 0; ti < N_TESTS; ti++) {
            test_case_t *tc = &test_cases[ti];

            /* Criar bounds */
            double *lower = (double *)malloc(n * sizeof(double));
            double *upper = (double *)malloc(n * sizeof(double));
            for (int i = 0; i < n; i++) {
                lower[i] = tc->lower_val;
                upper[i] = tc->upper_val;
            }

            /* CBS-nD */
            cbs_nd_params_t cfg = CBS_ND_DEFAULT_PARAMS;
            cfg.max_iter = 5000;

            cbs_nd_result_full_t res = cbs_nd_search(
                tc->func, NULL, n, tc->T, lower, upper, eps, &cfg);

            total_tests++;
            const char *status_str;
            if (res.converged) {
                total_pass++;
                status_str = "✓ PASS";
            } else {
                switch (res.status) {
                case CBS_ND_MAX_ITER: status_str = "✗ MAX_ITER"; break;
                case CBS_ND_DEGENERATE_DOMAIN: status_str = "✗ DEGEN"; break;
                default: status_str = "✗ FAIL"; break;
                }
            }

            printf("%-14s │ %6d │ %6d │ %10.2e │ %10s │ %s\n",
                   tc->name, res.total_evals, res.n_iterations,
                   res.error, status_str, tc->properties);

            cbs_nd_free(&res);
            free(lower);
            free(upper);
        }
        printf("\n");
    }

    printf("==========================================================\n");
    printf("  RESULTADO: %d/%d testes passaram (eps=%.0e)\n", total_pass, total_tests, eps);
    printf("==========================================================\n\n");

    /* Comparação CBS vs Gradient Descent para n=10 */
    printf("──────────────────────────────────────────────────────────\n");
    printf("  COMPARAÇÃO: CBS-nD vs Gradient Descent (n=10)\n");
    printf("──────────────────────────────────────────────────────────\n");
    printf("%-14s │ %8s %8s │ %8s %8s │ %s\n",
           "Função", "CBS_eval", "CBS_err", "GD_eval", "GD_err", "Vencedor");
    printf("───────────────┼───────────────────┼───────────────────┼──────────\n");

    int n_compare = 10;
    for (size_t ti = 0; ti < N_TESTS; ti++) {
        test_case_t *tc = &test_cases[ti];

        double *lower = (double *)malloc(n_compare * sizeof(double));
        double *upper = (double *)malloc(n_compare * sizeof(double));
        for (int i = 0; i < n_compare; i++) {
            lower[i] = tc->lower_val;
            upper[i] = tc->upper_val;
        }

        /* CBS */
        cbs_nd_params_t cfg = CBS_ND_DEFAULT_PARAMS;
        cfg.max_iter = 5000;
        cbs_nd_result_full_t cbs_res = cbs_nd_search(
            tc->func, NULL, n_compare, tc->T, lower, upper, eps, &cfg);

        /* GD */
        gd_result_t gd_res = gradient_descent_level(
            tc->func, NULL, n_compare, tc->T, lower, upper, eps, 5000);

        const char *winner;
        if (cbs_res.converged && !gd_res.converged)
            winner = "CBS";
        else if (!cbs_res.converged && gd_res.converged)
            winner = "GD";
        else if (cbs_res.converged && gd_res.converged)
            winner = (cbs_res.total_evals <= gd_res.evals) ? "CBS" : "GD";
        else
            winner = "AMBOS FALHAM";

        printf("%-14s │ %8d %8.1e │ %8d %8.1e │ %s\n",
               tc->name,
               cbs_res.total_evals, cbs_res.error,
               gd_res.evals, gd_res.error,
               winner);

        cbs_nd_free(&cbs_res);
        free(gd_res.x_star);
        free(lower);
        free(upper);
    }

    printf("\n");
    return 0;
}
