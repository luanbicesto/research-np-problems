/**
 * cbs_algorithm.h - Busca Binária Contínua com Pivô pela Mediana
 *
 * Implementa busca binária no contínuo com dois métodos de pivô:
 * - Ponto médio (bisseção clássica)
 * - Mediana (divisão de massa)
 */

#ifndef CBS_ALGORITHM_H
#define CBS_ALGORITHM_H

#include <stddef.h>

/* Tipo para funções f: R -> R */
typedef double (*func_t)(double x, void *params);

/* Resultado de uma busca */
typedef struct {
    double x_star;          /* Solução encontrada */
    double f_x_star;        /* f(x*) */
    double error;           /* |f(x*) - T| */
    int iterations;         /* Número de iterações */
    int f_evals;            /* Número de avaliações de f */
    int converged;          /* 1 se convergiu, 0 caso contrário */
} cbs_result_t;

/* Histórico de uma iteração */
typedef struct {
    double a;               /* Extremo esquerdo */
    double b;               /* Extremo direito */
    double pivot;           /* Pivô escolhido */
    double f_pivot;         /* f(pivô) */
    double error;           /* |f(pivô) - T| */
    double interval_width;  /* b - a */
} cbs_iteration_t;

/* Histórico completo da busca */
typedef struct {
    cbs_iteration_t *entries;
    int count;
    int capacity;
} cbs_history_t;

/* Direção de monotonicidade */
typedef enum {
    MONOTONE_INCREASING = 1,
    MONOTONE_DECREASING = -1,
    MONOTONE_UNKNOWN = 0
} monotone_dir_t;

/* Métodos de pivô */
typedef enum {
    PIVOT_MIDPOINT,     /* Ponto médio: (a+b)/2 */
    PIVOT_MEDIAN,       /* Mediana: divide área de |f| ao meio */
    PIVOT_MEDIAN_INV    /* Mediana inversa: divide área de 1/|f-T| ao meio */
} pivot_method_t;

/**
 * Busca binária contínua.
 *
 * Encontra x* em [a, b] tal que f(x*) ≈ T.
 * Requer que f seja monótona no intervalo [a, b].
 *
 * @param f         Função a inverter
 * @param params    Parâmetros extras para f (pode ser NULL)
 * @param T         Valor alvo
 * @param a         Extremo esquerdo do intervalo
 * @param b         Extremo direito do intervalo
 * @param eps       Precisão desejada (|f(x*) - T| < eps)
 * @param max_iter  Número máximo de iterações
 * @param method    Método de pivô (PIVOT_MIDPOINT ou PIVOT_MEDIAN)
 * @param history   Se não-NULL, armazena histórico da busca
 * @return          Resultado da busca
 */
cbs_result_t cbs_search(
    func_t f, void *params,
    double T,
    double a, double b,
    double eps, int max_iter,
    pivot_method_t method,
    cbs_history_t *history
);

/**
 * Detecta monotonicidade de f no intervalo [a, b].
 * Usa amostragem em alguns pontos.
 */
monotone_dir_t cbs_detect_monotonicity(
    func_t f, void *params,
    double a, double b,
    int n_samples
);

/**
 * Seleciona ramo monótono para função unimodal.
 * Retorna o intervalo [*out_a, *out_b] onde f é monótona e contém solução.
 *
 * @return 1 se encontrou ramo, 0 caso contrário
 */
int cbs_select_branch(
    func_t f, void *params,
    double T,
    double a, double b,
    double *out_a, double *out_b,
    monotone_dir_t *out_dir
);

/* Gerenciamento de histórico */
cbs_history_t *cbs_history_create(int initial_capacity);
void cbs_history_free(cbs_history_t *h);

#endif /* CBS_ALGORITHM_H */
