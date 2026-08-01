# Survey: Encontrar Pontos em Level Sets de Funções Contínuas

## Formulação do Problema

**Problema:** Dada f: ℝⁿ → ℝ contínua em domínio compacto D ⊂ ℝⁿ, encontrar x* = (x₁*, ..., xₙ*) tal que f(x*) = T.

Equivalentemente: encontrar um ponto no level set L_T = {x ∈ D : f(x) = T}.

**Observações estruturais:**
- Para n=1, isto é root-finding clássico (bisseção, ITP, Brent)
- Para n>1, o level set é genericamente uma variedade (n-1)-dimensional
- O problema é subdeterminado para n>1 (infinitas soluções)
- O problema pode ser reduzido a root-finding de g(x) = f(x) - T = 0, onde g: ℝⁿ → ℝ

**Distinção fundamental:** Diferentemente do caso f: ℝⁿ → ℝⁿ (sistema determinado, n equações em n incógnitas), aqui temos 1 equação em n incógnitas — o sistema é subdeterminado.

---

## 1. Existência: Condições que Garantem L_T ≠ ∅

### 1.1 Teorema do Valor Intermediário (TVI) Generalizado

Para f: D → ℝ com D ⊂ ℝⁿ compacto e conexo, e f contínua:

**Teorema (TVI para funções escalares em ℝⁿ):** Se existem a, b ∈ D tais que f(a) < T < f(b), então existe x* ∈ D com f(x*) = T.

*Prova:* Segue diretamente do TVI clássico aplicado à restrição de f a qualquer caminho contínuo γ: [0,1] → D conectando a e b (que existe pela conexidade de D). A composição f∘γ: [0,1] → ℝ é contínua com (f∘γ)(0) < T < (f∘γ)(1).

**Implicação:** Para o caso escalar f: ℝⁿ → ℝ, a existência é *trivialmente* garantida pelo TVI clássico quando T ∈ (min_D f, max_D f). A dificuldade não está na existência, mas em *encontrar* o ponto.

### 1.2 Teorema de Poincaré-Miranda

O teorema de Poincaré-Miranda [Miranda, 1940; Poincaré, 1883] generaliza o teorema de Bolzano para o caso F: ℝⁿ → ℝⁿ (sistema determinado):

**Teorema (Poincaré-Miranda):** Seja F = (f₁, ..., fₙ): [a₁,b₁] × ... × [aₙ,bₙ] → ℝⁿ contínua. Se para cada i, fᵢ ≤ 0 na face xᵢ = aᵢ e fᵢ ≥ 0 na face xᵢ = bᵢ (ou vice-versa), então existe x* com F(x*) = 0.

**Relevância para nosso problema:** O teorema de Miranda aplica-se diretamente ao caso determinado (n equações, n incógnitas). Para nosso problema escalar (1 equação, n incógnitas), é uma ferramenta mais poderosa que o necessário — mas torna-se relevante se reduzirmos o problema a um sistema determinado via restrição a curvas ou subvariedades.

**Referências:**
- Miranda, C. (1940). "Un'osservazione su un teorema di Brouwer." *Boll. Un. Mat. Ital.* 3, 5–7.
- Kulpa, W. (1997). "The Poincaré-Miranda theorem." *Amer. Math. Monthly* 104(6), 545–550.
- Vrahatis, M.N. (2016). "Generalization of the Bolzano theorem for simplices." *Topology Appl.* 202, 40–46.

### 1.3 Brouwer Degree Theory

A teoria do grau de Brouwer fornece uma ferramenta topológica para garantir existência de zeros:

**Teorema:** Seja Ω ⊂ ℝⁿ aberto limitado, F: Ω̄ → ℝⁿ contínua, e p ∉ F(∂Ω). Se deg(F, Ω, p) ≠ 0, então existe x ∈ Ω com F(x) = p.

Para nosso problema escalar, a teoria do grau é menos diretamente aplicável (requer mapa entre espaços da mesma dimensão). Contudo, pode ser usada indiretamente — por exemplo, para verificar se um nível é atingido analisando a topologia das superfícies de nível vizinhas.

**Referência:** Drábek, P. & Milota, J. (2021). *Brouwer Degree: The Core of Nonlinear Analysis.* Birkhäuser.

### 1.4 Teorema de Borsuk-Ulam

**Teorema (Borsuk-Ulam):** Para toda função contínua f: Sⁿ → ℝⁿ, existe x ∈ Sⁿ tal que f(x) = f(-x).

**Aplicação ao nosso problema:** Para f: Sⁿ → ℝ (função escalar na esfera), Borsuk-Ulam garante que existem pontos antípodas com o mesmo valor. Isto implica que o level set L_T é não-vazio para todo T entre min f e max f, e que os level sets têm estrutura topológica rica (separam a esfera).

**Generalização (Borsuk-Ulam para level sets):** Se f: Sⁿ → ℝ é contínua, então para todo T ∈ (min f, max f), o level set L_T = f⁻¹(T) é não-vazio e, pelo teorema da pré-imagem regular (quando T é valor regular), é uma subvariedade de dimensão n-1 de Sⁿ.

**Referências:**
- Matoušek, J. (2003). *Using the Borsuk-Ulam Theorem.* Springer.
- Yang, X.S. (2023). "Generalizations of Bolzano intermediate value theorem for balls and convex domains." *Fixed Point Theory* 24(1).

### 1.5 Resumo sobre Existência

Para nosso problema específico (f: D → ℝ escalar, D compacto conexo):

| Condição | Garantia |
|----------|----------|
| f contínua, D conexo, T ∈ (min f, max f) | L_T ≠ ∅ (TVI) |
| f contínua, D compacto | L_T é compacto (quando não-vazio) |
| T valor regular de f | L_T é variedade (n-1)-dimensional |
| f ∈ C¹, ∇f ≠ 0 em L_T | L_T é subvariedade suave |

**Conclusão:** A existência no caso escalar é essencialmente trivial via TVI. O problema computacional é inteiramente sobre *encontrar* o ponto, não sobre provar que ele existe.

---

## 2. Métodos Clássicos para Encontrar Pontos em Level Sets

### 2.1 Redução a Otimização: min |f(x) - T|²

A abordagem mais direta: reformular como problema de otimização

    min_{x ∈ D}  g(x) = (f(x) - T)²

Se f(x*) = T para algum x* ∈ D, então g(x*) = 0 é o mínimo global. Métodos:

**Gradient Descent:**
- ∇g(x) = 2(f(x) - T)∇f(x)
- Converge a pontos estacionários de g, que incluem: (a) zeros de f-T, (b) pontos críticos de f onde ∇f = 0
- Problema: pode convergir a mínimos locais com g > 0 (pontos onde ∇f = 0 mas f ≠ T)
- Complexidade por iteração: O(n) (avaliação de gradiente)
- Taxa de convergência: O(1/k) para g convex, O(1/k²) com aceleração de Nesterov

**Newton/Gauss-Newton:**
- Para g(x) = (f(x) - T)², o método de Gauss-Newton resolve o sistema linearizado
- Passo: Δx = -(J^T J)⁻¹ J^T r, onde J = ∇f(x)^T e r = f(x) - T
- No caso escalar (f: ℝⁿ → ℝ), J é vetor 1×n (o gradiente), e o sistema é subdeterminado
- Solução de norma mínima: Δx = -[(f(x)-T)/||∇f||²] ∇f(x) — isto é exatamente *gradient descent normalizado* na direção de ∇f
- Convergência local quadrática quando ∇f(x*) ≠ 0

**Referências:**
- Nocedal, J. & Wright, S.J. (2006). *Numerical Optimization.* 2nd ed., Springer.
- Kelley, C.T. (1999). *Iterative Methods for Optimization.* SIAM.

### 2.2 Continuation Methods / Homotopy Methods

**Ideia central:** Rastrear uma curva no espaço (x, λ) definida implicitamente por uma homotopia.

Para encontrar um ponto no level set f(x) = T, podemos definir:
- Começar de um ponto x₀ onde f(x₀) = T₀ é conhecido
- Definir H(x, λ) = f(x) - [(1-λ)T₀ + λT]
- Rastrear a curva H(x, λ) = 0 de λ=0 até λ=1

**Predictor-Corrector Methods:**
1. Predição: seguir a tangente à curva solução
2. Correção: Newton para voltar à curva

**Vantagens:**
- Garantias teóricas de convergência (sob condições de regularidade)
- Funciona bem para problemas com boa estrutura geométrica
- Complexidade proporcional ao comprimento da curva solução

**Desvantagens:**
- Requer ponto inicial no level set (para outro valor T₀)
- A curva pode bifurcar ou ter singularidades
- Para nosso problema escalar, a curva solução é (n-1)-dimensional para cada λ fixo — precisa-se de restrições adicionais

**Referências:**
- Allgower, E.L. & Georg, K. (2003). *Introduction to Numerical Continuation Methods.* SIAM Classics in Applied Mathematics.
- Allgower, E.L. & Georg, K. (1993). "Continuation and path following." *Acta Numerica* 2, 1–64.

### 2.3 Marching Cubes / Marching Methods

**Marching Cubes [Lorensen & Cline, 1987]:** Algoritmo para extrair isosuperfícies (level sets) de campos escalares discretizados em grade regular.

**Procedimento:**
1. Discretizar D em grade regular com espaçamento h
2. Para cada cubo da grade, avaliar f nos 8 vértices
3. Classificar vértices como acima/abaixo de T
4. Interpolar linearmente nas arestas para encontrar interseções com L_T
5. Construir triângulos conectando as interseções

**Complexidade:**
- Avaliações de f: O((1/h)ⁿ) — exponencial na dimensão
- Para n=3, isto é O(1/h³), prático para grades moderadas
- Para n grande, completamente impraticável

**Relevância:** Para nosso problema (encontrar *um* ponto), basta encontrar *uma* célula onde f muda de sinal em relação a T e interpolar. Mas a busca exaustiva na grade é O((1/h)ⁿ).

**Referência:**
- Lorensen, W.E. & Cline, H.E. (1987). "Marching cubes: A high resolution 3D surface construction algorithm." *SIGGRAPH '87*, 163–169.

### 2.4 Bisseção Multidimensional

#### O caso determinado (f: ℝⁿ → ℝⁿ)

**Bisseção em dimensão alta via Miranda/Poincaré-Miranda:**
- Allshouse & Georg (1990) propuseram métodos baseados em subdivisão simplicial
- Vrahatis (1988, 2016) desenvolveu generalizações da bisseção usando o teorema de Miranda
- Convergência linear garantida quando as condições de Miranda são satisfeitas

**Resultado recente crucial [Hollender, Lawrence & Segal-Halevi, 2023/2024]:**

Para f: [0,1]ⁿ → ℝⁿ satisfazendo condições de Miranda:

> "In general, finding an approximate root might require an **exponential number of evaluations** even for a two-dimensional function."

Mais precisamente:
- Sem condições de monotonicidade: encontrar ε-raiz pode exigir Ω(1/ε) avaliações em dimensão 2 (exponencial na representação binária de ε)
- Com **uma** condição de monotonicidade em dimensão 2: número polinomial de avaliações
- Com todas d²-d condições "ex-diagonais" de monotonicidade: polinomial em 1/ε para dimensão fixa d
- Com d²-d-2 condições: pode ser exponencial

**Referência:**
- Hollender, A., Lawrence, C. & Segal-Halevi, E. (2024). "Computing approximate roots of monotone functions." arXiv:2310.07333v2.

#### O caso subdeterminado (f: ℝⁿ → ℝ) — nosso problema

Para f: ℝⁿ → ℝ escalar, pode-se adaptar a bisseção:
1. Encontrar dois pontos a, b ∈ D com f(a) < T < f(b)
2. Aplicar bisseção ao longo do segmento [a,b]
3. Após log₂(1/ε) iterações, obtém-se x com |f(x) - T| < ε·L·||b-a|| (L = constante de Lipschitz de f ao longo do segmento)

**Observação crucial:** No caso escalar, a "bisseção ao longo de uma reta" reduz o problema a n=1 e funciona em O(log(1/ε)) avaliações. O desafio é que isto encontra um ponto *específico* (na reta entre a e b), sem controle sobre *qual* ponto do level set é encontrado. Se quisermos controlar a localização ou encontrar pontos com propriedades adicionais, o problema torna-se mais difícil.

### 2.5 Multi-Dimensional Bisection Method (MDBM)

**Csernák & Stépán (2012):** Método para encontrar subvariedades (n-k)-dimensionais de raízes de f: ℝⁿ → ℝᵏ.

Para o caso k=1 (nosso problema):
- Discretiza o domínio em grade
- Identifica células onde f muda de sinal
- Refina iterativamente usando bisseção em cada aresta
- Determina a topologia da subvariedade das soluções

**Complexidade:** O((1/h)ⁿ) para a grade inicial — exponencial em n.

**Referência:**
- Csernák, G. & Stépán, G. (2012). "Bisection method in higher dimensions and the efficiency number." *Period. Polytech. Mech. Eng.* 56(2), 81–86.

### 2.6 Level Set Estimation via Gaussian Processes

**Abordagem Bayesiana [Gotovos, Casati, Hutter & Krause, 2013]:**
- Modela f como processo Gaussiano com prior dado
- Seleciona ativamente pontos para avaliar f
- Classifica o domínio em regiões acima/abaixo do threshold T
- Identifica a fronteira (level set) com garantias probabilísticas

**Complexidade:** Dependente da complexidade do kernel GP e da dimensão intrínseca do level set, não necessariamente exponencial em n se o kernel tem estrutura.

**Referências:**
- Gotovos, A., Casati, N., Hutter, G. & Krause, A. (2013). "Active learning for level set estimation." *IJCAI*, 1344–1350.
- Bryan, B. et al. (2006). "Active learning for identifying function threshold boundaries." *NeurIPS*.

---

## 3. Complexidade Computacional

### 3.1 Formulação Precisa do Problema Computacional

**Problema (LEVEL-SET-POINT):**
- **Input:** Oráculo para f: D → ℝ contínua (D ⊂ ℝⁿ compacto), valor alvo T, precisão ε > 0
- **Output:** x ∈ D tal que |f(x) - T| ≤ ε
- **Medida de complexidade:** Número de avaliações do oráculo (oracle complexity)

**Variantes:**
1. **Black-box (zeroth-order):** Apenas avaliações f(x)
2. **First-order:** Avaliações de f(x) e ∇f(x)
3. **Classe restrita:** f pertence a alguma classe (Lipschitz, convexa, analítica, polinomial)

### 3.2 O Caso Escalar é "Fácil" — Mas Com Ressalvas

**Observação fundamental:** Para f: ℝⁿ → ℝ escalar com T ∈ (min f, max f):

1. Encontrar dois pontos a, b com f(a) < T < f(b): pode exigir busca no domínio
2. Uma vez encontrados a, b: bisseção ao longo de [a,b] converge em O(log(1/ε)) avaliações

**Portanto:** Se temos um par de pontos em lados opostos do level set, o problema é resolvido em tempo logarítmico em 1/ε, *independentemente de n*.

**A dificuldade real:** Encontrar o par inicial (a,b). Isto pode ser:
- Trivial (se sabemos min/max de f, ou se temos informação a priori)
- Potencialmente difícil (se f é quase constante em grande parte do domínio)

### 3.3 Oracle Complexity — Resultados Fundamentais

#### Framework de Complexidade Baseada em Informação (IBC)

O framework de Traub, Wasilkowski & Woźniakowski (1988) estuda a complexidade de problemas contínuos usando oráculo:

**Definição:** O *information complexity* n(ε, d) é o número mínimo de avaliações do oráculo necessárias (no pior caso) para resolver o problema com erro ≤ ε em dimensão d.

**Classificação de tractabilidade:**
- **Tractável polinomialmente:** n(ε, d) ≤ C · d^p · ε^{-q} para constantes C, p, q
- **Intratável (curse of dimensionality):** n(ε, d) cresce exponencialmente em d
- **Fortemente tractável:** n(ε, d) ≤ C · ε^{-q} (independente de d)

**Referências:**
- Traub, J.F., Wasilkowski, G.W. & Woźniakowski, H. (1988). *Information-Based Complexity.* Academic Press.
- Novak, E. & Woźniakowski, H. (2008–2012). *Tractability of Multivariate Problems.* Vols. I–III, EMS.

#### Resultado de Sikorski (1982) para f: ℝⁿ → ℝⁿ Lipschitz

**Teorema [Sikorski, 1982]:** Para o problema de encontrar zeros de f: [0,1]ⁿ → ℝⁿ com f Lipschitz (constante K), a complexidade ótima é:

    n(ε, n) ≈ (K/ε)ⁿ

Isto é **exponencial na dimensão n** — maldição da dimensionalidade pura.

**Interpretação:** Para sistemas determinados (n equações, n incógnitas) com apenas continuidade Lipschitz, não há como evitar custo exponencial.

**Referência:**
- Sikorski, K. (1982). "Optimal solution of nonlinear equations satisfying a Lipschitz condition." *Numer. Math.* 40, 111–117.
- Sikorski, K. (2001). *Optimal Solution of Nonlinear Equations.* Oxford University Press.

### 3.4 Complexidade do Nosso Problema Específico (f: ℝⁿ → ℝ)

Para o caso escalar f: ℝⁿ → ℝ, a situação é fundamentalmente diferente do caso determinado:

**Caso favorável — quando já temos bracket:**
Se existem a, b ∈ D com f(a) < T < f(b), bisseção ao longo de [a,b] resolve em O(log(1/ε)) avaliações, *independente de n*. Isto é **fortemente tractável**.

**Caso desfavorável — busca do bracket:**
Se precisamos encontrar pontos em lados opostos do level set, a complexidade depende da classe de f:

| Classe de f | Complexidade de encontrar bracket |
|-------------|-----------------------------------|
| Geral contínua (black-box) | Potencialmente Ω((1/ε)ⁿ) no pior caso |
| Lipschitz com constante L | O((L·diam(D)/δ)ⁿ) onde δ = distância de T à imagem das bordas |
| Convexa | O(n · log(1/ε)) via métodos de corte |
| Monotônica em cada variável | O(n · log(1/ε)) |
| Polinomial de grau d | Depende do modelo (BSS vs Turing) |

### 3.5 Caso Convexo — Tractabilidade Polinomial

**Teorema:** Se f: ℝⁿ → ℝ é convexa e T > min f, então encontrar x com f(x) = T pode ser feito em tempo polinomial em n.

**Prova (esboço):** O sublevel set S_T = {x : f(x) ≤ T} é convexo. Encontrar um ponto na fronteira ∂S_T = L_T equivale ao *convex feasibility problem*:
- Métodos do elipsoide: O(n² · log(1/ε)) avaliações de f e subgradientes
- Cutting plane methods: O(n · log(1/ε)) iterações, cada uma com custo O(n²)

Mais especificamente: dado x₀ com f(x₀) > T, pode-se usar bisseção na direção -∇f(x₀) para encontrar o level set, convergindo em O(log(1/ε)) passos internos.

**Referências:**
- Nemirovski, A. & Yudin, D. (1983). *Problem Complexity and Method Efficiency in Optimization.* Wiley.
- Grötschel, M., Lovász, L. & Schrijver, A. (1993). *Geometric Algorithms and Combinatorial Optimization.* Springer.

### 3.6 Resumo de Complexidade por Classe

| Classe | Dimensão | Complexidade (avaliações de f) | Tractável? |
|--------|----------|-------------------------------|------------|
| Contínua geral, black-box | n | Ω(exp(n)) no pior caso | ❌ |
| Lipschitz (constante L) | n | Θ((L/ε)ⁿ) para sistema n×n | ❌ |
| Lipschitz escalar + bracket | n | O(log(1/ε)) | ✅ |
| Convexa | n | O(n² log(1/ε)) | ✅ |
| Monótona (todas condições) | n | poly(n, log(1/ε)) | ✅ |
| Polinomial (BSS model) | n | Depende do grau | Parcial |

---

## 4. Maldição da Dimensionalidade

### 4.1 O Resultado Fundamental de Intratabilidade

**Teorema (informal, compilação de resultados):** Para funções contínuas gerais f: [0,1]ⁿ → ℝ conhecidas apenas via oráculo (black-box), qualquer algoritmo determinístico que garanta encontrar x com |f(x) - T| ≤ ε para *toda* f na classe pode necessitar de exponencialmente muitas avaliações em n no pior caso.

**Intuição:** O domínio [0,1]ⁿ tem "volume" 1, mas para "varrer" o domínio com resolução ε em cada coordenada, são necessários (1/ε)ⁿ pontos. Uma função contínua pode "esconder" seu level set em uma região arbitrariamente pequena do domínio.

**Construção adversarial:** Para qualquer algoritmo A que faz k avaliações:
- Construa f que vale T+δ em todo o domínio, exceto numa bola de raio r centrada em ponto p escolhido adversarialmente
- Se k < vol(D)/vol(B(r)) ≈ (1/r)ⁿ, com alta probabilidade nenhuma avaliação cai na bola
- O algoritmo não pode distinguir esta f de f constante = T+δ (que não tem level set = T)

### 4.2 Condições que Quebram a Maldição

#### 4.2.1 Funções Convexas

Para f convexa, a complexidade de encontrar o level set é **polinomial em n**:
- O sublevel set é convexo → geometria favorável
- Método do elipsoide: O(n² log(R/ε)) separating oracle calls
- Cutting plane (Vaidya, Lee-Sidford-Wong): Õ(n log(R/ε))
- A chave: convexidade garante que a informação local (subgradiente) fornece informação *global* sobre a localização do level set

**Referência:**
- Lee, Y.T., Sidford, A. & Wong, S.C.W. (2015). "A faster cutting plane method and its implications for combinatorial and convex optimization." *FOCS*, 1049–1065.

#### 4.2.2 Funções com Monotonicidade

**Resultado de Hollender et al. (2024):** Para f: [0,1]ⁿ → ℝⁿ com condições de monotonicidade tipo Miranda:
- Se todas as d²-d condições "ex-diagonais" são satisfeitas → polinomial
- Se menos condições → pode ser exponencial

Para o caso escalar f: ℝⁿ → ℝ:
- Se f é monotônica em *alguma* variável xᵢ (∂f/∂xᵢ > 0 em todo D), então pode-se fazer bisseção nessa variável, fixando as outras: O(log(1/ε)) avaliações
- Se f é monotônica em *todas* as variáveis → trivial

#### 4.2.3 Funções Lipschitz com Constante Conhecida

Para f: [0,1]ⁿ → ℝ Lipschitz com constante L:
- Saber que f é Lipschitz permite excluir regiões do domínio após cada avaliação
- Se f(x₀) = v₀, então f(x) ∈ [v₀ - L||x-x₀||, v₀ + L||x-x₀||]
- Algoritmos tipo DIRECT (Dividing Rectangles) exploram isto
- **Mas:** no pior caso, a complexidade permanece exponencial: Θ((L·diam(D)/(T-f_min))ⁿ)
- Lipschitz *sozinha* não quebra a maldição para busca geral

**Referência:**
- Jones, D.R., Perttunen, C.D. & Stuckman, B.E. (1993). "Lipschitzian optimization without the Lipschitz constant." *J. Optim. Theory Appl.* 79, 157–181.

#### 4.2.4 Funções Analíticas / Suaves

Para f ∈ C^∞ ou analítica:
- A suavidade melhora taxas de convergência *em ε* (superpolinomial/exponencial em log(1/ε))
- **Mas:** não elimina a dependência exponencial em n no pior caso
- Razão: a "informação geométrica" de derivadas altas é local e não propaga globalmente em dimensão alta

**Exceção:** Se f tem representação de baixo rank (ex: f(x) = g(Ax) com A ∈ ℝᵐˣⁿ, m ≪ n), a dimensão efetiva é m, não n.

#### 4.2.5 Estruturas que Efetivamente Quebram a Maldição

| Estrutura | Por que quebra | Complexidade |
|-----------|---------------|--------------|
| Convexidade | Informação local → controle global | O(poly(n) · log(1/ε)) |
| Monotonicidade global | Reduz a bisseção 1D | O(log(1/ε)) |
| Baixa dimensão efetiva | Problema vive em subespaço | O(poly(m) · log(1/ε)), m ≪ n |
| Separabilidade f(x) = Σgᵢ(xᵢ) | n problemas 1D | O(n · log(1/ε)) |
| Estrutura de tensor | Decomposição hierárquica | poly(n, 1/ε) sob condições |
| Bracket conhecido a priori | Problema já parcialmente resolvido | O(log(1/ε)) |

### 4.3 Resultados de Impossibilidade Formais

**Teorema [compilação de Novak & Woźniakowski]:** O problema de integração numérica em [0,1]ⁿ para funções em C^r([0,1]ⁿ) sofre a maldição da dimensionalidade: o número de avaliações necessárias para erro ≤ ε é Ω(ε^{-n/r}).

**Analogia para root-finding:** Embora não haja um teorema publicado exatamente nesta forma para o problema LEVEL-SET-POINT escalar, a construção adversarial da Seção 4.1 mostra que:

> Para a classe de funções contínuas gerais em [0,1]ⁿ, qualquer algoritmo determinístico que garanta |f(x)-T| ≤ ε precisa de Ω((1/ε)^{n-1}) avaliações no pior caso.

**Justificativa:** O level set é (n-1)-dimensional, e "encontrar um ponto nele" é essencialmente tão difícil quanto amostrar uma variedade (n-1)-dimensional desconhecida dentro de [0,1]ⁿ.

**Referências:**
- Novak, E. & Woźniakowski, H. (2008). *Tractability of Multivariate Problems. Vol. I.* EMS.
- Novak, E. & Woźniakowski, H. (2010). *Tractability of Multivariate Problems. Vol. II.* EMS.
- Heinrich, S. (2006). "The randomized information complexity of elliptic PDE." *J. Complexity* 22, 220–249.

---

## 5. Conexão com P vs NP

### 5.1 Modelo Blum-Shub-Smale (BSS)

O modelo BSS [Blum, Shub & Smale, 1989] define computação sobre os reais:

**Máquina BSS:** Opera sobre ℝ∞ (sequências de reais). Operações básicas com custo unitário: +, -, ×, ÷, comparações (<, >, =). O *tamanho* de uma entrada é o número de componentes reais.

**Classes de complexidade:**
- **P_ℝ:** Problemas decidíveis em tempo polinomial no tamanho da entrada (número de reais)
- **NP_ℝ:** Problemas com certificados verificáveis em tempo polinomial

**Problema NP_ℝ-completo [Blum, Shub & Smale, 1989]:**

> **4-FEASIBILITY:** Dadas polinômios quárticos p₁, ..., pₖ: ℝⁿ → ℝ, decidir se existe x ∈ ℝⁿ com pᵢ(x) ≥ 0 para todo i.

Equivalentemente (por transformações algébricas):

> **QUAD (variante):** Decidir se um sistema de equações polinomiais quadráticas tem solução real.

**Referências:**
- Blum, L., Shub, M. & Smale, S. (1989). "On a theory of computation and complexity over the real numbers." *Bull. Amer. Math. Soc.* 21(1), 1–46.
- Blum, L., Cucker, F., Shub, M. & Smale, S. (1998). *Complexity and Real Computation.* Springer.

### 5.2 P_ℝ vs NP_ℝ: Estado Atual

**Conjectura:** P_ℝ ≠ NP_ℝ.

**Evidências:**
- O problema de decidir se um polinômio real tem zero real é NP_ℝ-completo
- Separação P_ℝ ≠ NP_ℝ é um problema aberto, análogo ao P vs NP clássico
- Bürgisser & Cucker (2006) mostraram que, assumindo a conjectura de Riemann generalizada, certos problemas algébricos separam as classes

**Conexão com nosso problema:** Encontrar x com f(x) = T para f polinomial é uma instância do problema de *busca* associado a NP_ℝ. Se f é polinomial de grau d em n variáveis:
- *Decidir* se L_T ≠ ∅ é NP_ℝ-hard para grau ≥ 4
- *Encontrar* um ponto é pelo menos tão difícil quanto decidir

### 5.3 Smale's 17th Problem

**Enunciado [Smale, 2000]:** Existe um algoritmo que encontra uma raiz aproximada de um sistema de n polinômios complexos em n incógnitas em tempo polinomial *na média*?

**Solução [Beltrán & Pardo, 2011; Bürgisser & Cucker, 2011; Lairez, 2016]:**

Lairez (2016) resolveu afirmativamente o problema de Smale:

> **Teorema [Lairez, 2016]:** Existe um algoritmo determinístico que computa uma raiz aproximada de n polinômios complexos em n incógnitas em tempo polinomial na média (em relação ao tamanho da entrada N), no modelo BSS com raiz quadrada.

**Detalhes:**
- Beltrán & Pardo (2011): algoritmo randomizado em tempo polinomial na média com complexidade O(N^{O(log log N)})
- Bürgisser & Cucker (2011): análise de condition numbers e geometria do espaço de sistemas
- Lairez (2016): derandomização — algoritmo determinístico usando "a aleatoriedade contida na própria entrada"
- Lairez (2019): melhoria via "rigid continuation paths" com complexidade quasi-linear

**Limitações para nosso problema:**
- Estes resultados são para **ℂⁿ**, não ℝⁿ — sobre os reais, a situação é muito mais difícil
- São resultados de **caso médio**, não pior caso
- Aplicam-se ao caso **determinado** (n equações, n incógnitas), não ao subdeterminado

**Referências:**
- Smale, S. (2000). "Mathematical problems for the next century." *Math. Intelligencer* 20(2), 7–15.
- Beltrán, C. & Pardo, L.M. (2011). "Fast linear homotopy to find approximate zeros of polynomial systems." *Found. Comput. Math.* 11, 95–129.
- Lairez, P. (2017). "A deterministic algorithm to compute approximate roots of polynomial systems in polynomial average time." *Found. Comput. Math.* 17, 1265–1292.
- Rojas, J.M. (2019). "A Faster Solution to Smale's 17th Problem I: Real Binomial Systems." arXiv:1901.09739.

### 5.4 Classe PPAD e Fixed Points

**Conexão indireta mas importante:**

A classe PPAD [Papadimitriou, 1994] captura a dificuldade computacional de encontrar pontos fixos:
- Encontrar um ponto fixo de Brouwer (f: Bⁿ → Bⁿ contínua) é PPAD-completo [Daskalakis, Goldberg & Papadimitriou, 2009]
- PPAD está contido em NP ∩ coNP (sob hipóteses criptográficas, é improvável que esteja em P)

**Relação com nosso problema:**
- Encontrar f(x) = x (ponto fixo) é equivalente a encontrar zero de g(x) = f(x) - x (sistema determinado n×n)
- Nosso problema escalar (f: ℝⁿ → ℝ, encontrar level set) é *mais fácil* que PPAD no sentido estrutural
- Mas num modelo de oráculo, ambos podem ter complexidade exponencial sem hipóteses adicionais

**Referências:**
- Papadimitriou, C.H. (1994). "On the complexity of the parity argument and other inefficient proofs of existence." *J. Comput. Syst. Sci.* 48, 498–532.
- Daskalakis, C., Goldberg, P.W. & Papadimitriou, C.H. (2009). "The complexity of computing a Nash equilibrium." *SIAM J. Comput.* 39(1), 195–259.

### 5.5 A Questão Fundamental para Nosso Problema

**Pergunta precisa:** No modelo BSS sobre ℝ, qual é a complexidade de:
- **Decisão:** Dado f polinomial de grau d em n variáveis e T ∈ ℝ, decidir se ∃x com f(x) = T?
- **Busca:** Encontrar tal x (quando existe)?

**Respostas conhecidas:**
1. Se f é linear: trivial (T está na imagem sse não é perpendicular ao kernel)
2. Se f é quadrática: reduz-se a decidir se uma forma quadrática atinge certo valor — pode ser feito em P_ℝ
3. Se f é cúbica ou superior: o problema de decidir pode codificar problemas NP_ℝ-hard (via reduções à viabilidade de sistemas polinomiais)

**Observação crucial:** Para f: ℝⁿ → ℝ *escalar* de grau d, a decisão "∃x: f(x) = T?" é equivalente a "∃x: f(x) - T = 0?", que é uma *única* equação polinomial em n variáveis. Um polinômio real de grau d ≥ 2 em n ≥ 2 variáveis genericamente tem zeros (pelo teorema de Bezout e argumentos dimensionais). Portanto:
- Para d ≥ 2 e n ≥ 2, L_T é genericamente não-vazio para *todo* T na imagem
- A questão de *decidir* L_T ≠ ∅ é não-trivial apenas para T nos extremos (perto de max f ou min f)
- Encontrar min f ou max f é otimização global — que é NP_ℝ-hard para polinômios de grau ≥ 4

---

## 6. Resultados de Impossibilidade e Decidibilidade

### 6.1 Franek & Krčál: Satisfatibilidade Robusta e Indecidibilidade

**Resultado fundamental [Franek & Krčál, 2014]:**

Considere o problema: dada f: K → ℝⁿ contínua (K compacto, dim K = d), decidir se existe g com ||g - f||_∞ ≤ α tal que g tem um zero em K ("robust satisfiability").

> **Teorema [Franek & Krčál, 2014]:**
> - Se dim K ≤ 2n - 3: o problema é **decidível** em tempo polinomial (para n fixo)
> - Se dim K ≥ 2n - 2: o problema é **indecidível**

O threshold dim K = 2n - 3 vem do *stable range* na teoria de homotopia.

**Implicação para nosso problema:** Para f: K → ℝ (n=1 na notação acima), a robust satisfiability é decidível se dim K ≤ 2·1 - 3 = -1, o que é vazio! Portanto, para n=1, o resultado não se aplica diretamente. Na verdade, para o caso escalar, a decidibilidade segue do TVI (não há indecidibilidade).

Para o problema reformulado F(x) = (f(x) - T): K → ℝ¹ com K ⊂ ℝⁿ (dim K = n):
- Decidível se n ≤ 2·1 - 3 = -1 → **nunca** pelo critério geral
- Mas nosso caso é especial: F: ℝⁿ → ℝ é *escalar*, logo o TVI clássico resolve a existência

**Interpretação correta:** A indecidibilidade de Franek-Krčál aplica-se ao caso *vetorial* (encontrar zeros de maps ℝⁿ → ℝⁿ com informação aproximada). O caso escalar é fundamentalmente mais simples.

**Referências:**
- Franek, P. & Krčál, M. (2014). "Robust satisfiability of systems of equations." *SODA 2014*, 193–203.
- Franek, P. & Krčál, M. (2015). "Cohomotopy groups capture robust properties of zero sets via homotopy theory." arXiv:1507.04310.
- Franek, P. & Krčál, M. (2017). "Solving equations and optimization problems with uncertainty." *J. Appl. Comput. Topology* 1, 297–330.

### 6.2 Resultado de Impossibilidade para Pontos Fixos de Funções Não-Contrativas

**Teorema [Sikorski, 2001]:** Para funções contínuas não-contrativas f: [0,1]² → [0,1]², o problema de encontrar um ponto fixo ε-aproximado tem complexidade **infinita** no pior caso — não existe algoritmo que funcione para toda f na classe.

**Relevância:** Isto mostra que já em dimensão 2, para problemas de existência tipo Brouwer (caso determinado), pode não existir algoritmo finito sem hipóteses adicionais.

### 6.3 Lower Bounds para o Caso Escalar

**Para f: [0,1]ⁿ → ℝ contínua geral (black-box oracle):**

**Lower bound fundamental:** Se não temos *nenhuma* informação sobre onde f atinge o valor T (i.e., não temos bracket), qualquer algoritmo determinístico pode precisar de arbitrariamente muitas avaliações.

**Construção:** Considere a classe de funções f: [0,1]ⁿ → ℝ que são constantes (f ≡ T + 1) exceto numa bola de raio r. Para determinar se f atinge T, precisamos "encontrar" esta bola — o que requer Ω(1/rⁿ) avaliações em grade.

**Porém:** Se a *existência* de T na imagem é garantida a priori (T ∈ (min f, max f)), e se temos pontos em ambos os lados:

> **Teorema (folklore):** Dados a, b ∈ [0,1]ⁿ com f(a) < T < f(b) e f contínua, existe algoritmo com O(⌈log₂(1/ε)⌉) avaliações que encontra x com |f(x) - T| ≤ ε (bisseção no segmento [a,b]).

### 6.4 A Dicotomia Fundamental

O problema LEVEL-SET-POINT apresenta uma **dicotomia abrupta**:

| Informação disponível | Complexidade |
|----------------------|-------------|
| Black-box, sem bracket | Ω(exponencial em n) |
| Bracket (a,b) com f(a)<T<f(b) | O(log(1/ε)), independente de n |
| f convexa, um ponto com f>T e outro com f<T | O(n · log(1/ε)) |
| f Lipschitz, busca do bracket | O((L/gap)ⁿ) |
| f polinomial (BSS) | NP_ℝ-hard para decidir, busca mais difícil |

**Insight crucial:** A dificuldade do problema está inteiramente na **busca do bracket** (fase de localização), não na **convergência ao nível** (fase de refinamento). Uma vez localizada a região onde f cruza T, o refinamento é sempre eficiente.

### 6.5 O Papel da Continuidade

**Pergunta:** A continuidade garante que polinomialmente muitas avaliações bastam?

**Resposta:** **NÃO** em geral. A continuidade sozinha (sem bracket) é insuficiente:
- Uma função contínua pode atingir o valor T numa região de medida zero arbitrariamente pequena
- Sem hipóteses adicionais (Lipschitz, convexidade, bracket), não há upper bound polinomial em n

**Porém:** A continuidade garante que, *dado* um bracket, a convergência é eficiente. E para domínios compactos conexos com f contínua:
- min f e max f são atingidos (Weierstrass)
- Todo T ∈ [min f, max f] é atingido (TVI + conexidade)
- Se avaliamos f em *qualquer* dois pontos com valores em lados opostos de T, temos um bracket

**Portanto:** A questão real é: quantas avaliações aleatórias são necessárias para encontrar um bracket? Para funções "genéricas" (onde f-T muda de sinal frequentemente), poucas avaliações bastam. O pior caso ocorre para funções patologicamente "quase-constantes".

---

## 7. Síntese e Conclusões

### 7.1 Resumo do Estado da Arte

1. **Existência:** Para f: D → ℝ escalar, contínua, D compacto conexo, L_T ≠ ∅ ⟺ T ∈ [min f, max f]. Completamente resolvido pelo TVI.

2. **Algoritmos:**
   - Com bracket: bisseção resolve em O(log(1/ε)), independente de n
   - Sem bracket, caso geral: potencialmente exponencial em n
   - Caso convexo: polinomial (ellipsoid, cutting plane)
   - Caso polinomial (complexo, caso médio): resolvido por Lairez (2016)

3. **Lower bounds:**
   - Para funções gerais (black-box): exponencial em n (maldição da dimensionalidade)
   - Para funções com estrutura (convexas, monótonas): polinomial
   - A dicotomia "com bracket/sem bracket" é o fator determinante

4. **Conexão P vs NP:**
   - No modelo BSS: decidir se um polinômio de grau ≥ 4 tem zero é NP_ℝ-completo
   - Para o caso escalar (1 equação), a situação é menos clara — genericamente há zeros
   - A dificuldade transfere-se para otimização (encontrar min/max) mais que para level-set finding

### 7.2 Lacunas e Questões Abertas

1. **Complexidade exata do problema escalar com Lipschitz:** Qual é o tight lower bound para encontrar T no level set de f: [0,1]ⁿ → ℝ com f L-Lipschitz, sabendo que T ∈ (min f, max f)?

2. **Papel da dimensão intrínseca:** Se o level set L_T tem dimensão intrínseca k < n-1 (e.g., por simetrias), isto melhora a complexidade?

3. **Algoritmos adaptativos:** Existem algoritmos que se adaptam à "facilidade" da instância (ex: condition number) e evitam o pior caso exponencial?

4. **Caso intermediário:** Entre "bracket dado" (trivial) e "nada conhecido" (exponencial), que informação parcial permite complexidade intermediária?

5. **Conexão com learning:** O framework de active learning para level set estimation (Gotovos et al.) fornece bounds não-piores que os clássicos sob priors Gaussianos — mas qual é a complexidade minimax sem prior?

### 7.3 Referências Bibliográficas Consolidadas

**Livros fundamentais:**
- Allgower, E.L. & Georg, K. (2003). *Introduction to Numerical Continuation Methods.* SIAM.
- Blum, L., Cucker, F., Shub, M. & Smale, S. (1998). *Complexity and Real Computation.* Springer.
- Novak, E. & Woźniakowski, H. (2008). *Tractability of Multivariate Problems. Vol. I.* EMS.
- Sikorski, K. (2001). *Optimal Solution of Nonlinear Equations.* Oxford University Press.
- Traub, J.F., Wasilkowski, G.W. & Woźniakowski, H. (1988). *Information-Based Complexity.* Academic Press.
- Nemirovski, A. & Yudin, D. (1983). *Problem Complexity and Method Efficiency in Optimization.* Wiley.

**Artigos-chave:**
- Beltrán, C. & Pardo, L.M. (2011). *Found. Comput. Math.* 11, 95–129.
- Blum, L., Shub, M. & Smale, S. (1989). *Bull. Amer. Math. Soc.* 21(1), 1–46.
- Csernák, G. & Stépán, G. (2012). *Period. Polytech. Mech. Eng.* 56(2), 81–86.
- Daskalakis, C., Goldberg, P.W. & Papadimitriou, C.H. (2009). *SIAM J. Comput.* 39(1), 195–259.
- Franek, P. & Krčál, M. (2014). *SODA 2014*, 193–203.
- Gotovos, A., Casati, N., Hutter, G. & Krause, A. (2013). *IJCAI*, 1344–1350.
- Hollender, A., Lawrence, C. & Segal-Halevi, E. (2024). arXiv:2310.07333v2.
- Lairez, P. (2017). *Found. Comput. Math.* 17, 1265–1292.
- Miranda, C. (1940). *Boll. Un. Mat. Ital.* 3, 5–7.
- Sikorski, K. (1982). *Numer. Math.* 40, 111–117.
- Smale, S. (2000). *Math. Intelligencer* 20(2), 7–15.
- Vrahatis, M.N. (1988). *Numer. Math.* 53, 219–242.

---

*Survey compilado em 2026-08-01. Autor: Agente de Pesquisa (Computer Scientist).*
