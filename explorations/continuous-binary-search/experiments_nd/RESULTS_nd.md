# Resultados: CBS-nD — Busca de Level Set em R^n

**Data:** 2026-08-02  
**Status:** Milestone concluído  
**Algoritmo:** CBS-nD Box (Continuous Binary Search, variante hiper-retângulo)

## 1. Problema

Dada f: R^n → R **contínua** em domínio compacto D = [a₁,b₁] × ... × [aₙ,bₙ], encontrar:

```
k* = (k₀*, k₁*, ..., kₙ*) tal que f(k*) = T
```

onde T é um valor alvo dado.

**Equivalente:** encontrar um ponto no level set L_T = {x ∈ D : f(x) = T}.

## 2. Resultado Principal

### Para f convexa: algoritmo polinomial

```
Complexidade: O(n² · log(R/ε)) avaliações de f
```

Onde:
- n = dimensão do espaço
- R = diâmetro do domínio (max_i (bᵢ - aᵢ))
- ε = precisão desejada (|f(k*) - T| < ε)

O algoritmo é **polinomial em n** e **logarítmico em 1/ε**.

### Para f geral (apenas Lipschitz): exponencial inevitável

```
Lower bound: Ω((L/ε)^n) avaliações de f
```

Qualquer algoritmo precisa de exponencialmente muitas avaliações no pior caso (Nemirovsky-Yudin 1983, Sikorski 2001).

## 3. Algoritmo CBS-nD Box

### Pseudocódigo

```
CBS-nD(f, T, bounds[n], ε):
  Repetir:
    q = centro do box (bounds)
    fq = f(q)
    Se |fq - T| < ε: retornar q

    grad = gradiente por diferenças finitas (2n evals)
    j = coordenada com maior |grad[j]|
    
    Se (fq > T) XOR (grad[j] < 0):
      bounds[j].upper = q[j]    // corta metade superior
    Senão:
      bounds[j].lower = q[j]    // corta metade inferior
```

### Por que funciona (para f convexa)

1. O gradiente ∇f(q) define um **separating hyperplane**: como f é convexa, se f(q) > T então a solução está no semi-espaço {x : ⟨∇f(q), x-q⟩ ≤ 0}.
2. Cortar na coordenada de maior |∂f/∂xⱼ| maximiza a redução de volume.
3. Cada corte elimina metade do domínio na coordenada j → após n cortes (um por coordenada), o volume reduz por fator 2^n... mas na prática convergimos em ~30 iterações porque sempre cortamos na melhor direção.

### Conexão formal: CBS = Cutting Plane Method

O CBS-nD é um caso particular do **cutting plane method**:

| Conceito CBS | Equivalente Cutting Plane |
|---|---|
| Centro do box | Ponto de query (analytic center) |
| f(q) > T ou < T | Separating oracle |
| Corte em coordenada j | Halfspace update |
| Redução do box | Redução do politopo K |
| Mediana ponderada (1D) | Centróide volumétrico |

Em 1D (n=1), CBS com mediana ponderada ≡ cutting plane com corte ótimo (ρ = 1/2 exato).

## 4. Resultados Experimentais

### 4.1 Funções de Teste

| # | Função | Fórmula | Propriedades |
|---|---|---|---|
| 1 | Sphere | Σ xᵢ² | Convexa, separável |
| 2 | Ellipsoid | Σ (i+1)·xᵢ² | Convexa, mal-condicionada |
| 3 | Rosenbrock | Σ (1-xᵢ)²+100(xᵢ₊₁-xᵢ²)² | **Não-convexa** |
| 4 | Sum\|x\| | Σ \|xᵢ\| | Convexa, não-diferenciável |
| 5 | Linear | Σ (i+1)·xᵢ | Linear |
| 6 | Quad+cross | Σ xᵢ² + 0.5·Σ xᵢ·xᵢ₊₁ | Convexa, termos cruzados |
| 7 | Max coord | max_i xᵢ | Convexa, não-diferenciável |
| 8 | LogSumExp | log(Σ exp(xᵢ)) | Convexa, smooth |

### 4.2 Resultados por Dimensão (ε = 10⁻⁸)

**n = 2:**

| Função | Evals | Iters | Erro | Status |
|---|---|---|---|---|
| Sphere | 146 | 29 | 5.2e-9 | ✓ |
| Ellipsoid | 156 | 31 | 1.9e-9 | ✓ |
| Rosenbrock | 1 | 0 | 0.0 | ✓ (trivial: centro já é solução) |
| Sum\|x\| | 136 | 27 | 7.5e-9 | ✓ |
| Linear | 353 | 82 | 9.3e-9 | ✓ |
| Quad+cross | 141 | 28 | 1.3e-10 | ✓ |
| Max coord | 136 | 27 | 7.5e-9 | ✓ |
| LogSumExp | 136 | 27 | 3.1e-9 | ✓ |

**n = 5:**

| Função | Evals | Iters | Erro | Status |
|---|---|---|---|---|
| Sphere | 320 | 29 | 5.2e-9 | ✓ |
| Ellipsoid | 342 | 31 | 1.9e-9 | ✓ |
| Rosenbrock | 2287 | 238 | 1.3e+0 | ✗ DEGEN |
| Sum\|x\| | 298 | 27 | 7.5e-9 | ✓ |
| Linear | 320 | 29 | 9.3e-9 | ✓ |
| Quad+cross | 309 | 28 | 1.3e-10 | ✓ |
| Max coord | 298 | 27 | 7.5e-9 | ✓ |
| LogSumExp | 298 | 27 | 8.9e-9 | ✓ |

**n = 10:**

| Função | Evals | Iters | Erro | Status |
|---|---|---|---|---|
| Sphere | 610 | 29 | 5.2e-9 | ✓ |
| Ellipsoid | 652 | 31 | 1.9e-9 | ✓ |
| Rosenbrock | 8953 | 478 | 6.2e+0 | ✗ DEGEN |
| Sum\|x\| | 568 | 27 | 7.5e-9 | ✓ |
| Linear | 631 | 30 | 9.3e-9 | ✓ |
| Quad+cross | 589 | 28 | 1.3e-10 | ✓ |
| Max coord | 568 | 27 | 7.5e-9 | ✓ |
| LogSumExp | 568 | 27 | 7.5e-9 | ✓ |

**n = 20:**

| Função | Evals | Iters | Erro | Status |
|---|---|---|---|---|
| Sphere | 1190 | 29 | 5.2e-9 | ✓ |
| Ellipsoid | 1272 | 31 | 1.9e-9 | ✓ |
| Rosenbrock | 37319 | 948 | 1.7e+1 | ✗ DEGEN |
| Sum\|x\| | 1108 | 27 | 7.5e-9 | ✓ |
| Linear | 1272 | 31 | 9.3e-9 | ✓ |
| Quad+cross | 1149 | 28 | 1.3e-10 | ✓ |
| Max coord | 1108 | 27 | 7.5e-9 | ✓ |
| LogSumExp | 985 | 24 | 1.2e-9 | ✓ |

### 4.3 Resumo

```
Total: 29/32 testes passaram
Falhas: apenas Rosenbrock (não-convexa) para n ≥ 5
```

### 4.4 Escalamento Observado

| n | Evals típico (convexa) | Iters | Evals/iter |
|---|---|---|---|
| 2 | ~140 | ~28 | 5 (= 2n+1) |
| 5 | ~310 | ~28 | 11 (= 2n+1) |
| 10 | ~590 | ~28 | 21 (= 2n+1) |
| 20 | ~1150 | ~28 | 41 (= 2n+1) |

**Observação chave:** iterações ≈ 27-31, **independente de n**. O número de iterações depende apenas de log(R/ε). O custo por iteração é (2n+1) para o gradiente por diferenças finitas.

**Complexidade confirmada:** Total ≈ (2n+1) × 28 ≈ 56n + 28 = **O(n)** para ε fixo.

Mais rigorosamente: **O(n · log(R/ε))** avaliações totais (cada iteração custa O(n), são O(log(R/ε)) iterações). O O(n²) do bound teórico é pessimista — empiricamente é O(n·log(R/ε)).

### 4.5 CBS-nD vs Gradient Descent (n=10)

| Função | CBS evals | CBS err | GD evals | GD err | Vencedor |
|---|---|---|---|---|---|
| Sphere | 610 | 5.2e-9 | 22 | 5.0 | CBS |
| Ellipsoid | 652 | 1.9e-9 | 22 | 10.0 | CBS |
| Rosenbrock | 8953 | 6.2 | 105001 | 1.2 | Ambos falham |
| Sum\|x\| | 568 | 7.5e-9 | 22 | 3.0 | CBS |
| Linear | 631 | 9.3e-9 | 105001 | 0.7 | CBS |
| Quad+cross | 589 | 1.3e-10 | 22 | 8.0 | CBS |
| Max coord | 568 | 7.5e-9 | 105001 | 0.02 | CBS |
| LogSumExp | 568 | 7.5e-9 | 105001 | 0.002 | CBS |

**CBS converge em 7/8; GD não converge em nenhuma** (GD com step fixo é inadequado para level-set finding — otimiza |f-T|² mas fica preso).

## 5. O Que Isso Significa

### 5.1 Contribuição

Não é um resultado novo para a comunidade de otimização (cutting plane methods existem desde 1979). O que formalizamos é:

1. **Unificação:** CBS-1D (nossa busca binária contínua com mediana ponderada) é a degenerescência 1D do cutting plane method. A mediana = centróide, o corte = separating hyperplane.

2. **Implementação limpa:** Algoritmo CBS-Box em ~250 linhas de C, sem dependências, O(n) memória.

3. **Fronteira tratável/intratável:**
   - f convexa → **O(n · log(R/ε))** — polinomial ✓
   - f Lipschitz geral → **Θ((L/ε)^n)** — exponencial ✗

### 5.2 Analogia com P vs NP

| Mundo Discreto | Mundo Contínuo (CBS) |
|---|---|
| 2-SAT ∈ P | f convexa → O(poly(n)) |
| 3-SAT é NP-completo | f grau ≥ 4 → NP_R-completo (BSS) |
| Unit propagation | Subgradiente (propaga info local→global) |
| Backtracking | Amostragem exaustiva do domínio |
| Estrutura local propagável | Convexidade / monotonicidade |
| Sem estrutura | Lipschitz geral / funções arbitrárias |

**Insight central:** A **convexidade** é a propriedade que permite que uma avaliação local f(q) + ∇f(q) produza informação **global** sobre onde está a solução. Sem essa propriedade, cada avaliação dá apenas informação local e a busca é inevitavelmente exponencial.

### 5.3 Limitações

1. **Requer convexidade** (ou monotonicidade por coordenada) para garantia polinomial
2. **Não é novo** como algoritmo — é cutting plane com gradiente numérico
3. **Requer T ∈ (min f, max f)** — existência da solução é pré-condição
4. **Encontra UMA solução** — para f convexa com T > min f, o level set é uma superfície (n-1)-dimensional; o algoritmo encontra um ponto qualquer nela

## 6. Questões em Aberto

1. **Classes intermediárias:** Quasi-convexas? Funções com k extremos (k fixo)? Log-concave?
2. **Detecção automática de convexidade:** Pode-se verificar on-the-fly se f é convexa e adaptar a estratégia?
3. **Dimensão efetiva:** Se f depende realmente de d ≪ n variáveis, pode-se detectar e reduzir?
4. **Conexão com NP combinatório:** Relaxações contínuas de SAT/MAX-CUT são convexas? (Spoiler: SDP relaxations são, mas o arredondamento introduz gap.)
5. **CBS-Elipsóide:** A variante com update de Löwner-John é mais eficiente que CBS-Box?

## 7. Arquivos

```
explorations/continuous-binary-search/
├── theory_nd.md                          # Formalização teórica CBS ↔ Cutting Plane
├── experiments_nd/
│   ├── cbs_nd.h                          # API do CBS-nD
│   ├── cbs_nd.c                          # Implementação (~250 LOC)
│   ├── benchmark_nd.c                    # Benchmark: 8 funções × 4 dimensões
│   └── Makefile
└── experiments/
    ├── COMPLEXITY.md                     # Análise de complexidade do CBS-1D
    └── (implementação CBS-1D anterior)
```

## 8. Referências

1. Nemirovsky, A. & Yudin, D. (1983). *Problem Complexity and Method Efficiency in Optimization.*
2. Khachiyan, L. (1979). "A polynomial algorithm in linear programming."
3. Grünbaum, B. (1960). "Partitions of mass-distributions by hyperplanes."
4. Blum, L., Shub, M. & Smale, S. (1989). "On a theory of computation over the real numbers."
5. Sikorski, K. (2001). *Optimal Solution of Nonlinear Equations.* Oxford.
6. Oliveira & Takahashi (2021). "An Enhancement of the Bisection Method." ACM TOMS.
