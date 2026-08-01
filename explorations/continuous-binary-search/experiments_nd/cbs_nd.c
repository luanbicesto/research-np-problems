/**
 * cbs_nd.c — Implementação do CBS-nD (variante Box)
 *
 * Algoritmo:
 *   1. Centro do box → avaliar f
 *   2. Gradiente por diferenças finitas centrais (2n evals)
 *   3. Escolher coordenada de corte (maior |∂f/∂xᵢ|)
 *   4. Cortar o box baseado no sinal de (f(q)-T) e grad[j]
 *   5. Repetir até |f(q)-T| < ε ou max_iter
 *
 * Complexidade: O(n² log(R/ε)) avaliações de f
 */

#include "cbs_nd.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

/* Parâmetros default estáticos */
static const cbs_nd_params_t DEFAULT_PARAMS = CBS_ND_DEFAULT_PARAMS;

/**
 * Estima gradiente por diferenças finitas centrais.
 * Custo: 2n avaliações de f.
 */
static int estimate_gradient(
    func_nd_t f, void *params, int n,
    const double *x, const double *lower, const double *upper,
    double h_factor, double *grad, int *evals)
{
    double *x_plus = (double *)malloc(n * sizeof(double));
    double *x_minus = (double *)malloc(n * sizeof(double));
    if (!x_plus || !x_minus) {
        free(x_plus);
        free(x_minus);
        return -1;
    }

    memcpy(x_plus, x, n * sizeof(double));
    memcpy(x_minus, x, n * sizeof(double));

    for (int i = 0; i < n; i++) {
        double width = upper[i] - lower[i];
        double h = width * h_factor;
        if (h < 1e-15) h = 1e-15;

        /* Clamp para dentro do domínio */
        double xi_plus = x[i] + h;
        double xi_minus = x[i] - h;
        if (xi_plus > upper[i]) xi_plus = upper[i];
        if (xi_minus < lower[i]) xi_minus = lower[i];

        double actual_h = xi_plus - xi_minus;
        if (actual_h < 1e-15) {
            grad[i] = 0.0;
            continue;
        }

        x_plus[i] = xi_plus;
        x_minus[i] = xi_minus;

        double f_plus = f(x_plus, n, params);
        double f_minus = f(x_minus, n, params);
        (*evals) += 2;

        grad[i] = (f_plus - f_minus) / actual_h;

        /* Restaurar */
        x_plus[i] = x[i];
        x_minus[i] = x[i];
    }

    free(x_plus);
    free(x_minus);
    return 0;
}

/**
 * Escolher coordenada de corte.
 */
static int choose_cut_coordinate(
    const double *grad, const double *lower, const double *upper,
    int n, int iter, cbs_nd_cut_method_t method)
{
    switch (method) {
    case CUT_CYCLIC:
        return iter % n;

    case CUT_MAX_WIDTH: {
        int best = 0;
        double max_w = upper[0] - lower[0];
        for (int i = 1; i < n; i++) {
            double w = upper[i] - lower[i];
            if (w > max_w) { max_w = w; best = i; }
        }
        return best;
    }

    case CUT_MAX_GRADIENT:
    default: {
        int best = 0;
        double max_g = fabs(grad[0]);
        for (int i = 1; i < n; i++) {
            double g = fabs(grad[i]);
            if (g > max_g) { max_g = g; best = i; }
        }
        /* Fallback: se gradiente ≈ 0, usar max_width */
        if (max_g < 1e-15) {
            double max_w = upper[0] - lower[0];
            best = 0;
            for (int i = 1; i < n; i++) {
                double w = upper[i] - lower[i];
                if (w > max_w) { max_w = w; best = i; }
            }
        }
        return best;
    }
    }
}

cbs_nd_result_full_t cbs_nd_search(
    func_nd_t f, void *params,
    int n, double T,
    const double *lower_in, const double *upper_in,
    double eps,
    const cbs_nd_params_t *cfg)
{
    const cbs_nd_params_t *p = cfg ? cfg : &DEFAULT_PARAMS;

    /* Resultado */
    cbs_nd_result_full_t res;
    memset(&res, 0, sizeof(res));
    res.x_star = (double *)calloc(n, sizeof(double));
    res.cuts_per_coord = (int *)calloc(n, sizeof(int));

    if (!res.x_star || !res.cuts_per_coord) {
        res.status = CBS_ND_NUMERICAL_ERROR;
        return res;
    }

    /* Cópias mutáveis dos bounds */
    double *lo = (double *)malloc(n * sizeof(double));
    double *hi = (double *)malloc(n * sizeof(double));
    double *q = (double *)malloc(n * sizeof(double));
    double *grad = (double *)malloc(n * sizeof(double));

    if (!lo || !hi || !q || !grad) {
        free(lo); free(hi); free(q); free(grad);
        res.status = CBS_ND_NUMERICAL_ERROR;
        return res;
    }

    memcpy(lo, lower_in, n * sizeof(double));
    memcpy(hi, upper_in, n * sizeof(double));

    int total_evals = 0;
    int iter = 0;
    cbs_nd_status_t status = CBS_ND_MAX_ITER;

    for (iter = 0; iter < p->max_iter; iter++) {
        /* 1. Centro do box */
        for (int i = 0; i < n; i++) {
            q[i] = (lo[i] + hi[i]) / 2.0;
        }

        /* 2. Avaliar f no centro */
        double fq = f(q, n, params);
        total_evals++;

        double error = fabs(fq - T);

        /* 3. Verificar convergência */
        if (error < eps) {
            memcpy(res.x_star, q, n * sizeof(double));
            res.f_value = fq;
            res.error = error;
            res.converged = 1;
            status = CBS_ND_OK;
            break;
        }

        /* 4. Verificar domínio degenerado */
        double max_width = 0.0;
        for (int i = 0; i < n; i++) {
            double w = hi[i] - lo[i];
            if (w > max_width) max_width = w;
        }
        if (max_width < eps * 1e-5) {
            /* Domínio colapsou — retornar melhor ponto */
            memcpy(res.x_star, q, n * sizeof(double));
            res.f_value = fq;
            res.error = error;
            res.converged = (error < eps);
            status = CBS_ND_DEGENERATE_DOMAIN;
            break;
        }

        /* 5. Estimar gradiente */
        if (estimate_gradient(f, params, n, q, lo, hi, p->grad_h_factor, grad, &total_evals) != 0) {
            status = CBS_ND_NUMERICAL_ERROR;
            break;
        }

        /* 6. Escolher coordenada de corte */
        int j = choose_cut_coordinate(grad, lo, hi, n, iter, p->cut_method);

        /* 7. Executar corte */
        /*
         * Lógica: se f(q) > T, queremos ir para onde f diminui.
         *         Se grad[j] > 0, f diminui para x_j menores → cortar [q_j, hi_j]
         *         Se grad[j] < 0, f diminui para x_j maiores → cortar [lo_j, q_j]
         *
         * Equivalente: cortar o lado onde f se afasta de T.
         */
        if ((fq > T) == (grad[j] > 0)) {
            /* Solução está em x_j < q_j */
            hi[j] = q[j];
        } else {
            /* Solução está em x_j > q_j */
            lo[j] = q[j];
        }

        res.cuts_per_coord[j]++;

        if (p->verbose && (iter % 100 == 0 || iter < 10)) {
            fprintf(stderr, "[CBS-nD] iter=%d, f(q)=%.6e, err=%.2e, cut coord=%d, width=%.2e\n",
                    iter, fq, error, j, hi[j] - lo[j] + (hi[j] == q[j] ? q[j] - lo[j] : 0));
        }

        /* Guardar último ponto avaliado */
        memcpy(res.x_star, q, n * sizeof(double));
        res.f_value = fq;
        res.error = error;
    }

    res.total_evals = total_evals;
    res.n_iterations = iter;
    res.status = status;

    free(lo);
    free(hi);
    free(q);
    free(grad);

    return res;
}

void cbs_nd_free(cbs_nd_result_full_t *result) {
    if (result) {
        free(result->x_star);
        free(result->cuts_per_coord);
        result->x_star = NULL;
        result->cuts_per_coord = NULL;
    }
}
