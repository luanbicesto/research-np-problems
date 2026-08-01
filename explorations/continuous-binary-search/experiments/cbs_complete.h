/**
 * cbs_complete.h - Algoritmo Completo de Busca Binária no Contínuo
 *
 * Pipeline:
 *   1. Seleção de ramo monótono (para f não-monótona)
 *   2. CBS+ITP no ramo selecionado
 *
 * Aceita qualquer f contínua e qualquer T ∈ Im(f|[a,b]).
 * Não requer monotonicidade a priori — o algoritmo a encontra.
 */

#ifndef CBS_COMPLETE_H
#define CBS_COMPLETE_H

#include "cbs_algorithm.h"

/**
 * Resultado completo incluindo info de seleção de ramo.
 */
typedef struct {
    cbs_result_t search_result;     /* Resultado da busca CBS+ITP */
    double branch_a;                /* Extremo esquerdo do ramo selecionado */
    double branch_b;                /* Extremo direito do ramo selecionado */
    monotone_dir_t branch_dir;      /* Direção de monotonicidade do ramo */
    int branch_evals;               /* Avaliações gastas na seleção de ramo */
    int total_evals;                /* Avaliações totais (ramo + busca) */
} cbs_complete_result_t;

/**
 * Busca binária contínua completa.
 *
 * Dado f: [a,b] → R contínua e T ∈ Im(f), encontra x* com f(x*) = T.
 * NÃO requer que f seja monótona — seleciona automaticamente um ramo.
 * Aceita qualquer x* (se houver múltiplas soluções, retorna uma).
 *
 * Estratégia de seleção de ramo:
 *   - Se f já é monótona em [a,b]: usa direto
 *   - Se f é unimodal: busca ternária para achar extremo, escolhe ramo
 *   - Se f tem múltiplos extremos: amostra para encontrar ramo válido
 *
 * @param f         Função contínua
 * @param params    Parâmetros de f
 * @param T         Valor alvo
 * @param a, b      Intervalo de busca
 * @param eps       Precisão desejada
 * @return          Resultado completo (ramo + busca)
 */
cbs_complete_result_t cbs_complete_search(
    func_t f, void *params,
    double T,
    double a, double b,
    double eps
);

#endif /* CBS_COMPLETE_H */
