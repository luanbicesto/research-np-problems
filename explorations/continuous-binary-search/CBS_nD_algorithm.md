# CBS-nD: Algoritmo de Busca Binária Contínua em n Dimensões

**Data:** 2026-08-01  
**Status:** Design completo, pronto para implementação  
**Base:** Formalização CBS ↔ Cutting Plane (CBS_as_cutting_plane.md, Seção 6-7)

---

## 1. Especificação

**Input:**
- `f: R^n → R` — função convexa, acesso black-box (somente avaliações)
- `T` — valor alvo (target)
- `D = [a₁,b₁] × ... × [aₙ,bₙ]` — domínio (hiperretângulo)
- `ε > 0` — tolerância desejada: queremos `|f(x*) - T| < ε`

**Output:**
- `x* ∈ D` com `|f(x*) - T| < ε`

**Hipóteses:**
- f é convexa em D
- T ∈ Im(f|_D) (existe solução)
- ∇f(x) ≠ 0 para todo x ∈ D (gradiente não-degenerado)
- Gradiente não disponível explicitamente — estimado por diferenças finitas

---

## 2. Pseudocódigo Principal (CBS-nD Genérico)

```
ALGORITHM CBS_nD(f, T, D, ε, δ, max_iter)

INPUT:
  f        : R^n → R (black-box, convexa)
  T        : R (valor alvo)
  D        : domínio inicial (representação depende da variante)
  ε        : tolerância |f(x*) - T| < ε
  δ        : regularização para peso (default: ε/10)
  max_iter : limite de iterações (default: 10 * n² * ceil(log(diam(D)/ε)))

OUTPUT:
  x* aproximado, f(x*), erro, contagem de avaliações

INITIALIZE:
  K₀ ← D                           // corpo convexo corrente
  k ← 0
  best_x ← centro(K₀)
  best_err ← |f(best_x) - T|
  total_evals ← 0

LOOP while k < max_iter:
  // 1. COMPUTAR PONTO DE QUERY (centróide ponderado ou proxy)
  c_k ← compute_query_point(K_k, f, T, δ)
  
  // 2. AVALIAR FUNÇÃO
  f_c ← f(c_k)
  total_evals += 1
  
  // 3. VERIFICAR CONVERGÊNCIA
  err ← |f_c - T|
  if err < best_err:
    best_x ← c_k
    best_err ← err
  if err < ε:
    RETURN (c_k, f_c, err, total_evals, CONVERGED)
  
  // 4. ESTIMAR GRADIENTE (diferenças finitas)
  g_k ← estimate_gradient(f, c_k, n, h)    // h = step para diferenças finitas
  total_evals += 2*n                         // forward differences: 2n evals
  
  // 5. VERIFICAR CONVEXIDADE (opcional, periódico)
  if k % check_interval == 0:
    if NOT convexity_check(f, K_k, c_k, g_k):
      WARN "f pode não ser convexa — resultado não garantido"
  
  // 6. CONSTRUIR HIPERPLANO DE CORTE
  //    H = {x : g_k · (x - c_k) = 0}
  //    Semi-espaço retido: depende do sinal de f(c_k) - T
  if f_c > T:
    // Pela convexidade: x* está em {x : g_k · (x - c_k) ≤ 0}
    halfspace ← {x : g_k · (x - c_k) ≤ 0}
  else:  // f_c < T
    halfspace ← {x : g_k · (x - c_k) ≥ 0}
  
  // 7. ATUALIZAR DOMÍNIO
  K_{k+1} ← intersect(K_k, halfspace)
  
  // 8. VERIFICAR DEGENERESCÊNCIA
  if volume(K_{k+1}) ≈ 0 or is_empty(K_{k+1}):
    RETURN (best_x, f(best_x), best_err, total_evals, DEGENERATE)
  
  k ← k + 1

RETURN (best_x, f(best_x), best_err, total_evals, MAX_ITER_REACHED)
```

### 2.1. Estimação de Gradiente por Diferenças Finitas

```
FUNCTION estimate_gradient(f, x, n, h)
  // Diferenças centrais (mais preciso, 2n avaliações)
  g ← vetor de n componentes
  for i = 1 to n:
    e_i ← vetor canônico i-ésimo
    g[i] ← (f(x + h*e_i) - f(x - h*e_i)) / (2*h)
  RETURN g

  // Alternativa: diferenças forward (n avaliações extras)
  // f_x ← f(x)  // já disponível
  // g[i] ← (f(x + h*e_i) - f_x) / h
```

**Escolha de h:** `h = max(ε^(1/3) * max(|x_i|, 1), δ/10)` — balanço entre erro de truncamento O(h²) e erro numérico O(ε_mach/h).

---

## 3. Estruturas de Dados

### 3.1. Representação do Domínio

Três opções, correspondendo às três variantes:

| Representação | Armazenamento | Interseção com semi-espaço | Centróide |
|---|---|---|---|
| **Elipsóide** | Centro c ∈ R^n + Matriz P ∈ R^{n×n} | O(n²) — update de Löwner-John | O(1) — é o centro |
| **Hiperretângulo (Box)** | 2n doubles (a_i, b_i) | O(n) — corte coordenada-a-coordenada | O(n) — média dos extremos |
| **Politopo (H-rep)** | m hiperplanos (A ∈ R^{m×n}, b ∈ R^m) | O(1) — adicionar hiperplano | O(n^4) ou MCMC |

**Recomendação para implementação inicial:** Elipsóide (trade-off ótimo entre simplicidade e convergência).

### 3.2. Centróide Ponderado em R^n

O centróide ponderado exato requer integração sobre K_k:
```
c_w(K_k) = ∫_{K_k} x · w(x) dx / ∫_{K_k} w(x) dx
```

**Métodos práticos (em ordem de complexidade):**

1. **Centro do elipsóide** (Variante A): O(1) — é simplesmente `c_k = center(E_k)`
2. **Centro do box** (Variante B): O(n) — média `(a_i + b_i)/2`
3. **Monte Carlo**: amostra M pontos uniformes em K_k, computa média ponderada
   ```
   c_w ≈ Σ x_j · w(x_j) / Σ w(x_j),  x_j ~ Uniform(K_k)
   ```
   Custo: M avaliações de f. Recomendado M = O(n²) para precisão adequada.
4. **Centróide analítico** (para politopo): resolver min_{x ∈ K} Σ -log(b_i - a_i·x)
   — requer solver de programação convexa interno.

### 3.3. Escolha da Direção do Corte

A direção do corte é **sempre** `g_k = ∇f(c_k)` (estimado). Isso garante:
- O hiperplano é tangente à curva de nível de f em c_k
- Pela convexidade, o semi-espaço oposto ao gradiente contém toda a sublevel set {f ≤ f(c_k)}
- É a direção de máxima informação (corta o máximo de volume "impossível")

**Normalização:** O hiperplano é `g_k · (x - c_k) = 0`. Para estabilidade numérica, normalizar g_k: `g_k ← g_k / ‖g_k‖`.

**Fallback:** Se `‖g_k‖ < tol` (gradiente quase nulo), estamos perto de um minimizador. Se f(c_k) ≈ T, convergimos. Se não, usar direção de maior variação (eigenvector principal de K_k).

---

## 4. Variantes

### 4A. CBS-Elipsóide (Domínio = Elipsóide, Update = Löwner-John)

**Representação:** Elipsóide E = {x : (x-c)ᵀ P⁻¹ (x-c) ≤ 1}, armazenado como (c, P) onde P ∈ R^{n×n} simétrica definida positiva.

```
ALGORITHM CBS_Ellipsoid(f, T, box, ε, δ, max_iter)

INITIALIZE:
  // Elipsóide mínimo contendo o box [a₁,b₁]×...×[aₙ,bₙ]
  c ← ((a₁+b₁)/2, ..., (aₙ+bₙ)/2)
  P ← diag(((b₁-a₁)/2)², ..., ((bₙ-aₙ)/2)²)
  
LOOP while k < max_iter:
  // Query no centro do elipsóide
  f_c ← f(c)
  if |f_c - T| < ε: RETURN c
  
  // Estimar gradiente
  g ← estimate_gradient(f, c, n, h)
  g_norm ← g / ‖g‖
  
  // Determinar semi-espaço retido
  if f_c > T:
    // Reter {x : gᵀ(x-c) ≤ 0}, i.e., corte a = g_norm
    a ← g_norm
  else:
    // Reter {x : gᵀ(x-c) ≥ 0}, i.e., corte a = -g_norm
    a ← -g_norm
  
  // UPDATE DE LÖWNER-JOHN (fórmula do elipsóide)
  // Novo elipsóide = mínimo elipsóide contendo E ∩ {x : aᵀ(x-c) ≤ 0}
  Pa ← P · a
  aPa ← aᵀ · Pa             // escalar: aᵀ P a
  
  // Atualizar centro
  c_new ← c - (1/(n+1)) · Pa / sqrt(aPa)
  
  // Atualizar matriz de forma
  P_new ← (n²/(n²-1)) · (P - (2/(n+1)) · (Pa · Paᵀ) / aPa)
  
  c ← c_new
  P ← P_new
  k ← k + 1

RETURN (best_x, best_err, total_evals)
```

**Propriedades:**
- Custo por iteração: O(n²) para o update de P
- Redução de volume: vol(E_{k+1})/vol(E_k) ≤ e^{-1/(2(n+1))}
- Iterações para ε: O(n² · ln(diam(D)/ε))
- Não precisa armazenar faces/hiperplanos — apenas (c, P)
- **Limitação:** O centro do elipsóide NÃO é o centróide ponderado por w — é uma aproximação. Perdemos a aceleração por concentração.

### 4B. CBS-Box (Domínio = Hiperretângulo, Corte Coordenada-a-Coordenada)

**Representação:** Box B = [a₁,b₁] × ... × [aₙ,bₙ], armazenado como 2n doubles.

```
ALGORITHM CBS_Box(f, T, box, ε, δ, max_iter)

INITIALIZE:
  a[1..n], b[1..n] ← extremos do box
  
LOOP while k < max_iter:
  // Query no centro do box
  c ← ((a[1]+b[1])/2, ..., (a[n]+b[n])/2)
  f_c ← f(c)
  if |f_c - T| < ε: RETURN c
  
  // Estimar gradiente
  g ← estimate_gradient(f, c, n, h)
  
  // ESTRATÉGIA: cortar na coordenada de maior componente do gradiente
  i* ← argmax_i |g[i]|
  
  // Corte ao longo de coordenada i*
  if (f_c > T AND g[i*] > 0) OR (f_c < T AND g[i*] < 0):
    // x* tem coordenada i* menor que c[i*]
    b[i*] ← c[i*]
  else:
    // x* tem coordenada i* maior que c[i*]
    a[i*] ← c[i*]
  
  k ← k + 1

RETURN (best_x, best_err, total_evals)
```

**Variante cíclica (mais robusta):**
```
  // Em vez de argmax|g[i]|, ciclar pelas coordenadas:
  i* ← (k mod n) + 1
  // Garante que todas coordenadas são cortadas a cada n iterações
```

**Variante com mediana ponderada (CBS-Box com w):**
```
  // Para cada coordenada i*, em vez de cortar no centro:
  // 1. Fixar todas coordenadas exceto i* no centro
  // 2. Computar peso w(t) = 1/(|f(c₁,...,t,...,cₙ) - T| + δ) ao longo de t ∈ [a[i*], b[i*]]
  // 3. Encontrar mediana de w (reuso do CBS-1D!)
  // Custo: ~20 avaliações extras por iteração (quadratura para mediana)
```

**Propriedades:**
- Custo por iteração: O(n) + custo do gradiente O(n)
- Redução de volume: exatamente 1/2 por iteração (corte pelo centro numa coordenada)
- Iterações para ε: O(n · log₂(diam(D)/ε)) — precisa n cortes para reduzir todas dimensões
- **Vantagem:** Implementação trivial, sem álgebra linear
- **Limitação:** Convergência mais lenta que elipsóide para n grande (corte só alinhado a eixos)

### 4C. CBS-Simplex (Domínio = Simplex, Corte pelo Centróide)

**Representação:** Simplex S com n+1 vértices v₀, v₁, ..., vₙ ∈ R^n.

```
ALGORITHM CBS_Simplex(f, T, vertices, ε, δ, max_iter)

INITIALIZE:
  V ← {v₀, ..., vₙ}  // n+1 vértices do simplex
  // Simplex inicial: enclosar o box D num simplex regular
  
LOOP while k < max_iter:
  // Centróide do simplex
  c ← (1/(n+1)) · Σᵢ vᵢ
  f_c ← f(c)
  if |f_c - T| < ε: RETURN c
  
  // Estimar gradiente
  g ← estimate_gradient(f, c, n, h)
  g_norm ← g / ‖g‖
  
  // Determinar semi-espaço retido
  if f_c > T:
    // Reter {x : gᵀ(x-c) ≤ 0}
    // Substituir o vértice mais distante no lado errado
    i_drop ← argmax_i { gᵀ(vᵢ - c) }  // vértice mais "acima" do corte
  else:
    i_drop ← argmin_i { gᵀ(vᵢ - c) }  // vértice mais "abaixo" do corte
  
  // Substituir vértice: projetar no hiperplano de corte
  // Novo vértice = interseção da aresta (v_drop → c) com o hiperplano
  // Simplex_{k+1} tem vértices: V \ {v_{i_drop}} ∪ {c}
  V[i_drop] ← c   // substituir pelo centróide (Nelder-Mead-like)
  
  k ← k + 1

RETURN (best_x, best_err, total_evals)
```

**Propriedades:**
- Custo por iteração: O(n²) para computar centróide e selecionar vértice
- Redução de volume: vol(S_{k+1})/vol(S_k) = (n/(n+1))^n ≈ 1/e ≈ 0.368
- Iterações para ε: O(n · ln(diam(D)/ε))
- **Vantagem:** Bound de Grünbaum automático (centróide do simplex)
- **Limitação:** O simplex pode degenerar (ficar "fino") — precisa restart periódico

---

## 5. Análise de Complexidade

### 5.1. Tabela Comparativa

| Variante | Iterações | Custo/iter (ops) | Avaliações de f/iter | Total evals | Memória |
|----------|-----------|------------------|---------------------|-------------|---------|
| **CBS-Elipsóide** | O(n² ln(R/ε)) | O(n²) | 1 + 2n (grad) | O(n³ ln(R/ε)) | O(n²) |
| **CBS-Box** | O(n log₂(R/ε)) | O(n) | 1 + 2n (grad) | O(n² log(R/ε)) | O(n) |
| **CBS-Simplex** | O(n ln(R/ε)) | O(n²) | 1 + 2n (grad) | O(n² ln(R/ε)) | O(n²) |

Onde R = diam(D) / ε (razão de condicionamento geométrico).

### 5.2. Detalhamento por Variante

**CBS-Elipsóide:**
- Redução de volume por iteração: `exp(-1/(2(n+1)))`
- Para vol(E_k) ≤ (ε/R)^n · vol(E_0):
  - k ≥ 2n(n+1) · ln(R/ε)
  - = O(n² · ln(R/ε))
- Custo total de operações aritméticas: O(n⁴ · ln(R/ε))
- Custo total de avaliações de f: O(n³ · ln(R/ε))

**CBS-Box:**
- Redução de volume por iteração: exatamente 1/2 (uma coordenada)
- Cada coordenada é cortada a cada n iterações → todas reduzem em n rounds
- Para max_i(b_i - a_i) ≤ ε/λ_max:
  - k ≥ n · log₂(max_i(b_i - a_i) · λ_max / ε)
  - = O(n · log(R/ε))
- Custo total de avaliações: O(n² · log(R/ε))
- **Mais eficiente em avaliações para n moderado**

**CBS-Simplex:**
- Redução de volume por iteração: ≤ (n/(n+1))^n ≈ 1/e
- Para vol(S_k) ≤ (ε/R)^n · vol(S_0):
  - k ≥ n · n · ln(R/ε) / ln((n+1)/n) ≈ n² · ln(R/ε)
  - Hmm, mais precisamente: vol reduz por fator ~1/e, preciso (ε/R)^n, logo k = O(n · ln(R/ε))
- Custo total de avaliações: O(n² · ln(R/ε))
- Comparable ao Box mas com melhor constante geométrica

### 5.3. Custo do Gradiente por Diferenças Finitas

- Diferenças centrais: **2n** avaliações de f por iteração
- Diferenças forward: **n** avaliações extras (reusa f(c_k))
- Para n grande, o gradiente domina o custo

**Otimização: gradient-free directions**
- Em vez de estimar ∇f completo, usar **1 direção aleatória** por iteração
- Corte por `(f(c + h·d) - f(c)) · d` onde d ~ N(0,I) normalizado
- Reduz custo para O(1) avaliações extras/iter, mas convergência degrada para O(n³ ln(R/ε))

### 5.4. Comparação com Métodos Existentes

| Método | Hipóteses | Iterações | Evals/iter | Total evals |
|--------|-----------|-----------|-----------|-------------|
| CBS-Elipsóide | f convexa | O(n² ln(R/ε)) | 2n+1 | O(n³ ln(R/ε)) |
| CBS-Box | f convexa | O(n log(R/ε)) | 2n+1 | O(n² log(R/ε)) |
| Nelder-Mead | f contínua | ??? (sem garantia) | 1-2 | ??? |
| BFGS (sem grad) | f C², Lipschitz | O(n ln(1/ε)) | 2n+1 | O(n² ln(1/ε)) |
| Elipsóide clássico | LP/convexa, grad | O(n² ln(R/ε)) | 1 | O(n² ln(R/ε)) |

**Nota:** CBS-Box é competitivo com BFGS em número de avaliações, mas tem garantias mais fortes (convergência global sem line search, funciona sem Hessiana).

---

## 6. Detecção de Convexidade On-the-Fly

A convexidade de f é **essencial** para a validade do oráculo (Proposição 6.2 da formalização). Se f não é convexa, o corte pode descartar a solução. Precisamos verificar durante a execução.

### 6.1. Testes de Convexidade

**Teste 1: Desigualdade de Jensen (probabilístico)**
```
FUNCTION check_convexity_jensen(f, K_k, n_samples, n_tests)
  violations ← 0
  for t = 1 to n_tests:
    // Amostrar 2 pontos em K_k
    x ← random_point_in(K_k)
    y ← random_point_in(K_k)
    λ ← random_uniform(0.1, 0.9)
    
    // Verificar f(λx + (1-λ)y) ≤ λf(x) + (1-λ)f(y)
    z ← λ*x + (1-λ)*y
    lhs ← f(z)
    rhs ← λ*f(x) + (1-λ)*f(y)
    
    if lhs > rhs + tolerance:
      violations += 1
  
  RETURN (violations == 0), violations/n_tests
```
Custo: 3 · n_tests avaliações de f.

**Teste 2: Hessiana semi-definida positiva (amostrada)**
```
FUNCTION check_convexity_hessian(f, x, n, h)
  // Estimar Hessiana por diferenças finitas de segunda ordem
  H ← matrix(n, n)
  f_x ← f(x)
  for i = 1 to n:
    for j = i to n:
      if i == j:
        H[i][i] ← (f(x + h*eᵢ) - 2*f_x + f(x - h*eᵢ)) / h²
      else:
        H[i][j] ← (f(x+h*eᵢ+h*eⱼ) - f(x+h*eᵢ) - f(x+h*eⱼ) + f_x) / h²
        H[j][i] ← H[i][j]
  
  // Verificar se H é semi-definida positiva (menor autovalor ≥ -tol)
  λ_min ← min_eigenvalue(H)
  RETURN (λ_min ≥ -tolerance), λ_min
```
Custo: O(n²) avaliações de f + O(n³) para autovalores.

**Teste 3: Consistência do oráculo (retrospectivo)**
```
FUNCTION check_oracle_consistency(history, f, T)
  // Verificar que pontos descartados realmente não contêm a solução
  // Amostra pontos no lado descartado e verifica se f está longe de T
  for each cut (c_k, g_k, side) in history:
    sample points in discarded halfspace
    if any point has |f(p) - T| < current_best_error:
      RETURN FALSE  // oráculo provavelmente errou — f não é convexa aqui
  RETURN TRUE
```

### 6.2. Estratégia de Verificação Adaptativa

```
SCHEDULE DE VERIFICAÇÃO:
  - Iterações 1-5: verificar SEMPRE (Jensen + Hessiana no ponto)
  - Iterações 5-20: verificar a cada 5 iterações (Jensen)
  - Iterações 20+: verificar a cada 20 iterações (Jensen com poucos samples)
  - Se violação detectada: SWITCH para modo robusto (CBS-Box sem gradiente, corte conservador)

MODO ROBUSTO (fallback para f não-convexa):
  - Usar CBS-Box com corte cíclico no centro
  - Reter AMBOS os lados que contêm uma avaliação com sinal correto
  - Convergência mais lenta mas garantida para f contínua com solução bracketed
```

### 6.3. Métricas de Confiança

```
typedef struct {
    int tests_run;
    int violations_found;
    double confidence;         // 1.0 = certamente convexa, 0.0 = certamente não
    double min_eigenvalue;     // menor autovalor amostrado da Hessiana
    int oracle_consistent;     // 1 se nenhuma inconsistência detectada
} convexity_status_t;
```

---

## 7. API em C

### 7.1. Header Principal: `cbs_nd.h`

```c
/**
 * cbs_nd.h - CBS-nD: Busca Binária Contínua em n Dimensões
 *
 * Encontra x* ∈ D ⊂ R^n tal que f(x*) ≈ T, para f convexa.
 * Três variantes: Elipsóide, Box, Simplex.
 */

#ifndef CBS_ND_H
#define CBS_ND_H

#include <stddef.h>

/* ============================================================
 * TIPOS BÁSICOS
 * ============================================================ */

/** Função black-box f: R^n → R */
typedef double (*func_nd_t)(const double *x, int n, void *params);

/** Variantes do algoritmo */
typedef enum {
    CBS_ND_ELLIPSOID = 0,   /* Domínio elipsoidal, update Löwner-John */
    CBS_ND_BOX       = 1,   /* Domínio hiperretangular, corte por coordenada */
    CBS_ND_SIMPLEX   = 2    /* Domínio simplex, corte pelo centróide */
} cbs_nd_variant_t;

/** Método de estimação de gradiente */
typedef enum {
    GRAD_CENTRAL_DIFF = 0,  /* Diferenças centrais: (f(x+h)-f(x-h))/(2h), 2n evals */
    GRAD_FORWARD_DIFF = 1,  /* Diferenças forward: (f(x+h)-f(x))/h, n evals */
    GRAD_RANDOM_DIR   = 2   /* Direção aleatória: (f(x+hd)-f(x))/h · d, 1 eval */
} cbs_nd_grad_method_t;

/** Status de convergência */
typedef enum {
    CBS_ND_CONVERGED      = 0,  /* |f(x*) - T| < eps */
    CBS_ND_MAX_ITER       = 1,  /* Atingiu max_iter sem convergir */
    CBS_ND_DEGENERATE     = 2,  /* Domínio degenerou (volume ≈ 0) */
    CBS_ND_NOT_CONVEX     = 3,  /* Detecção de não-convexidade */
    CBS_ND_NO_SOLUTION    = 4   /* T não parece estar em Im(f|_D) */
} cbs_nd_status_t;

/* ============================================================
 * CONFIGURAÇÃO
 * ============================================================ */

/** Parâmetros do algoritmo */
typedef struct {
    int n;                      /* Dimensão do espaço */
    cbs_nd_variant_t variant;   /* Qual variante usar */
    cbs_nd_grad_method_t grad_method; /* Método de gradiente */
    
    double epsilon;             /* Tolerância: |f(x*) - T| < epsilon */
    double delta;               /* Regularização do peso (default: epsilon/10) */
    double grad_step;           /* Step h para diferenças finitas (0 = auto) */
    
    int max_iter;               /* Máximo de iterações (0 = auto: 10*n²*log(R/eps)) */
    int max_evals;              /* Máximo de avaliações de f (0 = sem limite) */
    
    int check_convexity;        /* 1 = verificar convexidade periodicamente */
    int convexity_interval;     /* A cada quantas iterações verificar (default: 10) */
    int convexity_samples;      /* Amostras para teste de Jensen (default: 5) */
    
    int verbose;                /* 0=silencioso, 1=resumo, 2=cada iteração */
} cbs_nd_config_t;

/** Configuração default */
cbs_nd_config_t cbs_nd_default_config(int n);

/* ============================================================
 * RESULTADO
 * ============================================================ */

/** Resultado completo */
typedef struct {
    double *x_star;             /* Solução encontrada (n doubles, alocado internamente) */
    double f_value;             /* f(x*) */
    double error;               /* |f(x*) - T| */
    
    int total_evals;            /* Avaliações totais de f */
    int n_iterations;           /* Iterações executadas */
    int converged;              /* 1 se |f(x*) - T| < epsilon */
    cbs_nd_status_t status;     /* Status detalhado */
    
    /* Diagnóstico */
    double final_volume;        /* Volume estimado do domínio final */
    double final_diameter;      /* Diâmetro estimado do domínio final */
    double convexity_confidence;/* Confiança na convexidade (0 a 1) */
    int grad_evals;             /* Avaliações usadas só para gradientes */
} cbs_nd_result_t;

/** Liberar memória do resultado */
void cbs_nd_result_free(cbs_nd_result_t *result);

/* ============================================================
 * DOMÍNIO (representação do box inicial)
 * ============================================================ */

/** Domínio = hiperretângulo [a₁,b₁] × ... × [aₙ,bₙ] */
typedef struct {
    int n;
    double *lower;  /* a[i]: limites inferiores (n doubles) */
    double *upper;  /* b[i]: limites superiores (n doubles) */
} cbs_nd_domain_t;

/** Criar domínio a partir de arrays */
cbs_nd_domain_t cbs_nd_domain_create(int n, const double *lower, const double *upper);

/** Criar domínio cúbico [-R, R]^n */
cbs_nd_domain_t cbs_nd_domain_cube(int n, double R);

/** Liberar domínio */
void cbs_nd_domain_free(cbs_nd_domain_t *domain);

/* ============================================================
 * FUNÇÕES PRINCIPAIS
 * ============================================================ */

/**
 * Busca CBS-nD completa.
 *
 * Encontra x* ∈ D tal que |f(x*) - T| < config.epsilon.
 * Requer f convexa em D (verificado opcionalmente durante execução).
 *
 * @param f       Função black-box f: R^n → R
 * @param params  Parâmetros extras para f (passado a cada chamada)
 * @param T       Valor alvo
 * @param domain  Domínio de busca (hiperretângulo)
 * @param config  Configuração do algoritmo
 * @return        Resultado (chamar cbs_nd_result_free após uso)
 */
cbs_nd_result_t cbs_nd_search(
    func_nd_t f, void *params,
    double T,
    const cbs_nd_domain_t *domain,
    const cbs_nd_config_t *config
);

/**
 * Versão simplificada com defaults.
 */
cbs_nd_result_t cbs_nd_solve(
    func_nd_t f, void *params,
    double T,
    int n,
    const double *lower,
    const double *upper,
    double epsilon
);

/* ============================================================
 * FUNÇÕES AUXILIARES (expostas para testes)
 * ============================================================ */

/**
 * Estimar gradiente de f no ponto x por diferenças finitas.
 * grad deve ter espaço para n doubles.
 * Retorna número de avaliações usadas.
 */
int cbs_nd_estimate_gradient(
    func_nd_t f, void *params,
    const double *x, int n,
    double h,
    cbs_nd_grad_method_t method,
    double *grad
);

/**
 * Teste de convexidade por Jensen.
 * Retorna fração de violações (0.0 = provavelmente convexa).
 */
double cbs_nd_test_convexity(
    func_nd_t f, void *params,
    const cbs_nd_domain_t *domain,
    int n_tests,
    int *evals_used
);

#endif /* CBS_ND_H */
```

### 7.2. Estruturas Internas (não expostas no header público)

```c
/* --- Interno: estado do elipsóide --- */
typedef struct {
    int n;
    double *center;     /* n doubles */
    double *P;          /* n×n matrix (row-major): shape matrix */
    double log_vol;     /* log do volume (para tracking sem overflow) */
} ellipsoid_state_t;

/* --- Interno: estado do box --- */
typedef struct {
    int n;
    double *lower;      /* n doubles */
    double *upper;      /* n doubles */
} box_state_t;

/* --- Interno: estado do simplex --- */
typedef struct {
    int n;
    double *vertices;   /* (n+1) × n doubles (row-major) */
    double log_vol;
} simplex_state_t;

/* --- Interno: histórico de iteração --- */
typedef struct {
    double *point;      /* ponto de query */
    double f_value;     /* f(point) */
    double *gradient;   /* gradiente estimado */
    double error;       /* |f - T| */
    double volume_ratio;/* vol(K_{k+1})/vol(K_k) estimado */
} cbs_nd_iter_record_t;
```

---

## 8. Qual Variante Implementar Primeiro — e Por Quê

### Recomendação: **CBS-Box** primeiro, depois **CBS-Elipsóide**

**Razões para começar com CBS-Box:**

1. **Simplicidade de implementação:** Não requer álgebra de matrizes. O domínio é 2n doubles. O corte é truncar um intervalo. Pode ser implementada em ~150 linhas de C.

2. **Debugging fácil:** O estado é totalmente legível (limites por coordenada). Fácil de visualizar, printar, e verificar invariantes.

3. **Conexão direta com CBS-1D:** Cada corte coordenado É uma busca binária 1D parcial. Podemos reusar lógica do `cbs_algorithm.c` existente.

4. **Sem dependências:** Não precisa de LAPACK, BLAS, ou rotinas de álgebra linear. Compila com `-lm` apenas.

5. **Baseline sólida:** Dá convergência garantida O(n · log(R/ε)) — suficiente para validar a teoria e ter um benchmark.

6. **Testes em baixa dimensão:** Para n=2,3,4, CBS-Box é competitiva com o elipsóide e muito mais simples.

**Depois, CBS-Elipsóide porque:**

1. **Melhor para n grande:** A convergência não depende do alinhamento do gradiente com os eixos.

2. **Interessante teoricamente:** Conecta diretamente com a formalização (Teorema 6.4 / 7.1).

3. **Update é formulaico:** As fórmulas de Löwner-John são conhecidas e estáveis (2 linhas de álgebra matricial).

4. **CBS-Simplex é frágil:** Degeneração do simplex é um problema prático sério. Deixar para depois.

### Plano de Implementação em Fases

```
FASE 1 (imediata): CBS-Box
  ├── cbs_nd.h (header completo, todas variantes declaradas)
  ├── cbs_nd_box.c (implementação CBS-Box)
  ├── cbs_nd_gradient.c (estimação de gradiente, shared)
  ├── cbs_nd_convexity.c (testes de convexidade, shared)
  ├── cbs_nd_common.c (config defaults, result alloc/free)
  ├── test_cbs_nd.c (testes com funções convexas conhecidas)
  └── Makefile atualizado

FASE 2 (após validar CBS-Box): CBS-Elipsóide  
  ├── cbs_nd_ellipsoid.c (update Löwner-John)
  ├── linalg.c / linalg.h (operações matriciais mínimas, sem LAPACK)
  └── benchmark: comparar CBS-Box vs CBS-Elipsóide em n=2..20

FASE 3 (se necessário): CBS-Simplex
  ├── cbs_nd_simplex.c
  └── restart strategy para evitar degeneração
```

### Funções de Teste para Validação

```c
/* Funções convexas com solução conhecida para testes */

// 1. Esfera: f(x) = ‖x - x0‖², T = r² → solução na esfera de raio r
double test_sphere(const double *x, int n, void *params);

// 2. Quadrática geral: f(x) = xᵀAx + bᵀx + c, A ≻ 0
double test_quadratic(const double *x, int n, void *params);

// 3. Norma L1: f(x) = Σ|xᵢ - cᵢ| (convexa, não-diferenciável)
double test_l1_norm(const double *x, int n, void *params);

// 4. Log-sum-exp: f(x) = log(Σ exp(aᵢᵀx + bᵢ)) (suave, convexa)
double test_logsumexp(const double *x, int n, void *params);

// 5. Rosenbrock (NÃO convexa globalmente — para testar detecção)
double test_rosenbrock(const double *x, int n, void *params);
```

---

## 9. Resumo Executivo

| Item | Decisão |
|------|---------|
| Primeira implementação | CBS-Box |
| Representação de domínio | 2n doubles (lower/upper) |
| Centróide | Centro do box (trivial) |
| Direção do corte | Gradiente por diferenças centrais |
| Seleção de coordenada | argmax |g_i| (componente de maior magnitude) |
| Complexidade | O(n² · log(R/ε)) avaliações de f |
| Detecção de convexidade | Jensen sampling (3·n_tests evals, periódico) |
| Linguagem | C, sem dependências externas |
| Gradiente | Diferenças centrais (2n evals), fallback forward (n evals) |
| Tolerância do gradiente | h = ε^(1/3) · max(‖x‖_∞, 1) |

**Próximo passo concreto:** Implementar `cbs_nd_box.c` com testes em n=2 (função esfera e quadrática), verificar convergência empírica contra a teoria O(n · log(R/ε)).
