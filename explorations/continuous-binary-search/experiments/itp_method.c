/**
 * itp_method.c - Implementação do método ITP
 *
 * Referência: Oliveira, I.F.D. & Takahashi, R.H.C. (2021).
 * "An Enhancement of the Bisection Method Average Performance
 *  Preserving Minmax Optimality". ACM Trans. Math. Softw.
 */

#include "itp_method.h"
#include <math.h>
#include <string.h>

/* sign(x): retorna -1, 0, ou +1 */
static double sign(double x)
{
    if (x > 0.0) return 1.0;
    if (x < 0.0) return -1.0;
    return 0.0;
}

cbs_result_t itp_search(
    func_t f, void *params,
    double T,
    double a, double b,
    double eps,
    double kappa1, double kappa2, int n0,
    cbs_history_t *history)
{
    cbs_result_t result;
    memset(&result, 0, sizeof(result));

    /* Avaliar nos extremos (g = f - T) */
    double ya = f(a, params) - T;
    double yb = f(b, params) - T;
    result.f_evals = 2;

    /* Garantir ya < 0 < yb (swap se necessário) */
    if (ya > 0.0) {
        double tmp;
        tmp = a; a = b; b = tmp;
        tmp = ya; ya = yb; yb = tmp;
    }

    /* Calcular n_half (número máximo de iterações da bisseção) */
    int n_half = (int)ceil(log2((b - a) / (2.0 * eps)));
    int n_max = n_half + n0;

    int j = 0;
    while ((b - a) > 2.0 * eps && j < n_max) {
        /* Step 1: Interpolation — calcular ponto médio e regula falsi */
        double x_half = (a + b) / 2.0;
        double x_f = (yb * a - ya * b) / (yb - ya);  /* regula falsi */

        /* Step 2: Truncation — perturbar em direção ao centro */
        double sigma = sign(x_half - x_f);
        double delta = kappa1 * pow(b - a, kappa2);
        double x_t;
        if (delta <= fabs(x_half - x_f)) {
            x_t = x_f + sigma * delta;
        } else {
            x_t = x_half;
        }

        /* Step 3: Projection — projetar no intervalo minmax */
        double r = eps * pow(2.0, n_max - j) - (b - a) / 2.0;
        double x_itp;
        if (fabs(x_t - x_half) <= r) {
            x_itp = x_t;
        } else {
            x_itp = x_half - sigma * r;
        }

        /* Avaliar f no ponto ITP */
        double y_itp = f(x_itp, params) - T;
        result.f_evals++;

        double error = fabs(y_itp);

        /* Registrar histórico */
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

        /* Atualizar intervalo */
        if (y_itp > 0.0) {
            b = x_itp;
            yb = y_itp;
        } else if (y_itp < 0.0) {
            a = x_itp;
            ya = y_itp;
        } else {
            /* Encontrou raiz exata */
            a = x_itp;
            b = x_itp;
        }

        j++;
    }

    /* Resultado */
    result.x_star = (a + b) / 2.0;
    result.f_x_star = f(result.x_star, params);
    result.f_evals++;
    result.error = fabs(result.f_x_star - T);
    result.iterations = j;
    result.converged = ((b - a) <= 2.0 * eps) ? 1 : 0;

    return result;
}
