/**
 * cbs_complete.c - Algoritmo Completo: Seleção de Ramo + CBS+ITP
 *
 * Resolve: dado f contínua (não necessariamente monótona) e T ∈ Im(f|[a,b]),
 * encontrar qualquer x* com f(x*) = T.
 *
 * Pipeline:
 *   Step 1: Detectar se f é monótona no intervalo
 *   Step 2: Se não é, encontrar um sub-intervalo monótono que contenha solução
 *   Step 3: Aplicar CBS+ITP no ramo monótono
 */

#include "cbs_complete.h"
#include "cbs_itp.h"
#include "itp_method.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

/* Número de amostras para detecção de monotonicidade e localização de ramo */
#define DETECT_SAMPLES 32

/**
 * Encontra um sub-intervalo [out_a, out_b] onde:
 *   - f é monótona
 *   - (f(out_a) - T) * (f(out_b) - T) < 0 (solução existe no sub-intervalo)
 *
 * Estratégia: amostra f em N pontos, encontra pares consecutivos onde
 * f cruza T (mudança de sinal de f-T), e retorna o primeiro encontrado.
 *
 * Retorna: 1 se encontrou ramo, 0 caso contrário.
 */
static int find_monotone_bracket(
    func_t f, void *params,
    double T,
    double a, double b,
    int n_samples,
    double *out_a, double *out_b,
    monotone_dir_t *out_dir,
    int *evals_used)
{
    int evals = 0;

    /* Amostrar f - T em pontos equidistantes */
    double *samples = malloc(sizeof(double) * (n_samples + 1));
    double *x_pts = malloc(sizeof(double) * (n_samples + 1));
    double dx = (b - a) / n_samples;

    for (int i = 0; i <= n_samples; i++) {
        x_pts[i] = a + i * dx;
        samples[i] = f(x_pts[i], params) - T;
        evals++;
    }

    /* Encontrar primeiro par onde f-T muda de sinal (bracketing) */
    int found = 0;
    for (int i = 0; i < n_samples; i++) {
        if (samples[i] * samples[i + 1] < 0.0) {
            /* f cruza T entre x_pts[i] e x_pts[i+1] */
            *out_a = x_pts[i];
            *out_b = x_pts[i + 1];

            if (samples[i] < 0.0 && samples[i + 1] > 0.0) {
                *out_dir = MONOTONE_INCREASING;
            } else {
                *out_dir = MONOTONE_DECREASING;
            }
            found = 1;
            break;
        }
        /* Caso exato: f(x_i) = T */
        if (fabs(samples[i]) < 1e-15) {
            *out_a = x_pts[i];
            *out_b = x_pts[i];
            *out_dir = MONOTONE_INCREASING; /* irrelevante, já achamos */
            found = 2; /* flag: solução exata */
            break;
        }
    }

    free(samples);
    free(x_pts);
    *evals_used = evals;
    return found;
}

/**
 * Seleção de ramo para funções unimodais.
 * Usa busca ternária para achar extremo, depois escolhe ramo com solução.
 */
static int select_branch_unimodal(
    func_t f, void *params,
    double T,
    double a, double b,
    double *out_a, double *out_b,
    monotone_dir_t *out_dir,
    int *evals_used)
{
    int evals = 0;

    /* Busca ternária para encontrar máximo (50 iterações = alta precisão) */
    double lo = a, hi = b;
    for (int i = 0; i < 50; i++) {
        double m1 = lo + (hi - lo) / 3.0;
        double m2 = hi - (hi - lo) / 3.0;
        double f1 = f(m1, params);
        double f2 = f(m2, params);
        evals += 2;
        if (f1 < f2) {
            lo = m1;
        } else {
            hi = m2;
        }
    }
    double x_max = (lo + hi) / 2.0;
    double f_max = f(x_max, params);
    evals++;

    /* Verificar se T é alcançável */
    if (T > f_max) {
        /* Talvez f tem mínimo, não máximo — tentar buscar mínimo */
        lo = a; hi = b;
        for (int i = 0; i < 50; i++) {
            double m1 = lo + (hi - lo) / 3.0;
            double m2 = hi - (hi - lo) / 3.0;
            double f1 = f(m1, params);
            double f2 = f(m2, params);
            evals += 2;
            if (f1 > f2) {
                lo = m1;
            } else {
                hi = m2;
            }
        }
        double x_min = (lo + hi) / 2.0;
        double f_min = f(x_min, params);
        evals++;

        if (T < f_min) {
            *evals_used = evals;
            return 0; /* T fora do range de f */
        }

        /* Ramos invertidos (f tem mínimo) */
        double fa = f(a, params); evals++;
        double fb = f(b, params); evals++;

        if (fa >= T && T >= f_min) {
            *out_a = a; *out_b = x_min;
            *out_dir = MONOTONE_DECREASING;
            *evals_used = evals;
            return 1;
        }
        if (fb >= T && T >= f_min) {
            *out_a = x_min; *out_b = b;
            *out_dir = MONOTONE_INCREASING;
            *evals_used = evals;
            return 1;
        }
        *evals_used = evals;
        return 0;
    }

    /* Caso padrão: f tem máximo */
    double fa = f(a, params); evals++;
    double fb = f(b, params); evals++;

    /* Ramo esquerdo [a, x_max]: crescente (fa → f_max) */
    if (fa <= T && T <= f_max) {
        *out_a = a;
        *out_b = x_max;
        *out_dir = MONOTONE_INCREASING;
        *evals_used = evals;
        return 1;
    }

    /* Ramo direito [x_max, b]: decrescente (f_max → fb) */
    if (fb <= T && T <= f_max) {
        *out_a = x_max;
        *out_b = b;
        *out_dir = MONOTONE_DECREASING;
        *evals_used = evals;
        return 1;
    }

    *evals_used = evals;
    return 0;
}

/* ===== Algoritmo Completo ===== */

cbs_complete_result_t cbs_complete_search(
    func_t f, void *params,
    double T,
    double a, double b,
    double eps)
{
    cbs_complete_result_t cr;
    memset(&cr, 0, sizeof(cr));

    int branch_evals = 0;

    /* --- Step 1: Verificar se f já é monótona no intervalo --- */
    double fa = f(a, params) - T;
    double fb = f(b, params) - T;
    branch_evals += 2;

    /* Se já temos bracketing direto: (fa e fb com sinais opostos) */
    if (fa * fb < 0.0) {
        /* Testar monotonicidade */
        monotone_dir_t dir = cbs_detect_monotonicity(f, params, a, b, 8);
        branch_evals += 8;

        if (dir != MONOTONE_UNKNOWN) {
            /* f é monótona e temos bracketing — vamos direto para CBS+ITP */
            cr.branch_a = a;
            cr.branch_b = b;
            cr.branch_dir = dir;
            cr.branch_evals = branch_evals;
            goto do_search;
        }

        /* Não é monótona mas temos bracketing — usar o intervalo todo.
         * Podemos tentar achar um sub-intervalo menor onde é monótona. */
        int sub_evals = 0;
        double sub_a, sub_b;
        monotone_dir_t sub_dir;
        int found = find_monotone_bracket(f, params, T, a, b,
                                          DETECT_SAMPLES, &sub_a, &sub_b,
                                          &sub_dir, &sub_evals);
        branch_evals += sub_evals;

        if (found == 2) {
            /* Solução exata encontrada na amostragem */
            cr.branch_a = sub_a;
            cr.branch_b = sub_a;
            cr.branch_evals = branch_evals;
            cr.search_result.x_star = sub_a;
            cr.search_result.f_x_star = T;
            cr.search_result.error = 0.0;
            cr.search_result.iterations = 0;
            cr.search_result.f_evals = branch_evals;
            cr.search_result.converged = 1;
            cr.total_evals = branch_evals;
            return cr;
        }

        if (found == 1) {
            cr.branch_a = sub_a;
            cr.branch_b = sub_b;
            cr.branch_dir = sub_dir;
            cr.branch_evals = branch_evals;
            goto do_search;
        }

        /* Fallback: usar intervalo original com direção inferida dos extremos */
        cr.branch_a = a;
        cr.branch_b = b;
        cr.branch_dir = (fa < fb) ? MONOTONE_INCREASING : MONOTONE_DECREASING;
        cr.branch_evals = branch_evals;
        goto do_search;
    }

    /* --- Step 2: Não temos bracketing direto — f não é monótona ou T não está entre f(a) e f(b) --- */

    /* Tentar seleção unimodal (busca ternária) */
    {
        double br_a, br_b;
        monotone_dir_t br_dir;
        int uni_evals = 0;
        int found = select_branch_unimodal(f, params, T, a, b,
                                           &br_a, &br_b, &br_dir, &uni_evals);
        branch_evals += uni_evals;

        if (found) {
            cr.branch_a = br_a;
            cr.branch_b = br_b;
            cr.branch_dir = br_dir;
            cr.branch_evals = branch_evals;
            goto do_search;
        }
    }

    /* Tentar amostragem direta para encontrar bracket */
    {
        double sub_a, sub_b;
        monotone_dir_t sub_dir;
        int sub_evals = 0;
        int found = find_monotone_bracket(f, params, T, a, b,
                                          DETECT_SAMPLES * 4,
                                          &sub_a, &sub_b, &sub_dir, &sub_evals);
        branch_evals += sub_evals;

        if (found >= 1) {
            cr.branch_a = sub_a;
            cr.branch_b = sub_b;
            cr.branch_dir = sub_dir;
            cr.branch_evals = branch_evals;
            goto do_search;
        }
    }

    /* Falha: não conseguiu encontrar ramo com solução */
    cr.branch_evals = branch_evals;
    cr.search_result.converged = 0;
    cr.search_result.error = INFINITY;
    cr.total_evals = branch_evals;
    return cr;

do_search:
    /* --- Step 3: CBS+ITP no ramo selecionado --- */
    {
        double branch_width = cr.branch_b - cr.branch_a;

        if (branch_width < eps) {
            /* Bracket já é menor que eps — solução encontrada */
            cr.search_result.x_star = (cr.branch_a + cr.branch_b) / 2.0;
            cr.search_result.f_x_star = f(cr.search_result.x_star, params);
            cr.search_result.error = fabs(cr.search_result.f_x_star - T);
            cr.search_result.f_evals = 1;
            cr.search_result.iterations = 0;
            cr.search_result.converged = 1;
        } else if (branch_width < 1.0) {
            /* Bracket estreito — pular fase CBS, usar ITP direto */
            cr.search_result = itp_search(
                f, params, T,
                cr.branch_a, cr.branch_b, eps,
                0.1, 2.0, 1, NULL
            );
        } else {
            /* Bracket largo — usar CBS+ITP completo */
            cbs_itp_params_t hp = CBS_ITP_DEFAULT;
            cr.search_result = cbs_itp_search(
                f, params, T,
                cr.branch_a, cr.branch_b, eps,
                &hp, NULL
            );
        }
        cr.total_evals = cr.branch_evals + cr.search_result.f_evals;
    }

    return cr;
}
