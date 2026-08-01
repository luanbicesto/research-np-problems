/**
 * cbs_itp.h - Método Híbrido: CBS (Mediana 1/|f-T|) + ITP
 *
 * Estratégia: usa a varredura global (mediana de 1/|f-T|) para localizar
 * rapidamente a região da solução, e depois aplica ITP para convergência
 * superlinear com custo mínimo.
 *
 * Motivação: a mediana 1/|f-T| "enxerga" onde a solução está em ~1-2 iterações
 * (visão global), e o ITP refina com convergência superlinear (1 eval/iter).
 */

#ifndef CBS_ITP_H
#define CBS_ITP_H

#include "cbs_algorithm.h"

/**
 * Parâmetros do método híbrido CBS+ITP.
 */
typedef struct {
    /* Fase CBS (varredura global) */
    int n_quad;             /* Pontos de quadratura para mediana (default: 64) */
    int cbs_max_iters;     /* Máximo de iterações CBS antes de switch (default: 3) */
    double switch_ratio;   /* Trocar para ITP quando intervalo < ratio * original (default: 0.1) */

    /* Fase ITP */
    double kappa1;         /* Parâmetro ITP truncamento (default: 0.1) */
    double kappa2;         /* Parâmetro ITP truncamento (default: 2.0) */
    int n0;                /* Slack ITP (default: 1) */
} cbs_itp_params_t;

/* Parâmetros padrão */
#define CBS_ITP_DEFAULT { \
    .n_quad = 64,          \
    .cbs_max_iters = 3,    \
    .switch_ratio = 0.1,   \
    .kappa1 = 0.1,         \
    .kappa2 = 2.0,         \
    .n0 = 1                \
}

/**
 * Busca híbrida CBS+ITP.
 *
 * Fase 1 (CBS): Usa mediana de 1/|f(x)-T| para reduzir o intervalo rapidamente.
 *   - Gasta ~65 evals/iter mas localiza a região da solução em poucos passos.
 *   - Troca para ITP quando: iterações CBS >= cbs_max_iters OU intervalo reduzido.
 *
 * Fase 2 (ITP): Usa interpolação + truncamento + projeção.
 *   - 1 eval/iter, convergência superlinear com garantia de pior caso.
 *
 * @param f         Função
 * @param params    Parâmetros de f
 * @param T         Valor alvo
 * @param a, b      Intervalo (f monótona, (f(a)-T)*(f(b)-T) < 0)
 * @param eps       Precisão desejada
 * @param hp        Hiperparâmetros do método híbrido
 * @param history   Se não-NULL, armazena histórico
 * @return          Resultado da busca
 */
cbs_result_t cbs_itp_search(
    func_t f, void *params,
    double T,
    double a, double b,
    double eps,
    cbs_itp_params_t *hp,
    cbs_history_t *history
);

#endif /* CBS_ITP_H */
