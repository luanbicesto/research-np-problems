/**
 * cbs_itp.c - Implementação do método híbrido CBS + ITP
 *
 * Fase 1: Varredura global com mediana de 1/|f(x)-T| para localizar a solução
 * Fase 2: ITP para convergência superlinear
 */

#include "cbs_itp.h"
#include "itp_method.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

/* Importar a função de cálculo da mediana inversa (definida em cbs_algorithm.c) */
/* Como é static lá, reimplementamos aqui de forma local */
static double compute_median_inv(
    func_t f, void *params,
    double T,
    double a, double b,
    int n_quad_points,
    int *f_evals_out)
{
    int evals = 0;
    double delta = 1e-12;

    double *weights = malloc(sizeof(double) * (n_quad_points + 1));
    double dx = (b - a) / n_quad_points;

    for (int i = 0; i <= n_quad_points; i++) {
        double x = a + i * dx;
        double fx = f(x, params);
        evals++;
        weights[i] = 1.0 / (fabs(fx - T) + delta);
    }

    double total = 0.0;
    for (int i = 0; i < n_quad_points; i++) {
        total += (weights[i] + weights[i + 1]) * dx / 2.0;
    }

    double half_total = total / 2.0;
    double cumulative = 0.0;
    double x_m = (a + b) / 2.0;

    for (int i = 0; i < n_quad_points; i++) {
        double segment = (weights[i] + weights[i + 1]) * dx / 2.0;
        if (cumulative + segment >= half_total) {
            double remaining = half_total - cumulative;
            double frac = (segment > 1e-15) ? remaining / segment : 0.5;
            if (frac < 0.0) frac = 0.0;
            if (frac > 1.0) frac = 1.0;
            x_m = a + (i + frac) * dx;
            break;
        }
        cumulative += segment;
    }

    free(weights);
    *f_evals_out = evals;
    return x_m;
}

static double sign_of(double x)
{
    return (x > 0.0) ? 1.0 : ((x < 0.0) ? -1.0 : 0.0);
}

cbs_result_t cbs_itp_search(
    func_t f, void *params,
    double T,
    double a, double b,
    double eps,
    cbs_itp_params_t *hp,
    cbs_history_t *history)
{
    cbs_result_t result;
    memset(&result, 0, sizeof(result));

    double original_width = b - a;

    /* Avaliar extremos */
    double fa = f(a, params) - T;
    double fb = f(b, params) - T;
    result.f_evals = 2;

    /* Garantir fa < 0 < fb */
    int swapped = 0;
    if (fa > 0.0) {
        double tmp;
        tmp = a; a = b; b = tmp;
        tmp = fa; fa = fb; fb = tmp;
        swapped = 1;
    }

    /* ===== FASE 1: CBS (mediana 1/|f-T|) ===== */
    int cbs_iters = 0;
    while (cbs_iters < hp->cbs_max_iters && (b - a) > 2.0 * eps) {
        /* Calcular pivô pela mediana */
        int quad_evals = 0;
        double pivot = compute_median_inv(f, params, T, a, b, hp->n_quad, &quad_evals);
        result.f_evals += quad_evals;

        /* Garantir que pivô está estritamente no intervalo */
        if (pivot <= a || pivot >= b) {
            pivot = (a + b) / 2.0;
        }

        /* Avaliar f no pivô */
        double y_pivot = f(pivot, params) - T;
        result.f_evals++;

        double error = fabs(y_pivot);

        /* Histórico */
        if (history) {
            cbs_iteration_t entry = {
                .a = a, .b = b,
                .pivot = pivot,
                .f_pivot = y_pivot + T,
                .error = error,
                .interval_width = b - a
            };
            if (history->count < history->capacity) {
                history->entries[history->count++] = entry;
            }
        }

        /* Verificar convergência */
        if (error < eps) {
            result.x_star = pivot;
            result.f_x_star = y_pivot + T;
            result.error = error;
            result.iterations = cbs_iters + 1;
            result.converged = 1;
            return result;
        }

        /* Atualizar intervalo */
        if (y_pivot > 0.0) {
            b = pivot;
            fb = y_pivot;
        } else {
            a = pivot;
            fa = y_pivot;
        }

        cbs_iters++;

        /* Verificar critério de switch */
        if ((b - a) < hp->switch_ratio * original_width) {
            break;
        }
    }

    /* ===== FASE 2: ITP no intervalo reduzido ===== */
    if ((b - a) > 2.0 * eps) {
        /* Calcular n_half para o intervalo reduzido */
        int n_half = (int)ceil(log2((b - a) / (2.0 * eps)));
        int n_max = n_half + hp->n0;

        int j = 0;
        while ((b - a) > 2.0 * eps && j < n_max) {
            /* ITP Step 1: Interpolation */
            double x_half = (a + b) / 2.0;
            double x_f = (fb * a - fa * b) / (fb - fa);

            /* ITP Step 2: Truncation */
            double sigma = sign_of(x_half - x_f);
            double delta = hp->kappa1 * pow(b - a, hp->kappa2);
            double x_t;
            if (delta <= fabs(x_half - x_f)) {
                x_t = x_f + sigma * delta;
            } else {
                x_t = x_half;
            }

            /* ITP Step 3: Projection */
            double r = eps * pow(2.0, n_max - j) - (b - a) / 2.0;
            double x_itp;
            if (fabs(x_t - x_half) <= r) {
                x_itp = x_t;
            } else {
                x_itp = x_half - sigma * r;
            }

            /* Avaliar */
            double y_itp = f(x_itp, params) - T;
            result.f_evals++;

            double error = fabs(y_itp);

            /* Histórico */
            if (history) {
                cbs_iteration_t entry = {
                    .a = a, .b = b,
                    .pivot = x_itp,
                    .f_pivot = y_itp + T,
                    .error = error,
                    .interval_width = b - a
                };
                if (history->count < history->capacity) {
                    history->entries[history->count++] = entry;
                }
            }

            /* Atualizar */
            if (y_itp > 0.0) {
                b = x_itp;
                fb = y_itp;
            } else if (y_itp < 0.0) {
                a = x_itp;
                fa = y_itp;
            } else {
                a = x_itp;
                b = x_itp;
            }

            j++;
        }
    }

    /* Resultado final */
    result.x_star = (a + b) / 2.0;
    result.f_x_star = f(result.x_star, params);
    result.f_evals++;
    result.error = fabs(result.f_x_star - T);
    result.iterations = cbs_iters + (int)((b - a) <= 2.0 * eps ? 0 :
                        ceil(log2((b - a) / (2.0 * eps))));
    /* Contar iterações totais do histórico */
    if (history) {
        result.iterations = history->count;
    }
    result.converged = 1;

    return result;
}
