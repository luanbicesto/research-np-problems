/**
 * itp_method.h - Método ITP (Interpolate, Truncate, Project)
 *
 * Estado da arte em root-finding (Oliveira & Takahashi, 2021).
 * Combina convergência superlinear com pior caso ótimo (como bisseção).
 */

#ifndef ITP_METHOD_H
#define ITP_METHOD_H

#include "cbs_algorithm.h"  /* Para func_t, cbs_result_t, cbs_history_t */

/**
 * Busca por ITP method.
 *
 * Encontra x* em [a, b] tal que f(x*) ≈ T.
 * Requer: (f(a)-T) e (f(b)-T) com sinais opostos.
 *
 * Hiperparâmetros (valores padrão recomendados):
 *   kappa1 = 0.1
 *   kappa2 = 2.0  (∈ [1, 1+φ) onde φ = golden ratio)
 *   n0     = 1    (slack para pior caso)
 *
 * @param f         Função
 * @param params    Parâmetros de f
 * @param T         Valor alvo (busca f(x)=T, internamente resolve f(x)-T=0)
 * @param a, b      Intervalo de bracketing
 * @param eps       Precisão desejada
 * @param kappa1    Parâmetro de truncamento (> 0)
 * @param kappa2    Parâmetro de truncamento (∈ [1, 1+φ))
 * @param n0        Slack adicional para pior caso (≥ 0)
 * @param history   Se não-NULL, armazena histórico
 * @return          Resultado da busca
 */
cbs_result_t itp_search(
    func_t f, void *params,
    double T,
    double a, double b,
    double eps,
    double kappa1, double kappa2, int n0,
    cbs_history_t *history
);

#endif /* ITP_METHOD_H */
