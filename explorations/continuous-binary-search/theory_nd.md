# CBS como Biseção Volumétrica: Formalização e Generalização para R^n

**Data:** 2026-08-01  
**Status:** Em desenvolvimento

## 1. Tese Principal

> O algoritmo CBS-1D (Continuous Binary Search com pivô pela mediana ponderada) é a degenerescência 1-dimensional do método cutting plane / elipsóide. A generalização natural para R^n herda as garantias de convergência do cutting plane com taxa de redução volumétrica O(1 - 1/(n+1)) por iteração.

## 2. Framework Unificado

### Definição 1 (Biseção Volumétrica Generalizada)

Seja D₀ ⊂ R^n um domínio compacto convexo contendo a solução x* (onde f(x*) = T). Uma **biseção volumétrica** é uma sequência de domínios D₀ ⊃ D₁ ⊃ D₂ ⊃ ... tal que:

1. **Separating oracle:** Em cada iteração k, dado um ponto de query qₖ ∈ Dₖ, o oráculo retorna um semi-espaço H⁺ₖ contendo x*.
2. **Update:** Dₖ₊₁ = Dₖ ∩ H⁺ₖ
3. **Redução:** μ(Dₖ₊₁) ≤ ρ · μ(Dₖ) para alguma medida μ e taxa ρ < 1.

### Definição 2 (Separating Oracle para Level Sets)

Para f: D → R contínua com f(x*) = T:
- Se f é **convexa** e T ∈ im(f): qualquer subgradiente g ∈ ∂f(qₖ) define um separating hyperplane válido quando f(qₖ) ≠ T.
- Se f é **monótona na direção do gradiente**: ∇f(qₖ) define a direção do corte.

**Oracle concreto:** Avaliar f(qₖ).
- Se f(qₖ) > T e f convexa: x* ∈ {x : ⟨∇f(qₖ), x - qₖ⟩ ≤ 0}
- Se f(qₖ) < T e f convexa: x* ∈ {x : ⟨∇f(qₖ), x - qₖ⟩ ≥ 0}

## 3. CBS-1D como Cutting Plane

### Teorema 1 (Equivalência em 1D)

*O CBS-1D com pivô pela mediana da distribuição w(x) = 1/(|f(x)-T| + δ) é uma biseção volumétrica com:*
- *Domínio: intervalo Dₖ = [aₖ, bₖ]*
- *Medida: μ_w(A) = ∫_A w(x)dx / ∫_D w(x)dx (medida ponderada)*
- *Ponto de query: qₖ = mediana de μ_w em [aₖ, bₖ]*
- *Taxa de redução: ρ = 1/2 (exata)*

**Demonstração:**

(i) O ponto de query é qₖ = med(μ_w), i.e., μ_w([aₖ, qₖ]) = μ_w([qₖ, bₖ]) = 1/2.

(ii) O separating oracle avalia f(qₖ) e usa monotonicidade (ou o fato de que estamos num ramo monótono) para determinar se x* ∈ [aₖ, qₖ] ou x* ∈ [qₖ, bₖ].

(iii) O domínio atualizado Dₖ₊₁ tem medida μ_w(Dₖ₊₁) = 1/2 · μ_w(Dₖ).

(iv) Portanto ρ = 1/2. Após K iterações: μ_w(Dₖ) = 2⁻ᴷ · μ_w(D₀). ∎

### Corolário 1.1 (Bisseção clássica)

*Para w(x) = 1 (prior uniforme), a mediana é o ponto médio e μ_w = medida de Lebesgue normalizada. Neste caso CBS-1D = bisseção clássica.*

### Corolário 1.2 (Vantagem do CBS sobre bisseção)

*O CBS com w(x) = 1/(|f(x)-T| + δ) converge em menos iterações que a bisseção porque cada corte elimina mais COMPRIMENTO do que metade:*

Se a distribuição w concentra massa perto de x*, então a mediana está perto de x*, e o corte elimina uma grande fração do comprimento do intervalo de cada vez. Formalmente:

Seja λ(Dₖ₊₁)/λ(Dₖ) a fração de comprimento retida. Então:
- Bisseção: λ(Dₖ₊₁)/λ(Dₖ) = 1/2 (sempre)
- CBS: λ(Dₖ₊₁)/λ(Dₖ) pode ser << 1/2 (quando w concentra massa longe do corte)

Porém o custo é maior por iteração (quadratura para computar mediana).

## 4. Generalização para R^n

### Definição 3 (CBS-nD)

**CBS-nD** é uma biseção volumétrica em R^n com:
- Medida adaptativa: w(x) = 1/(|f(x) - T| + δ) sobre D ⊂ R^n
- Ponto de query: centróide (ou mediana geométrica) da distribuição w em Dₖ
- Direção de corte: ∇f(qₖ) (estimado por diferenças finitas se necessário)
- Hiperplano: H = {x : ⟨∇f(qₖ), x - qₖ⟩ = 0}
- Semi-espaço retido: baseado no sinal de f(qₖ) - T

### Teorema 2 (Convergência do CBS-nD para f convexa)

*Seja f: D → R convexa e contínua, D ⊂ R^n convexo compacto, T ∈ (min_D f, max_D f). O CBS-nD com corte pelo centróide converge com taxa:*

$$\mu_w(D_{k+1}) \leq \left(1 - \frac{1}{n+1}\right) \cdot \mu_w(D_k)$$

*Após K iterações:*

$$\mu_w(D_K) \leq \left(1 - \frac{1}{n+1}\right)^K \cdot \mu_w(D_0)$$

*Para garantir μ_w(Dₖ) ≤ ε/μ_w(D₀), precisa-se de:*

$$K \geq (n+1) \ln\left(\frac{\mu_w(D_0)}{\varepsilon}\right)$$

**Demonstração (esboço):**

Pelo teorema de Grünbaum (1960): qualquer hiperplano passando pelo centróide de um convexo K divide-o em duas partes com volumes entre (n/(n+1))^n · vol(K) e (1 - (n/(n+1))^n) · vol(K). Para a medida ponderada w, o centróide de w garante que o corte remove pelo menos fração 1/(n+1) da massa. ∎

### Corolário 2.1 (Complexidade em avaliações)

*Para CBS-nD com f convexa:*
- Iterações: O(n · ln(R/ε)) onde R = diam(D₀)
- Avaliações por iteração: 2n+1 (gradiente por dif. finitas + 1 eval no centróide)
- **Total: O(n² · ln(R/ε)) avaliações de f**

### Corolário 2.2 (Comparação com elipsóide)

| Método | Iterações | Evals por iter | Total evals |
|--------|-----------|----------------|-------------|
| Elipsóide (Khachiyan) | O(n² ln(R/ε)) | 1 + subgradiente | O(n² ln(R/ε)) |
| Cutting plane (centróide) | O(n ln(R/ε)) | 1 + subgradiente | O(n ln(R/ε)) |
| **CBS-nD (Box)** | O(n log(R/ε)) | 2n+1 (dif. finitas) | **O(n² log(R/ε))** |

CBS-nD Box é competitivo com o elipsóide e não requer subgradiente explícito.

## 5. Caso n=1: Verificação

Para n=1:
- Iterações: O(1 · ln(R/ε)) = O(log(R/ε)) ✓
- Evals por iter: 2·1+1 = 3 (ou 65 se usar quadratura para mediana) 
- Redução: (1 - 1/2) = 1/2 ✓ (pois 1/(n+1) = 1/2 para n=1)

Recuperamos exatamente o CBS-1D!

## 6. Hierarquia de Classes

```
f: R^n → R, encontrar x* com f(x*) = T
═══════════════════════════════════════

EXPONENCIAL (inevitável):
  Lipschitz geral: Θ((L/ε)^n)
  Polinômio grau ≥ 4: NP_R-completo (BSS)

POLINOMIAL (maldição evitável):
  Convexa (sem gradiente): O(n² log(R/ε))        ← CBS-nD Box
  Convexa (com gradiente): O(n log(R/ε))          ← CBS-nD Elipsóide
  Monótona por coordenada: O(n log(R/ε))          ← Bisseção alternada
  Separável f = Σ fᵢ(xᵢ): O(n log(W/ε))         ← n × CBS-1D
  Low effective dim d_e: O(d_e² log(R/ε) + n·d_e) ← Active subspaces + CBS

LOGARÍTMICA (caso ideal):
  n=1, com bracket: O(log(W/ε))                   ← CBS-1D / ITP
```

## 7. Conexão com P vs NP (Modelo BSS)

### Observação Central

No modelo BSS (Blum-Shub-Smale) sobre os reais:

- **P_R:** Problemas decidíveis em tempo polinomial em n (dimensão do input)
- **NP_R:** Problemas com certificado verificável em poly(n)
- **NP_R-completo:** Decidir se polinômio de grau ≥ 4 tem raiz real

**Nosso resultado:**
- Encontrar x com f(x) = T para f **convexa** está em P_R (custo O(n² log(R/ε)))
- Encontrar x com f(x) = T para f **Lipschitz geral** requer custo exponencial

**Analogia:**
```
SAT (discreto)              ↔  Level-set finding (contínuo)
2-SAT ∈ P                  ↔  f convexa → O(poly(n))
3-SAT é NP-completo        ↔  f grau ≥ 4 → NP_R-completo
Unit propagation em SAT     ↔  Subgradiente em cutting plane
Propagação local → global   ↔  Informação local → restrição global
```

### Conjectura (Informal)

> A fronteira entre "busca eficiente" (polinomial) e "busca inevitavelmente exponencial" é caracterizada pela capacidade de uma avaliação local (f(q) e ∇f(q)) de produzir informação GLOBAL sobre a localização do level set. Para funções convexas, o subgradiente produz um separating hyperplane global. Para funções gerais, a informação local não propaga.

## 8. Variante CBS-Box (Para Implementação)

A variante mais simples para implementar primeiro:

```
CBS-nD-Box(f, T, bounds[n][2], ε):
  // bounds[i] = [aᵢ, bᵢ]
  
  Repetir até convergência:
    // 1. Centro do box (ou centróide ponderado)
    Para i = 0..n-1:
      q[i] = (bounds[i][0] + bounds[i][1]) / 2
    
    // 2. Avaliar f e verificar convergência
    fq = f(q)
    Se |fq - T| < ε: retornar q
    
    // 3. Gradiente por diferenças finitas
    Para i = 0..n-1:
      h = (bounds[i][1] - bounds[i][0]) × 1e-6
      grad[i] = (f(q + h·eᵢ) - f(q - h·eᵢ)) / (2h)
    
    // 4. Escolher coordenada de corte (maior |grad|)
    j = argmax_i |grad[i]|
    
    // 5. Corte: se fq > T e grad[j] > 0, solução está à esquerda de q[j]
    Se (fq > T) XOR (grad[j] < 0):
      bounds[j][1] = q[j]   // corta metade superior na coordenada j
    Senão:
      bounds[j][0] = q[j]   // corta metade inferior na coordenada j
    
  Retornar q
```

**Complexidade:** O(n · log(R/ε)) iterações × O(n) evals/iter = **O(n² log(R/ε))** total.

## Referências

1. Grünbaum, B. (1960). "Partitions of mass-distributions and convex bodies by hyperplanes." Pacific J. Math.
2. Khachiyan, L. (1979). "A polynomial algorithm in linear programming." Soviet Math. Doklady.
3. Nemirovsky, A. & Yudin, D. (1983). "Problem Complexity and Method Efficiency in Optimization."
4. Vaidya, P. (1996). "A new algorithm for minimizing convex functions over convex sets." Math. Programming.
5. Blum, L., Shub, M. & Smale, S. (1989). "On a theory of computation and complexity over the real numbers." Bull. AMS.
6. Oliveira, I.F.D. & Takahashi, R.H.C. (2021). "An Enhancement of the Bisection Method Average Performance Preserving Minmax Optimality." ACM TOMS.
7. Sikorski, K. (2001). "Optimal Solution of Nonlinear Equations." Oxford University Press.
