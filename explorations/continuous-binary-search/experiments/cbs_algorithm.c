/**
 * cbs_algorithm.c - Implementação da Busca Binária Contínua
 */

#include "cbs_algorithm.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

/* --- Histórico --- */

cbs_history_t *cbs_history_create(int initial_capacity)
{
    cbs_history_t *h = malloc(sizeof(cbs_history_t));
    if (!h) return NULL;
    h->entries = malloc(sizeof(cbs_iteration_t) * initial_capacity);
    if (!h->entries) { free(h); return NULL; }
    h->count = 0;
    h->capacity = initial_capacity;
    return h;
}

void cbs_history_free(cbs_history_t *h)
{
    if (h) {
        free(h->entries);
        free(h);
    }
}

static void history_append(cbs_history_t *h, cbs_iteration_t *entry)
{
    if (!h) return;
    if (h->count >= h->capacity) {
        h->capacity *= 2;
        h->entries = realloc(h->entries, sizeof(cbs_iteration_t) * h->capacity);
    }
    h->entries[h->count++] = *entry;
}

/* --- Cálculo do pivô pela mediana --- */

/**
 * Calcula a mediana de |f(x)| no intervalo [a, b] usando
 * integração numérica (regra do trapézio) + bisseção interna.
 *
 * Encontra x_m tal que ∫_a^{x_m} |f(t)|dt = ½ · ∫_a^b |f(t)|dt
 */
static double compute_median_pivot(
    func_t f, void *params,
    double a, double b,
    int n_quad_points,
    int *f_evals_out)
{
    int evals = 0;

    /* Pré-computar |f| em pontos equidistantes para integração */
    double *abs_f = malloc(sizeof(double) * (n_quad_points + 1));
    double dx = (b - a) / n_quad_points;

    for (int i = 0; i <= n_quad_points; i++) {
        double x = a + i * dx;
        abs_f[i] = fabs(f(x, params));
        evals++;
    }

    /* Integral total via trapézio */
    double total = 0.0;
    for (int i = 0; i < n_quad_points; i++) {
        total += (abs_f[i] + abs_f[i + 1]) * dx / 2.0;
    }

    double half_total = total / 2.0;

    /* Encontrar x_m onde a integral acumulada atinge half_total */
    double cumulative = 0.0;
    double x_m = a;

    for (int i = 0; i < n_quad_points; i++) {
        double segment = (abs_f[i] + abs_f[i + 1]) * dx / 2.0;
        if (cumulative + segment >= half_total) {
            /* Interpolar dentro do segmento */
            double remaining = half_total - cumulative;
            /* Aproximação linear dentro do segmento */
            double frac = (segment > 1e-15) ? remaining / segment : 0.5;
            x_m = a + (i + frac) * dx;
            break;
        }
        cumulative += segment;
    }

    free(abs_f);
    *f_evals_out = evals;
    return x_m;
}

/* --- Cálculo do pivô pela mediana inversa: peso = 1/|f(x)-T| --- */

/**
 * Calcula a mediana de 1/|f(x)-T| no intervalo [a, b].
 * Concentra massa perto de onde f(x) ≈ T (perto da solução).
 *
 * Usa regularização: w(x) = 1/(|f(x)-T| + delta) para evitar singularidade.
 */
static double compute_median_inv_pivot(
    func_t f, void *params,
    double T,
    double a, double b,
    int n_quad_points,
    int *f_evals_out)
{
    int evals = 0;
    double delta = 1e-12;  /* Regularização */

    /* Pré-computar peso em pontos equidistantes */
    double *weights = malloc(sizeof(double) * (n_quad_points + 1));
    double dx = (b - a) / n_quad_points;

    for (int i = 0; i <= n_quad_points; i++) {
        double x = a + i * dx;
        double fx = f(x, params);
        evals++;
        weights[i] = 1.0 / (fabs(fx - T) + delta);
    }

    /* Integral total via trapézio */
    double total = 0.0;
    for (int i = 0; i < n_quad_points; i++) {
        total += (weights[i] + weights[i + 1]) * dx / 2.0;
    }

    double half_total = total / 2.0;

    /* Encontrar x_m onde a integral acumulada atinge half_total */
    double cumulative = 0.0;
    double x_m = (a + b) / 2.0;  /* fallback */

    for (int i = 0; i < n_quad_points; i++) {
        double segment = (weights[i] + weights[i + 1]) * dx / 2.0;
        if (cumulative + segment >= half_total) {
            double remaining = half_total - cumulative;
            double frac = (segment > 1e-15) ? remaining / segment : 0.5;
            x_m = a + (i + frac) * dx;
            break;
        }
        cumulative += segment;
    }

    free(weights);
    *f_evals_out = evals;
    return x_m;
}

/* --- Busca principal --- */

cbs_result_t cbs_search(
    func_t f, void *params,
    double T,
    double a, double b,
    double eps, int max_iter,
    pivot_method_t method,
    cbs_history_t *history)
{
    cbs_result_t result;
    memset(&result, 0, sizeof(result));

    /* Avaliar extremos */
    double fa = f(a, params);
    double fb = f(b, params);
    result.f_evals = 2;

    /* Determinar direção de monotonicidade */
    int increasing = (fa < fb) ? 1 : 0;

    /* Número de pontos de quadratura para mediana */
    int n_quad = 64;

    for (int k = 0; k < max_iter; k++) {
        /* Calcular pivô */
        double pivot;
        if (method == PIVOT_MIDPOINT) {
            pivot = (a + b) / 2.0;
        } else if (method == PIVOT_MEDIAN) {
            int quad_evals = 0;
            pivot = compute_median_pivot(f, params, a, b, n_quad, &quad_evals);
            result.f_evals += quad_evals;
        } else { /* PIVOT_MEDIAN_INV */
            int quad_evals = 0;
            pivot = compute_median_inv_pivot(f, params, T, a, b, n_quad, &quad_evals);
            result.f_evals += quad_evals;
        }

        /* Avaliar f no pivô */
        double f_pivot = f(pivot, params);
        result.f_evals++;

        double error = fabs(f_pivot - T);

        /* Registrar histórico */
        if (history) {
            cbs_iteration_t entry = {
                .a = a, .b = b,
                .pivot = pivot,
                .f_pivot = f_pivot,
                .error = error,
                .interval_width = b - a
            };
            history_append(history, &entry);
        }

        /* Verificar convergência */
        if (error < eps) {
            result.x_star = pivot;
            result.f_x_star = f_pivot;
            result.error = error;
            result.iterations = k + 1;
            result.converged = 1;
            return result;
        }

        /* Verificar convergência por largura do intervalo */
        if (b - a < eps * 1e-10) {
            result.x_star = pivot;
            result.f_x_star = f_pivot;
            result.error = error;
            result.iterations = k + 1;
            result.converged = 1;
            return result;
        }

        /* Decisão: qual lado contém x*? */
        if (increasing) {
            if (f_pivot < T) {
                a = pivot;
            } else {
                b = pivot;
            }
        } else {
            if (f_pivot < T) {
                b = pivot;
            } else {
                a = pivot;
            }
        }
    }

    /* Não convergiu */
    double final_pivot = (a + b) / 2.0;
    result.x_star = final_pivot;
    result.f_x_star = f(final_pivot, params);
    result.f_evals++;
    result.error = fabs(result.f_x_star - T);
    result.iterations = max_iter;
    result.converged = 0;
    return result;
}

/* --- Detecção de monotonicidade --- */

monotone_dir_t cbs_detect_monotonicity(
    func_t f, void *params,
    double a, double b,
    int n_samples)
{
    if (n_samples < 2) n_samples = 2;

    double dx = (b - a) / (n_samples - 1);
    double prev = f(a, params);
    int inc_count = 0, dec_count = 0;

    for (int i = 1; i < n_samples; i++) {
        double x = a + i * dx;
        double curr = f(x, params);
        if (curr > prev) inc_count++;
        else if (curr < prev) dec_count++;
        prev = curr;
    }

    if (inc_count == n_samples - 1) return MONOTONE_INCREASING;
    if (dec_count == n_samples - 1) return MONOTONE_DECREASING;
    return MONOTONE_UNKNOWN;
}

/* --- Seleção de ramo --- */

int cbs_select_branch(
    func_t f, void *params,
    double T,
    double a, double b,
    double *out_a, double *out_b,
    monotone_dir_t *out_dir)
{
    /* Encontrar máximo por busca ternária */
    double lo = a, hi = b;
    for (int i = 0; i < 100; i++) {
        double m1 = lo + (hi - lo) / 3.0;
        double m2 = hi - (hi - lo) / 3.0;
        if (f(m1, params) < f(m2, params)) {
            lo = m1;
        } else {
            hi = m2;
        }
    }
    double x_max = (lo + hi) / 2.0;
    double f_max = f(x_max, params);

    /* T acima do máximo: sem solução */
    if (T > f_max) return 0;

    /* Tentar ramo esquerdo [a, x_max] (crescente) */
    double fa = f(a, params);
    if (fa <= T && T <= f_max) {
        *out_a = a;
        *out_b = x_max;
        *out_dir = MONOTONE_INCREASING;
        return 1;
    }

    /* Tentar ramo direito [x_max, b] (decrescente) */
    double fb = f(b, params);
    if (fb <= T && T <= f_max) {
        *out_a = x_max;
        *out_b = b;
        *out_dir = MONOTONE_DECREASING;
        return 1;
    }

    return 0;
}
