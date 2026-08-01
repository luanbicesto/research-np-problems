# Survey: Level-Set Finding em R^n — Estado da Arte

**Problema:** Dada f: R^n → R (black-box, contínua), encontrar x* ∈ D ⊂ R^n tal que f(x*) = T.

**Contexto:** CBS implementado para n=1 com custo O(log(W/ε)). Objetivo: generalizar para n arbitrário.

**Data:** 2026-08-01

---

## 1. Algoritmos Existentes para Level-Set Finding em R^n

### 1.1 Bisseção Generalizada (Eiger, Sikorski, Stenger — 1984)

**Referência:** Eiger, A., Sikorski, K., Stenger, F. "A bisection method for systems of nonlinear equations." ACM Trans. Math. Softw. 10(4), 367–377, 1984.

**Ideia:** Generalizar bisseção para sistemas F: R^n → R^n usando o grau topológico. Se deg(F, D) ≠ 0, existe uma raiz em D. O método subdivide simplexos e usa o grau topológico para identificar sub-regiões que contêm raízes.

**Complexidade:** O((1/ε)^n) avaliações no pior caso para obter precisão ε.

**Limitação:** Requer F: R^n → R^n (sistema quadrado). Para nosso problema (f: R^n → R, level set é variedade (n-1)-dimensional), precisa ser reformulado.

### 1.2 Bisseção Simplicial (Kearfott, Vrahatis)

**Referências:**
- Kearfott, R.B. "Some tests of generalized bisection." ACM TOMS 13(3), 1987.
- Vrahatis, M.N. "A rapid generalized method of bisection for solving systems of non-linear equations." Numer. Math. 49, 123–138, 1986.

**Ideia:** Particionar o domínio em simplexos e usar testes de inclusão (mudança de sinal generalizada via grau topológico) para descartar regiões sem raízes.

**Complexidade:** Exponencial em n. O volume de cada simplexo decresce por fator 1/2 por iteração, mas o número de simplexos cresce exponencialmente.

### 1.3 Métodos Intervalares (Moore, Hansen, Kearfott)

**Referências:**
- Moore, R.E. "Interval Analysis." Prentice-Hall, 1966.
- Hansen, E. "Global Optimization Using Interval Analysis." Marcel Dekker, 1992.
- Kearfott, R.B. "Rigorous Global Search." Kluwer, 1996.
- arXiv:2603.19965 (2026) — "Computational Complexity Analysis of Interval Methods"

**Ideia:** Usar aritmética intervalar para obter enclosures garantidas. Interval Newton: dado [x], computar N([x]) = m - [F'([x])]^{-1} F(m). Se N([x]) ⊂ [x], existe raiz única em [x].

**Complexidade:**
- Branch-and-bound intervalar: O(2^n) no pior caso para n dimensões
- Na prática, funciona bem para n ≤ 10–20 com boas pré-condições
- Artigo de 2026 (arXiv:2603.19965) confirma que o custo computacional é limitado por computational cost exponencial em alta dimensão

**Vantagem:** Rigor matemático — garante encontrar TODAS as raízes.

### 1.4 Otimização Lipschitz (Piyavskii-Shubert, DIRECT)

**Referências:**
- Piyavskii, S.A. (1972). "An algorithm for finding the absolute extremum of a function."
- Shubert, B.O. (1972). "A sequential method seeking the global maximum of a function."
- Jones, D.R., Perttunen, C.D., Stuckman, B.E. (1993). "Lipschitzian optimization without the Lipschitz constant." J. Optim. Theory Appl. 79(1).
- Bouttier, Cesari, Gerchinovitz (2020). "Regret analysis of the Piyavskii-Shubert algorithm for global Lipschitz optimization." arXiv:2002.02390.

**Piyavskii-Shubert:**
- Constrói envoltória inferior/superior usando constante de Lipschitz L
- Para n=1: O((L·W/ε)) avaliações — linear, não logarítmico
- Para n>1: generalização requer tesselação do espaço, custo O((L/ε)^n)
- Resultado recente (Bouttier et al. 2020): regret cumulativo O(L log T) para Lipschitz contínuas em 1D

**DIRECT (Jones et al. 1993):**
- Elimina necessidade de conhecer L explicitamente
- Subdivide hipercubos, avalia centros, seleciona "potentially optimal rectangles"
- Prático para n ≤ 20 variáveis
- Não tem garantia de convergência polinomial em n
- Artigo retrospectivo "DIRECT — 25 Years Later" (2020) confirma limitação dimensional

**Adaptação para level-set:** Minimizar |f(x) - T| em vez de f(x). Funciona, mas herda complexidade exponencial.

### 1.5 Métodos de Continuação/Homotopia (Allgower-Georg)

**Referências:**
- Allgower, E.L., Georg, K. "Numerical Continuation Methods." Springer, 1990.
- Allgower, E.L., Georg, K. "Introduction to Numerical Continuation Methods." SIAM, 2003.

**Ideia:** Para encontrar zeros de H(x,λ)=0, começar com λ=0 (problema fácil) e seguir a curva solução até λ=1 (problema difícil).

**Para level-set finding:**
- Definir H(x,t) = f(x) - (1-t)·f(x₀) - t·T
- Seguir a curva de H=0 de t=0 (trivial: x₀) até t=1 (solução: f(x)=T)
- Custo: O(n²) por passo preditor-corretor + número de passos ao longo da curva

**Complexidade:** Depende do comprimento da curva e da curvatura. Não é polinomial em n no pior caso, mas frequentemente eficiente na prática.

**Requisitos:** f deve ser diferenciável (C¹ mínimo). Não funciona para black-box puro sem gradiente.

### 1.6 Bayesian Optimization para Level Sets (Gotovos et al. 2013)

**Referências:**
- Gotovos, A., Casati, N., Hitz, G., Krause, A. "Active Learning for Level Set Estimation." IJCAI 2013.
- Zanette, Laloue, et al. (2018). "A unified approach to Bayesian optimization and level-set estimation." arXiv:1610.07379.
- arXiv:2407.05914 (2024). "Constructing Level Sets Using Smoothed Approximate Bayesian Computation."
- arXiv:1902.09682 (2019). Hierarchical GP-based level set estimation.

**Ideia:** Modelar f com Gaussian Process (GP). Escolher pontos de amostragem que maximizam informação sobre a fronteira {x: f(x) = T}. Classificar domínio em super-level set e sub-level set.

**Complexidade:**
- Depende do kernel e da suavidade de f
- Para kernel Matérn-ν com d dimensões: O(n^{d/(2ν+d)}) no regime teórico (sublinear no budget)
- Na prática: limitado a n ≤ 10–20 por custo O(N³) de inferência GP (N = número de observações)
- Escalável com sparse GPs ou random features até n ~ 50–100

**Vantagem:** Sample-efficient — poucas avaliações de f. Ideal quando f é caro.

### 1.7 Métodos Randomizados (Hit-and-Run para Level Sets)

**Referências:**
- Lovász, L., Vempala, S. (2006). "Hit-and-run from a corner." SIAM J. Computing 35(4).
- Lovász, L., Vempala, S. (2007). "The geometry of logconcave functions and sampling algorithms."
- arXiv:2211.04439 (2022). "Sampling from convex sets with a cold start."
- arXiv:2505.01937 (2025). "Faster Logconcave Sampling from a Cold Start in High Dimension."

**Ideia:** Se o sublevel set S_T = {x: f(x) ≤ T} é convexo, usar random walks (hit-and-run, ball walk) para amostrar uniformemente de S_T.

**Complexidade:**
- Hit-and-run mistura em O*(n³) passos para corpo convexo geral (Lovász-Vempala)
- Resultado de 2022: cold start em O*(n³) com multiscale decompositions
- Resultado de 2025: O*(n² log⁴ n) com Rényi divergence mais fraca
- Cada passo requer O(1) avaliações de f (line search no raio)

**Requisito crucial:** Convexidade de f (para que S_T seja convexo).

---

## 2. Complexidade de Avaliações (Oracle Complexity)

### 2.1 Resultados Fundamentais de Nemirovsky-Yudin (1983)

**Referência:** Nemirovski, A.S., Yudin, D.B. "Problem Complexity and Method Efficiency in Optimization." Wiley, 1983.

**Resultado central:** Para otimização convexa de f Lipschitz (constante L) sobre B_d(0,R) com oráculo de primeira ordem (valor + subgradiente):

- **Lower bound:** Ω(1/ε²) queries para obter f(x) - f* ≤ ε (independente de d!)
- **Upper bound:** O(L²R²/ε²) via subgradient method — tight!

Para oráculo de valor apenas (zero-order/derivative-free):
- Lower bound clássico: Ω(d) (herdado do first-order)
- **NOVO resultado (Kerger, julho 2026, arXiv:2607.13335):** Ω(d²/log(d+1)) para precisão Θ(d^{-1/2})
- Upper bound: O(d² log² d) via método de Protasov (1996)
- **Gap fechado** (up to log factors): Θ(d²) queries para otimização convexa derivative-free!

### 2.2 Para f Lipschitz Geral (Não-Convexa)

**Resultado (Novak, Woźniakowski — "Tractability of Multivariate Problems", 2008–2012):**

Para f: [0,1]^n → R com constante de Lipschitz L, encontrar x com |f(x) - min f| ≤ ε:

- **Lower bound:** Ω((L/(2ε))^n) avaliações
- **Prova:** Adversário pode esconder um "pico" de altura 2ε em qualquer subcubo de lado 2ε/L. Existem (L/(2ε))^n subcubos disjuntos possíveis. Sem avaliar todos, não se pode encontrar o pico.

**Implicação para level-set:** Encontrar x com |f(x) - T| < ε para f Lipschitz geral requer Ω((L/ε)^n) queries no worst case.

**Isso é THE curse of dimensionality formal.**

### 2.3 Para f Convexa

**Redução a otimização:** Se f é convexa e sabemos que min f < T, encontrar x com f(x) = T equivale a minimizar (f(x) - T)² sobre o sublevel set, ou usar bisseção no parâmetro de penalidade.

**Complexidade com first-order oracle (subgradiente disponível):**
- Ellipsoid method: O(n² log(R/ε)) separating hyperplane queries
- Cutting plane (center of gravity): O(n log(R/ε)) queries, mas cada query custa O(n⁴)
- Vaidya's method: O(n log(nR/ε)) queries com O(n³) per iteration

**Complexidade com value-only oracle:**
- Upper bound: O(n² log²(nR/ε)) via estimação de gradiente + ellipsoid
- Lower bound (novo, 2026): Ω(n²/log n) — essencialmente tight!
- **Conclusão: para f convexa, level-set finding é POLINOMIAL em n com value oracle.**

### 2.4 Para f com Bounded Hessian (f ∈ C²)

Se ||∇²f|| ≤ H uniformemente:
- Newton-type methods convergem quadraticamente localmente
- Globalmente: continuation methods com O(√(H·D)) passos (D = diâmetro)
- Complexidade: O(n² · √(H·D/ε)) avaliações (polinomial em n se H, D limitados)
- **Mas:** requer acesso a ∇f e ∇²f, não apenas valor.

### 2.5 Tabela Resumo de Oracle Complexity

| Classe de f | Oráculo | Complexidade para |f(x)-T|<ε | Referência |
|---|---|---|---|
| Lipschitz geral | Valor | Θ((L/ε)^n) — EXPONENCIAL | Novak-Woźniakowski |
| Lipschitz convexa | Valor | Θ(n² log(1/ε)) | Nemirovski-Yudin + Kerger 2026 |
| Lipschitz convexa | Valor + gradiente | O(n log(1/ε)) | Ellipsoid/Cutting plane |
| C² com Hessian bound | Valor + grad + Hessian | O(n² √(1/ε)) | Newton-continuation |
| Separável f=Σfᵢ | Valor | O(n · log(1/ε)) — LINEAR em n | Decomposição trivial |
| Low effective dim d_e | Valor | O(n² · (1/ε)^{d_e}) | REMBO/Active subspaces |

---

## 3. Bisseção em Dimensão Alta — Resultados de Sikorski

### 3.1 Sikorski (1982): Bisseção é Ótima em 1D

**Referência:** Sikorski, K. "Bisection is optimal." Numer. Math. 40, 111–117, 1982.

**Teorema:** Para f: [a,b] → R contínua com f(a)·f(b) < 0, o método da bisseção é ótimo entre todos os métodos determinísticos que usam apenas avaliações de f. Custo: ⌈log₂((b-a)/ε)⌉ avaliações.

### 3.2 Sikorski (2001): Livro "Optimal Solution of Nonlinear Equations"

**Referência:** Sikorski, K. "Optimal Solution of Nonlinear Equations." Oxford University Press, 2001.

**Resultados principais:**

1. **Para f: [0,1]^n → R^n Lipschitz com deg(f,D)≠0:**
   - Custo ótimo (worst case): Θ((1/ε)^n) avaliações de f
   - Isto é: bisseção em cada dimensão, sem possibilidade de fazer melhor

2. **Para n=2, funções não-contrativas:**
   - "O problema tem complexidade worst-case infinita" — mesmo para precisão finita, não existe algoritmo universal com garantia de convergência para toda f contínua

3. **Otimalidade da bisseção generalizada:**
   - Para a classe de funções satisfazendo condições de grau topológico, bisseção simplicial é assintoticamente ótima

### 3.3 O Teorema Fundamental (Lower Bound Exponencial)

**Enunciado informal:** Para f: [0,1]^n → R Lipschitz com constante L, qualquer algoritmo determinístico que garante encontrar x com |f(x) - T| < ε requer pelo menos Ω((L/(cε))^n) avaliações de f no pior caso, onde c é uma constante universal.

**Prova (esboço):**
1. Particionar [0,1]^n em M^n subcubos de lado 1/M, onde M = ⌊L/(2ε)⌋
2. Construir f₀ ≡ T + ε (não atinge T em lugar nenhum)
3. Modificar f₀ em exatamente um subcubo para criar um "vale" que atinge T
4. Duas funções que diferem apenas em um subcubo são indistinguíveis até que esse subcubo seja amostrado
5. Existem M^n subcubos possíveis → necessário Ω(M^n) queries para distinguir

**Consequência devastadora:** Para CBS generalizado, o custo é EXPONENCIALMENTE pior que 1D:
- n=1: O(log(1/ε)) ✓
- n=2: Ω((1/ε)²)
- n=10: Ω((1/ε)^{10})
- n=100: Ω((1/ε)^{100})

### 3.4 Hollender, Lawrence, Segal-Halevi (2023): Monotonicidade Salva

**Referência:** arXiv:2310.07333 (outubro 2023, revisado fevereiro 2024)

**Resultado fundamental para nosso projeto:**

Para F: [a,b]^d → R^d (sistema quadrado, condições de Miranda):
- **Sem monotonicidade:** encontrar raiz aproximada pode requerer número EXPONENCIAL de avaliações, mesmo para d=2
- **Com 1 condição de monotonicidade (d=2):** custo POLINOMIAL na precisão
- **Com todas d²-d condições ex-diagonais** (componente i decresce em variável j para i≠j): custo POLINOMIAL para todo d fixo
- **Com d²-d-2 condições:** pode ser EXPONENCIAL

**Interpretação para level-set (f: R^n → R):**
- Nosso problema é subquadrado (1 equação, n incógnitas)
- Mas se adicionarmos n-1 equações (e.g., ∇f ⊥ direção de busca), obtemos sistema quadrado
- Condições de monotonicidade parcial em f podem permitir bisseção polinomial!

---

## 4. Maldição da Dimensionalidade — Resultados Concretos

### 4.1 Prova Formal: Custo Exponencial para f Black-Box Lipschitz

**SIM, existe prova formal.** Múltiplas fontes independentes:

1. **Novak, E., Woźniakowski, H.** "Tractability of Multivariate Problems." EMS, Volumes I (2008), II (2010), III (2012).
   - Teorema: Para integração e aproximação de funções de Sobolev em [0,1]^d com standard information (avaliações pontuais), o problema sofre da maldição da dimensionalidade: o número mínimo de avaliações para erro ≤ ε é exponencial em d.
   - Formalmente: n(ε,d) ≥ c · (1/ε)^{d/s} para funções em W^s_p([0,1]^d)

2. **Sikorski (2001):** Lower bound de Ω((1/ε)^n) para root-finding de funções Lipschitz em R^n (ver Seção 3.3).

3. **Nemirovski-Yudin (1983):** Para otimização NÃO-convexa, qualquer método determinístico requer Ω((L/ε)^n) avaliações.

### 4.2 Suposições que Quebram a Maldição

#### 4.2.1 Convexidade → Método do Elipsóide

**Resultado:** Para f convexa, encontrar x com f(x) ≤ T + ε é equivalente a feasibility no sublevel set S_T = {x: f(x) ≤ T}.

**Métodos e complexidades (com separation oracle / subgradient):**
- Ellipsoid method (Khachiyan 1979): O(n² log(R/ε)) queries
- Cutting plane (center of gravity): O(n log(R/ε)) queries
- Vaidya (1996): O(n log(n/ε)) queries, O(n³) por iteração

**Com value oracle apenas:**
- Estimativa de gradiente via diferenças finitas: O(n) avaliações por "pseudo-gradiente"
- Total: O(n² · n · log(R/ε)) = O(n³ log(R/ε)) — POLINOMIAL em n

**O elipsóide é a generalização da bisseção para dimensão alta sob convexidade!**
- Em 1D com f convexa: bisseção = elipsóide degenerado (intervalo é elipsóide 1D)
- Em nD: elipsóide corta volume por fator (1 - 1/(n+1)) por iteração

**Referência chave:** Zib.de lecture notes: "The ellipsoid method can, in fact, be seen as a generalization of the bisection method to higher dimension."

#### 4.2.2 Monotonicidade Parcial

**Resultado (Hollender et al. 2023):** Se f satisfaz condições de monotonicidade ex-diagonal suficientes, root-finding é polinomial.

**Para nosso caso (f: R^n → R escalar):** Se f é monótona em cada variável separadamente (∂f/∂xᵢ > 0 para todo i), então:
- O level set L_T = {x: f(x) = T} é uma hipersuperfície "monótona"
- Bisseção coordenada a coordenada funciona: custo O(n · log(W/ε))
- Essencialmente: resolver n problemas 1D sequencialmente

#### 4.2.3 Separabilidade (f = Σ fᵢ(xᵢ))

Se f(x₁,...,xₙ) = Σᵢ fᵢ(xᵢ), encontrar x com f(x) = T:
- Resolver n problemas 1D independentes (em paralelo)
- Custo: O(n · log(W/ε)) — LINEAR em n, logarítmico em 1/ε
- Generalização: separabilidade parcial f = Σⱼ gⱼ(x_Sⱼ) com |Sⱼ| ≤ k → custo O(n · (1/ε)^k)

#### 4.2.4 Low Effective Dimension

**Referências:**
- Wang, Z. et al. "Bayesian Optimization in a Billion Dimensions via Random Embeddings (REMBO)." JAIR 55, 2016. arXiv:1301.1942.
- Cartis, Otemissov (2022). "A dimensionality reduction technique for unconstrained global optimization of functions with low effective dimensionality." arXiv:2003.09673.
- arXiv:2401.17825 (2024). "Learning the subspace of variation for global optimization of functions with low effective dimension."

**Definição:** f tem dimensão efetiva d_e ≪ n se existe subespaço V ⊂ R^n com dim(V) = d_e tal que f(x) = g(P_V x) para alguma g: R^{d_e} → R.

**Resultado (REMBO):** Se f tem dimensão efetiva d_e, projeção aleatória A ∈ R^{n×d_e} permite otimizar em R^{d_e}:
- Custo: O((1/ε)^{d_e}) — independente de n!
- Probabilidade de sucesso: alta para A Gaussiana aleatória
- Demonstrado para problemas com n = 10⁹ e d_e = 5

**Resultado (Active Subspaces, 2024):** Framework algorítmico para aprender o subespaço V e otimizar simultaneamente. Custo polinomial em n para descobrir V se gradientes disponíveis.

#### 4.2.5 Suavidade Alta (Classe C^k)

Para f ∈ C^k([0,1]^n) com derivadas limitadas:
- Melhor complexidade: O((1/ε)^{n/k}) — ainda exponencial em n, mas melhor por fator k
- Se k → ∞ (funções analíticas): complexidade pode ser subexponencial em n

---

## 5. Estado da Arte Prático (2020–2026)

### 5.1 Melhores Algoritmos por Regime Dimensional

| Dimensão | Método Recomendado | Custo típico | Requisitos |
|---|---|---|---|
| n=1 | ITP/CBS (nosso) | O(log(1/ε)) | f contínua, mudança de sinal |
| n=2–5 | Interval methods / Continuation | O((1/ε)^n) mas com constantes pequenas | f Lipschitz ou C¹ |
| n=5–20 | DIRECT + refinamento local | Milhares de avaliações | f Lipschitz, bounds |
| n=10–50 | CMA-ES / Bayesian Optimization | 10²–10⁴ avaliações | f black-box |
| n=50–100 | REMBO / Active Subspaces + BO | Depende de d_e efetiva | Low effective dimension |
| n=100–1000 | Gradient-based (se ∇f disponível) | O(n · log(1/ε)) | ∇f acessível |
| n=1000+ | SGD/Adam + diferenciação automática | O(n · polylog(1/ε)) | f diferenciável, autodiff |

### 5.2 CMA-ES (Covariance Matrix Adaptation Evolution Strategy)

**Referência:** Hansen, N. "The CMA Evolution Strategy: A Tutorial." arXiv:1604.00772, 2016.

**Estado:** O padrão-ouro para otimização derivative-free black-box em dimensão moderada.

- Adapta uma distribuição Gaussiana multivariada para amostrar pontos candidatos
- Custo por iteração: O(n²) (atualização da matriz de covariância)
- Variante LM-CMA (Large-scale): O(n) por iteração para n > 100
- NeurIPS 2016: CMA-ES com storage O(n) atingiu state-of-the-art
- Prático até n ~ 1000 com variantes lineares (sep-CMA, LM-CMA)
- **Para level-set:** minimizar |f(x) - T|² com CMA-ES

### 5.3 Neural Implicit Representations (NeRF, DeepSDF)

**Referências:**
- Park et al. "DeepSDF: Learning Continuous Signed Distance Functions." CVPR 2019.
- Mildenhall et al. "NeRF: Representing Scenes as Neural Radiance Fields." ECCV 2020.
- NVIDIA NGLOD (2022): Real-time rendering with implicit 3D shapes.

**Ideia:** Representar f como rede neural f_θ: R^n → R treinada em dados. O level set {x: f_θ(x) = T} é extraído via:
- Marching cubes (para n=3)
- Sphere tracing / ray marching
- Gradient descent no nível: x_{k+1} = x_k - η(f_θ(x_k) - T) · ∇f_θ(x_k)/||∇f_θ(x_k)||²

**Relevância:** Quando f é aprendida (surrogate model), gradientes estão disponíveis via backpropagation → custo O(n) por passo de Newton no level set.

**Limitação:** Não é black-box — requer treinamento prévio da rede.

### 5.4 MCMC para Level Sets

**Referências:**
- Robert & Casella. "Monte Carlo Statistical Methods." Springer, 2004.
- Lovász, Vempala (2007). Hit-and-run mixing para corpos convexos.
- arXiv:2104.07097 (2021). "Novel Matrix Hit and Run for Sampling Polytopes and its GPU Implementation."

**Approach:** Para amostrar do level set L_T = {x: f(x) ≈ T}:
1. Definir distribuição π(x) ∝ exp(-β|f(x)-T|²) (concentrada perto de L_T para β grande)
2. Amostrar via MCMC (Langevin, HMC, hit-and-run)
3. Para β → ∞, amostras convergem para L_T

**Complexidade de mixing:**
- Para f convexa: hit-and-run mistura em O*(n³) passos
- Para f log-côncava: HMC mistura em Õ(n^{1.5}/ε)
- Para f geral: sem garantia de mixing polynomial

### 5.5 Gradient-Based Methods (quando ∇f disponível)

Se temos acesso a ∇f (primeira ordem):

**Newton projetado no level set:**
```
x_{k+1} = x_k - (f(x_k) - T) · ∇f(x_k) / ||∇f(x_k)||²
```
- Convergência: local, quadrática (se f ∈ C² e ∇f(x*) ≠ 0)
- Custo: O(n) por iteração × O(log log(1/ε)) iterações = O(n · log log(1/ε))
- **Requer:** ponto inicial perto de L_T e ∇f(x*) ≠ 0

**Gradient flow:**
```
dx/dt = -sign(f(x) - T) · ∇f(x) / ||∇f(x)||
```
- Segue a direção de maior descida/subida até atingir L_T
- Discretização com step size η: converge em O(dist(x₀, L_T)/(η · min||∇f||)) passos

**Implicit function theorem approach:**
- Se ∇f(x*) ≠ 0, o level set é localmente variedade C¹ de dimensão n-1
- Parametrizar localmente e usar continuation ao longo da variedade
- Custo: O(n²) por passo (resolver sistema linear n×n)

---

## 6. A Questão Fundamental e Conexão com BSS P vs NP

### 6.1 Existe algoritmo polinomial em n para TODA f contínua?

**Resposta: NÃO.**

**Teorema (Sikorski 2001, Novak-Woźniakowski 2008):** Para a classe de funções f: [0,1]^n → R Lipschitz com constante L, qualquer algoritmo determinístico que usa avaliações de f como oráculo requer Ω((L/ε)^n) queries no pior caso para garantir |f(x) - T| < ε.

Além disso, **Sikorski (ResearchGate 2001)** prova que para fixed-points de funções não-contrativas em dimensão ≥ 2, o problema tem "complexidade worst-case infinita" — nenhum algoritmo finito resolve o problema para toda função da classe.

### 6.2 Conexão com o Modelo BSS (Blum-Shub-Smale)

**Referências:**
- Blum, L., Shub, M., Smale, S. "On a theory of computation and complexity over the real numbers." Bull. AMS 21, 1–46, 1989.
- Blum, L., Cucker, F., Shub, M., Smale, S. "Complexity and Real Computation." Springer, 1998.
- arXiv:1309.1270. "Satisfiability of cross product terms is complete for real nondeterministic polytime BSS machines."

**O modelo BSS:**
- Máquinas que operam sobre R (números reais com aritmética exata)
- Classes P_R e NP_R definidas analogamente a P e NP discretos
- NP_R-completo: decidir se um polinômio real tem raiz (4-FEAS: polinômio de grau 4)

**Conexão com nosso problema:**

1. **Encontrar raiz de f: R^n → R é NP_R-hard no modelo BSS?**
   - Se f é polinômio de grau ≥ 4: SIM (pela completude de 4-FEAS)
   - Se f é apenas Lipschitz/contínua: o problema não está bem-definido no BSS (requer representação finita de f)

2. **A exponencialidade do black-box se conecta com P_R ≠ NP_R?**
   - O lower bound Ω((1/ε)^n) é um resultado de INFORMATION-BASED complexity (limite de informação)
   - Não é diretamente um resultado de computational complexity no sentido BSS
   - PORÉM: a conjectura τ de Shub-Smale (sobre raízes inteiras de polinômios) implica P_R ≠ NP_R
   - E a dificuldade de encontrar raízes em dimensão alta está no coração dessa conjectura

3. **Analogia precisa:**
   - Bisseção 1D ↔ P (polinomial)
   - Level-set finding em R^n black-box Lipschitz ↔ Exponencial (como busca exaustiva)
   - Convexa em R^n ↔ P (elipsóide é polinomial)
   - A "estrutura" (convexidade) é o que separa P de exponencial, assim como em P vs NP

### 6.3 A Hierarquia de Dificuldade

```
FÁCIL (Polinomial em n)                    DIFÍCIL (Exponencial em n)
─────────────────────────────────────────────────────────────────────
f convexa                                   f Lipschitz geral
f monótona (todas coords)                   f contínua geral  
f separável (Σ fᵢ)                          f com muitos mínimos locais
f com low effective dim                     f com effective dim = n
f com ∇f disponível + boa inicialização    f black-box pura
```

### 6.4 Implicações para o Projeto

**A generalização do CBS para n arbitrário é IMPOSSÍVEL no sentido black-box Lipschitz.**

Caminhos viáveis:
1. **Restringir a classe de funções:** convexidade, monotonicidade, separabilidade
2. **Usar informação adicional:** gradientes, Hessians, estrutura
3. **Aceitar custo exponencial mas com constantes boas:** interval methods, DIRECT para n pequeno
4. **Explorar dimensão efetiva:** se o problema "vive" em subespaço de dimensão baixa
5. **Métodos probabilísticos:** MCMC, Bayesian optimization — sem garantia worst-case, mas bom average-case

---

## 7. Conclusões e Direções para o Projeto

### 7.1 Resumo dos Resultados-Chave

1. **Lower bound exponencial confirmado:** Para f: [0,1]^n → R Lipschitz black-box, encontrar x com |f(x)-T| < ε requer Ω((L/ε)^n) queries. Não existe CBS generalizado eficiente para toda f.

2. **Convexidade quebra a maldição:** Se f é convexa, o custo é O(n² log(1/ε)) — essencialmente o elipsóide como "bisseção n-dimensional".

3. **Monotonicidade parcial ajuda:** Condições de monotonicidade ex-diagonal (Hollender et al. 2023) permitem custo polinomial na precisão.

4. **Low effective dimension é a chave prática:** REMBO e active subspaces permitem resolver problemas nominalmente de dimensão alta se a dependência real é em poucas direções.

5. **Com gradiente, tudo muda:** Newton no level set converge em O(n · log log(1/ε)) — quase linear.

### 7.2 Proposta de Generalização do CBS

**Direção mais promissora: CBS + Elipsóide Híbrido**

Para f convexa:
```
1. Iniciar com elipsóide E₀ = B(x₀, R)
2. Avaliar f(center(E_k))
3. Se f(c) > T: cortar com hiperplano ∇f(c)·(x-c) ≤ 0
4. Se f(c) < T: cortar com hiperplano ∇f(c)·(x-c) ≥ 0
5. Se |f(c) - T| < ε: DONE
6. Atualizar elipsóide E_{k+1} (meia-elipsóide mínima)
```

Este é literalmente o CBS generalizado para convexas:
- Em 1D: degenera para bisseção (cortar intervalo pela metade)
- Em nD: cortar elipsóide por hiperplano → volume reduz por fator (1-1/(n+1))
- Custo: O(n² log(R/ε)) avaliações + gradientes

**Para f não-convexa com effective dimension baixa:**
```
1. Estimar subespaço ativo V via amostragem de gradientes
2. Projetar problema em V (dimensão d_e)
3. Aplicar CBS/DIRECT/BO no espaço reduzido
```

### 7.3 Questões Abertas Relevantes

1. **Hollender et al. 2023:** O caso de d²-d-1 condições de monotonicidade (uma faltando) permanece aberto.
2. **Randomized vs Deterministic:** Para f Lipschitz não-convexa, algoritmos randomizados podem quebrar o lower bound determinístico? (Provavelmente não por mais que fator polinomial.)
3. **Average-case complexity:** Para f "típica" (aleatória), qual é a complexidade? Muito menor que worst-case?
4. **Quantum:** Existe speed-up quântico? Grover dá √ no expoente: O((L/ε)^{n/2}).

---

## 8. Referências Completas

### Livros e Monografias
1. Nemirovski, A.S., Yudin, D.B. (1983). *Problem Complexity and Method Efficiency in Optimization.* Wiley.
2. Sikorski, K. (2001). *Optimal Solution of Nonlinear Equations.* Oxford University Press.
3. Novak, E., Woźniakowski, H. (2008–2012). *Tractability of Multivariate Problems.* EMS, Vols. I–III.
4. Allgower, E.L., Georg, K. (2003). *Introduction to Numerical Continuation Methods.* SIAM.
5. Blum, L., Cucker, F., Shub, M., Smale, S. (1998). *Complexity and Real Computation.* Springer.
6. Moore, R.E. (1966). *Interval Analysis.* Prentice-Hall.
7. Hansen, E. (1992). *Global Optimization Using Interval Analysis.* Marcel Dekker.
8. Kearfott, R.B. (1996). *Rigorous Global Search.* Kluwer.
9. Traub, J.F., Wasilkowski, G.W., Woźniakowski, H. (1988). *Information-Based Complexity.* Academic Press.

### Artigos Fundamentais
10. Eiger, A., Sikorski, K., Stenger, F. (1984). "A bisection method for systems of nonlinear equations." ACM TOMS 10(4), 367–377.
11. Sikorski, K. (1982). "Bisection is optimal." Numer. Math. 40, 111–117.
12. Jones, D.R., Perttunen, C.D., Stuckman, B.E. (1993). "Lipschitzian optimization without the Lipschitz constant." J. Optim. Theory Appl. 79(1), 157–181.
13. Gotovos, A., Casati, N., Hitz, G., Krause, A. (2013). "Active Learning for Level Set Estimation." IJCAI.
14. Lovász, L., Vempala, S. (2006). "Hit-and-run from a corner." SIAM J. Computing 35(4).
15. Hansen, N. (2016). "The CMA Evolution Strategy: A Tutorial." arXiv:1604.00772.

### Artigos Recentes (2020–2026)
16. Bouttier, C., Cesari, T.R., Gerchinovitz, S. (2020). "Regret analysis of the Piyavskii-Shubert algorithm." arXiv:2002.02390.
17. Wang, Z. et al. (2016). "Bayesian Optimization in a Billion Dimensions via Random Embeddings." JAIR 55. arXiv:1301.1942.
18. Hollender, A., Lawrence, C., Segal-Halevi, E. (2023). "Computing approximate roots of monotone functions." arXiv:2310.07333.
19. Kerger, P. (2026). "Closing the Oracle-Complexity Gap in Derivative-Free Convex Optimization." arXiv:2607.13335.
20. Cartis, C., Otemissov, A. (2022). "A dimensionality reduction technique for unconstrained global optimization of functions with low effective dimensionality." arXiv:2003.09673.
21. (2026). "Computational Complexity Analysis of Interval Methods in Solving Uncertain Nonlinear Systems." arXiv:2603.19965.
22. (2024). "Learning the subspace of variation for global optimization of functions with low effective dimension." arXiv:2401.17825.
23. (2025). "Faster Logconcave Sampling from a Cold Start in High Dimension." arXiv:2505.01937.
24. (2024). "Constructing Level Sets Using Smoothed Approximate Bayesian Computation." arXiv:2407.05914.
