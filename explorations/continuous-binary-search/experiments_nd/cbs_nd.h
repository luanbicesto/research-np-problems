/**
 * cbs_nd.h — Continuous Binary Search em R^n
 *
 * Generalização do CBS-1D para funções f: R^n → R.
 * Encontra x* ∈ R^n tal que f(x*) = T.
 *
 * Variante CBS-Box: domínio = hiper-retângulo, corte coordenada-a-coordenada.
 * Complexidade: O(n² log(R/ε)) avaliações de f para f convexa.
 *
 * Pré-condições:
 *   - f contínua no domínio
 *   - T ∈ (min_D f, max_D f) (existência de solução)
 *   - Para convergência garantida: f convexa ou monótona por coordenada
 */

#ifndef CBS_ND_H
#define CBS_ND_H

#include <stddef.h>

/* Tipo de função n-dimensional: recebe vetor x de dimensão n */
typedef double (*func_nd_t)(const double *x, int n, void *params);

/* Status de convergência */
typedef enum {
    CBS_ND_OK = 0,              /* Convergiu com |f(x*)-T| < eps */
    CBS_ND_MAX_ITER,            /* Máximo de iterações atingido */
    CBS_ND_NO_SOLUTION,         /* T fora do range de f no domínio */
    CBS_ND_NUMERICAL_ERROR,     /* Gradiente nulo ou NaN detectado */
    CBS_ND_DEGENERATE_DOMAIN    /* Domínio colapsou (bounds[i][0] ≈ bounds[i][1]) */
} cbs_nd_status_t;

/* Método de seleção de direção de corte */
typedef enum {
    CUT_MAX_GRADIENT = 0,       /* Corta na coordenada de maior |∂f/∂xᵢ| */
    CUT_CYCLIC,                 /* Corta ciclicamente (round-robin) */
    CUT_MAX_WIDTH               /* Corta na coordenada de maior largura */
} cbs_nd_cut_method_t;

/* Parâmetros configuráveis */
typedef struct {
    int max_iter;               /* Máximo de iterações (default: 1000) */
    double grad_h_factor;       /* h = width_i * grad_h_factor (default: 1e-7) */
    cbs_nd_cut_method_t cut_method;  /* Método de corte (default: MAX_GRADIENT) */
    int detect_convexity;       /* Testar convexidade? (default: 0) */
    int verbose;                /* Imprimir progresso? (default: 0) */
} cbs_nd_params_t;

/* Resultado da busca */
typedef struct {
    double *x_star;             /* Solução encontrada (n doubles, alocado internamente) */
    double f_value;             /* f(x*) */
    double error;               /* |f(x*) - T| */
    int total_evals;            /* Total de avaliações de f */
    int n_iterations;           /* Número de iterações do loop principal */
    int converged;              /* 1 se convergiu, 0 caso contrário */
    cbs_nd_status_t status;     /* Status detalhado */
    int cut_coord_histogram[];  /* (flexible array) cortes por coordenada — NÃO USAR */
} cbs_nd_result_t;

/* Resultado (versão com diagnóstico) */
typedef struct {
    double *x_star;             /* Solução (n doubles, alocado internamente — chamar cbs_nd_free) */
    double f_value;             /* f(x*) */
    double error;               /* |f(x*) - T| */
    int total_evals;            /* Total de avaliações de f */
    int n_iterations;           /* Iterações do loop */
    int converged;              /* 1 se |f(x*)-T| < eps */
    cbs_nd_status_t status;     /* Código de status */
    int *cuts_per_coord;        /* Cortes feitos em cada coordenada (n ints, alocado) */
} cbs_nd_result_full_t;

/* Parâmetros default */
#define CBS_ND_DEFAULT_PARAMS { \
    .max_iter = 1000,           \
    .grad_h_factor = 1e-7,     \
    .cut_method = CUT_MAX_GRADIENT, \
    .detect_convexity = 0,     \
    .verbose = 0               \
}

/**
 * CBS-nD Box: busca x* com f(x*) = T em hiper-retângulo.
 *
 * @param f         Função objetivo f: R^n → R
 * @param params    Parâmetros opacos passados a f
 * @param n         Dimensão do espaço
 * @param T         Valor alvo
 * @param lower     Limites inferiores do domínio (n doubles)
 * @param upper     Limites superiores do domínio (n doubles)
 * @param eps       Precisão desejada: |f(x*)-T| < eps
 * @param cfg       Configuração (NULL para defaults)
 * @return          Resultado (chamar cbs_nd_free após uso)
 */
cbs_nd_result_full_t cbs_nd_search(
    func_nd_t f, void *params,
    int n, double T,
    const double *lower, const double *upper,
    double eps,
    const cbs_nd_params_t *cfg
);

/**
 * Liberar memória alocada pelo resultado.
 */
void cbs_nd_free(cbs_nd_result_full_t *result);

#endif /* CBS_ND_H */
