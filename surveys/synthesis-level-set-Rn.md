# Síntese: Level-Set Finding em R^n

**Data:** 2026-08-01  
**Fontes:** Survey do Matemático + Survey do Cientista da Computação

---

## 1. Resumo Executivo

### O que se sabe

**Problema:** Dada f: [0,1]^n → R contínua (black-box), encontrar x* com f(x*) = T.

A situação é completamente compreendida em termos de uma **dicotomia abrupta:**

- **Com bracket** (pontos a,b com f(a) < T < f(b)): O(log(1/ε)) avaliações, independente de n. Trivial.
- **Sem bracket, f geral:** Ω((L/ε)^n) avaliações. Maldição da dimensionalidade formal e provada.
- **Sem bracket, f convexa:** Θ(n² log(1/ε)) avaliações. Polinomial. Elipsóide = bisseção n-dimensional.

A dificuldade reside **inteiramente na fase de localização** (encontrar onde f cruza T), nunca na fase de refinamento.

### O que está em aberto

1. **Tight bound para Lipschitz escalar sem bracket:** O lower bound Ω((L/ε)^n) é para o pior caso; qual é a complexidade média?
2. **Monotonicidade parcial:** Hollender et al. provaram que d²-d condições ex-diagonais bastam para polinomial, e d²-d-2 não. O caso d²-d-1 está **aberto**.
3. **Dimensão efetiva:** REMBO funciona para d_e baixa, mas não há algoritmo adaptativo que detecte d_e e aproveite automaticamente com garantias worst-case.
4. **Caso intermediário:** Entre "bracket dado" e "nada conhecido", que informação parcial permite complexidade sub-exponencial?
5. **Randomizado vs. determinístico:** Para Lipschitz não-convexa, randomização melhora o expoente? (Provavelmente não além de fator polinomial.)

---

## 2. Tabela de Complexidade por Classe de Função

| Classe de f | Lower bound | Upper bound | Algoritmo ótimo | Notas |
|---|---|---|---|---|
| Contínua geral (black-box) | Ω(∞) sem bracket | — | Não existe | Pode não ser finito |
| Lipschitz (L), sem bracket | Ω((L/ε)^n) | O((L/ε)^n) | Grid search | **Maldição provada** |
| Lipschitz + bracket dado | Ω(log(1/ε)) | O(log(1/ε)) | Bisseção 1D no segmento | Independente de n |
| Convexa, value oracle | Ω(n²/log n) | O(n² log(1/ε)) | Elipsóide (derivative-free) | **Gap fechado** (Kerger 2026) |
| Convexa, first-order | Ω(n) | O(n log(1/ε)) | Cutting plane / Elipsóide | |
| Monótona em todas coord. | O(n log(1/ε)) | O(n log(1/ε)) | Bisseção coordenada | Linear em n |
| Separável (f = Σfᵢ) | Ω(n log(1/ε)) | O(n log(1/ε)) | n bisseções 1D | Linear em n |
| Low effective dim d_e | Ω((1/ε)^{d_e}) | O(n² · (1/ε)^{d_e}) | REMBO + Active Subspaces | Independe de n nominal |
| C^k (suavidade k) | Ω((1/ε)^{n/k}) | O((1/ε)^{n/k}) | Aprox. polinomial + grid | Ainda exponencial |
| Com ∇f + boa inicialização | — | O(n · log log(1/ε)) | Newton projetado | Convergência local quadrática |
| Polinomial grau ≥ 4 (BSS) | NP_R-hard (decisão) | — | — | Mesmo decidir existência é hard |

---

## 3. A Questão Central: Maldição da Dimensionalidade é Inevitável?

### Resposta curta: Sim, para classes gerais. Não, para classes estruturadas.

### A dicotomia precisa:

**INEVITÁVEL para:**
- f Lipschitz geral (L fixo, n cresce): Θ((L/ε)^n) — provado por Sikorski/Novak-Woźniakowski
- f contínua sem nenhuma estrutura: pode ser infinito
- f polinomial grau ≥ 4 (no sentido BSS): NP_R-hard

**EVITÁVEL para:**
- f convexa: Θ(n²) queries (polinomial!)
- f com monotonicidade parcial suficiente: polinomial (Hollender et al.)
- f separável ou com baixa dimensão efetiva: depende apenas de d_e, não de n
- f com bracket a priori: O(log(1/ε)), completamente independente de n

### O mecanismo da maldição:

A prova é por **argumento adversarial**: uma função Lipschitz pode "esconder" seu level set numa bola de raio ε/L. Existem (L/ε)^n bolas disjuntas no domínio. Sem avaliar cada uma, o adversário posiciona o level set onde nenhuma query caiu.

### O que a quebra:

**Informação global sobre a geometria de f.** Convexidade fornece uma garantia: o subgradiente em qualquer ponto elimina metade (volumetricamente) do espaço de busca. Isso é exatamente o que a bisseção faz em 1D — e o elipsóide faz em nD.

---

## 4. Conexão com P vs NP

### 4.1 No modelo BSS (computação sobre R)

| Aspecto | Resultado |
|---|---|
| Decidir se polinômio grau ≥ 4 tem raiz real | NP_R-completo (Blum-Shub-Smale 1989) |
| Encontrar raiz (quando existe) | Pelo menos tão difícil quanto decidir |
| Conjectura τ (Shub-Smale) | Se verdadeira ⟹ P_R ≠ NP_R |
| 17º problema de Smale (caso complexo) | Resolvido: avg-case polinomial (Lairez 2017) |
| 17º problema de Smale (caso REAL) | **ABERTO** |

### 4.2 Analogia estrutural com P vs NP discreto

```
Busca combinatória (SAT)          Level-set finding em R^n
─────────────────────────────────  ─────────────────────────────────
Busca exaustiva: O(2^n)           Grid search: O((L/ε)^n)
Estrutura (2-SAT, Horn): P         Convexidade: P (elipsóide)
Sem estrutura: NP-hard             Sem estrutura: exp(n)
Certificado verificável em poly    Bracket verificável em O(1)
Propagação de constraints          Subgradiente elimina semi-espaço
DPLL/CDCL: backtracking           Cutting plane: corte iterativo
```

A analogia é profunda: **a "facilidade" de um problema de busca depende de quanta informação local se propaga globalmente.** Em SAT, unit propagation/resolution propaga constraints. Em level-set convexo, o subgradiente propaga informação sobre toda a geometria do sublevel set.

### 4.3 A separação information-complexity vs computational-complexity

O lower bound Ω((L/ε)^n) é de **information-based complexity** (IBC): número mínimo de queries ao oráculo. Isso é mais forte que complexidade computacional BSS porque:
- Não depende do modelo de computação
- Aplica-se mesmo com poder computacional ilimitado entre queries
- É análogo a lower bounds de árvore de decisão em complexidade discreta

A conjectura τ de Shub-Smale vive no mundo de **computational complexity** BSS: mesmo com a função dada explicitamente (não black-box), encontrar raízes inteiras de polinômios pode ser hard.

---

## 5. O que Nosso CBS Pode Contribuir

### 5.1 O gap na literatura

A literatura trata separadamente:
- **IBC/Oracle complexity:** bounds teóricos assintóticos (Sikorski, Novak-Woźniakowski)
- **Otimização convexa:** elipsóide/cutting plane para o caso convexo puro
- **Prática:** CMA-ES, BO, DIRECT — heurísticas sem teoria unificadora

**O que falta:** Uma teoria unificada que:
1. Conecte o CBS 1D (information-optimal com pivô pela mediana) com o elipsóide nD (bisseção volumétrica)
2. Caracterize **exatamente** qual estrutura de f permite generalização eficiente
3. Forneça algoritmos **adaptativos** que detectem a classe de f e escolham a estratégia

### 5.2 Contribuições possíveis do projeto

| Contribuição | Viabilidade | Impacto |
|---|---|---|
| CBS como caso degenerado do elipsóide (unificação teórica) | Alta | Médio — elegante, mas folk knowledge |
| CBS + monotonicidade parcial detectável | Alta | **Alto** — preenche gap de Hollender et al. |
| CBS adaptativo com estimação de d_e on-the-fly | Média | Alto — ponte entre REMBO e busca ativa |
| CBS para f "quase-convexa" (uma noção relaxada) | Média | **Alto** — nova classe tractável |
| Lower bounds para classes intermediárias | Baixa | Muito alto (se conseguir) |
| Implementação competitiva vs. CMA-ES/DIRECT para n=5-20 | Alta | Prático |

### 5.3 A direção mais promissora

**CBS + Elipsóide Híbrido para funções convexas (e extensões):**

O insight fundamental do nosso CBS é: **pivô pela mediana = corte que elimina metade da incerteza.** Em 1D isso é bisseção. Em nD com convexidade, isso é exatamente o elipsóide (corte pelo subgradiente elimina ≥ 1/(n+1) do volume, que é o melhor possível com um hiperplano).

A generalização natural:
1. Manter uma **região de incerteza** R_k (análogo ao intervalo [a_k, b_k] do CBS 1D)
2. Escolher ponto de query que **corta R_k pela mediana** de alguma medida
3. Usar o resultado para atualizar R_k

Isso unifica:
- CBS 1D: R_k = intervalo, mediana = ponto médio
- Elipsóide: R_k = elipsóide, corte por hiperplano na metade volumétrica
- Cutting plane: R_k = politopo

### 5.4 Classe nova a explorar: "f com bracket direcional"

**Definição proposta:** f tem bracket direcional se para alguma direção v ∈ R^n, a restrição t ↦ f(x₀ + tv) é monótona (ou pelo menos muda de sinal). Isso permite aplicar CBS 1D ao longo de v.

**Questão:** Para quais classes de f podemos encontrar v eficientemente?
- f convexa: v = -∇f(x₀) funciona (se f(x₀) > T)
- f com ∇f: v = ∇f/||∇f|| é direção de maior variação
- f black-box: estimação de v via diferenças finitas (custo O(n) queries)

Isso define uma nova classe intermediária entre "bracket dado" (trivial) e "nada" (exponencial).

---

## 6. Próximos Passos Concretos

### Imediatos (1-2 semanas)

1. **Formalizar a unificação CBS-Elipsóide:**
   - Escrever prova de que CBS 1D é caso degenerado (n=1) do elipsóide
   - Definir o "CBS n-dimensional" como elipsóide com pivô informacional
   - Resultado esperado: nota teórica de 3-5 páginas

2. **Implementar CBS-Elipsóide para f convexa:**
   - Input: f black-box convexa, target T, bounds do domínio
   - Estimar gradiente via diferenças finitas (custo 2n queries)
   - Aplicar corte elipsoidal com tracking de volume
   - Benchmark contra CMA-ES e DIRECT para n=2,5,10,20

3. **Testar detecção de dimensão efetiva:**
   - Implementar active subspaces (amostrar gradientes, PCA)
   - Avaliar em funções com d_e conhecido (controle)
   - Combinar com CBS no subespaço reduzido

### Curto prazo (1-2 meses)

4. **Explorar monotonicidade parcial:**
   - Implementar o framework de Hollender et al. para nosso caso escalar
   - Testar: quanta monotonicidade parcial podemos detectar em n queries?
   - Propor critério adaptativo: "se detecto monotonicidade em k coords, uso bisseção coordenada; senão, uso elipsóide"

5. **Definir e estudar a classe "quasi-convexa com ruído":**
   - f é ε-quasi-convexa se ∃g convexa com ||f-g||_∞ < ε
   - O elipsóide funciona com perturbação limitada?
   - Bounds de robustez do CBS-Elipsóide

6. **Lower bound para "bracket direcional":**
   - Quantas queries são necessárias para encontrar uma direção v onde f muda de sinal?
   - Conjectura: O(n) para f Lipschitz com |Im(f)| suficientemente grande

### Médio prazo (3-6 meses)

7. **Paper: "Continuous Binary Search as Volumetric Bisection":**
   - Unificação teórica CBS ↔ Elipsóide
   - Nova classe de complexidade intermediária (bracket direcional)
   - Resultados experimentais vs. estado da arte

8. **Conexão formal com P_R vs NP_R:**
   - Mostrar que CBS-Elipsóide resolve em poly toda instância que está "em P_R"
   - Identificar o boundary exato: qual grau polinomial torna o level-set finding NP_R-hard?
   - Relação com conjectura τ

---

*Síntese produzida em 2026-08-01 a partir dos surveys do Matemático e do Cientista da Computação.*
