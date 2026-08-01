# Busca Binária Contínua com Pivô pela Mediana

**Data:** 2026-08-01  
**Status:** Em desenvolvimento  
**Sessão:** session-001

## 1. Motivação

Na busca binária discreta, o pivô é o elemento central do vetor ordenado. A cada iteração, metade do espaço é descartada. Queremos generalizar isso para funções contínuas f: [a,b] → ℝ:

- **Qual é o pivô ótimo no contínuo?**
- **Como garantir que a busca funciona quando f não é monótona?**

## 2. Definições

### 2.1 O Problema

Dada uma função contínua f: [a,b] → ℝ e um valor alvo T ∈ Im(f), encontrar x* ∈ [a,b] tal que f(x*) = T.

### 2.2 Analogia Discreta → Contínuo

| Busca binária discreta | Versão contínua |
|---|---|
| Vetor ordenado A[1..n] | Função contínua f: [a,b] → ℝ (monótona no intervalo de busca) |
| Alvo t | Valor alvo T ∈ Im(f) |
| Pivô = A[n/2] | Pivô = ponto x_p ∈ (a,b) |
| Comparar A[m] com t | Comparar f(x_p) com T |
| Descartar metade | Substituir [a,b] por [a,x_p] ou [x_p,b] |

### 2.3 Mediana como Pivô

Dada uma função peso w(x) ≥ 0 no intervalo [a,b], a **mediana** é o ponto x_m tal que:

$$\int_a^{x_m} w(x)\,dx = \int_{x_m}^b w(x)\,dx = \frac{1}{2}\int_a^b w(x)\,dx$$

**Interpretação:** x_m divide a "massa" ou "informação" em duas partes iguais.

## 3. Escolha do Pivô

### 3.1 Pivô Geométrico (Ponto Médio)

x_p = (a + b) / 2

- Ótimo no **pior caso** (minimax)
- Assume distribuição uniforme de incerteza sobre x*
- Reduz intervalo por fator 2 a cada iteração
- Convergência: |I_k| = (b-a) / 2^k

### 3.2 Pivô pela Mediana (Divisão de Massa)

x_p tal que ∫_a^{x_p} π(x)dx = 1/2

onde π(x) é a distribuição de probabilidade (prior) sobre a localização de x*.

- Ótimo **em expectativa** (Bayes-ótimo)
- Extrai exatamente **1 bit de informação** por query (máximo possível)
- Para prior uniforme, coincide com o ponto médio
- Convergência: H(π_k) = H(π_0) - k bits

### 3.3 Resultado de Otimalidade

**Teorema:** O pivô que maximiza a informação ganha (entropia de Shannon) por avaliação é a mediana da distribuição de incerteza. Isso decorre de:

- Cada avaliação produz resposta binária (esquerda/direita)
- Entropia binária H(p) = -p·log(p) - (1-p)·log(1-p) é máxima em p = 1/2
- P(esquerda) = 1/2 ⟺ x_p é mediana de π

### 3.4 Trade-off

| Pivô | Convergência | Custo por iteração | Garantia |
|---|---|---|---|
| Ponto médio | log₂((b-a)/ε) | O(1) | Pior caso |
| Mediana do prior | ≤ log₂((b-a)/ε) | Requer integração | Em expectativa |
| Newton/interpolação | ~log₂(log₂(1/ε)) | Requer f'(x) | Local (sem garantia global) |

## 4. Monotonicidade e Seleção de Ramo

### 4.1 O Problema da Não-Monotonicidade

A busca binária requer que a decisão "esquerda/direita" seja determinística. Para f monótona, comparar f(x_p) com T determina unicamente o lado. Para f não-monótona (ex: Gaussiana), f(x_p) < T não garante de que lado está x*.

### 4.2 Decomposição em Ramos Monótonos

**Definição:** Um *ramo monótono* de f é um sub-intervalo maximal [c,d] ⊆ [a,b] onde f é estritamente monótona (crescente ou decrescente).

Para f unimodal com máximo em x_max:
- Ramo esquerdo: [a, x_max] — estritamente crescente
- Ramo direito: [x_max, b] — estritamente decrescente

### 4.3 "Dar uma Direção" = Escolher um Ramo

**Definição formal:** Uma *direção* para o par (f, T) é um subconjunto conexo S ⊆ [a,b] tal que:
1. f|_S é estritamente monótona
2. T ∈ f(S) (a solução existe no ramo)

**Teorema (existência):** Se f é contínua e T ∈ Im(f), existe pelo menos uma direção para (f, T).

**Escolha para "qualquer x*":** Se aceitamos qualquer solução, basta encontrar UM ramo monótono que contenha uma solução e buscar nele.

### 4.4 Resultado de Impossibilidade

**Teorema:** Não existe transformação contínua φ: ℝ → ℝ que torne φ∘f globalmente monótona E preserve as raízes de f(x) = T, quando f assume T em mais de um ponto.

**Consequência:** A abordagem correta é **restringir o domínio**, não transformar a função.

## 5. Condições para o Algoritmo Funcionar

### 5.1 Condição Suficiente (Classe A)

Se f ∈ C¹ e f'(x*) ≠ 0, o algoritmo funciona. Razão: existe vizinhança de x* onde f é estritamente monótona. Após ⌈log₂((b-a)/d)⌉ iterações (onde d = distância de x* ao ponto crítico mais próximo), o intervalo é inteiramente monótono.

### 5.2 Caso Limite (Classe B)

Se x* é extremo local (f'(x*) = 0, T = f(x*)), o problema muda de natureza: vira otimização (encontrar máximo/mínimo), não inversão.

### 5.3 Falha (Classe C)

Funções contínuas mas monótonas em **nenhum** sub-intervalo (ex: Weierstrass). O algoritmo falha completamente.

**Nota:** Pelo Teorema de Banach-Mazurkiewicz, "genericamente" funções contínuas são Classe C. Porém na prática (funções computáveis, analíticas, definidas por expressão fechada), estamos sempre na Classe A.

### 5.4 Classificação

| Classe | Condição | Exemplos | Algoritmo funciona? |
|---|---|---|---|
| A | f ∈ C¹, f'(x*) ≠ 0 | Gaussiana, polinômios, exp, trig | ✅ Sim |
| B | f'(x*) = 0 (extremo) | Vértice de parábola | ⚠️ Muda para otimização |
| C | Monótona em nenhum intervalo | Weierstrass, fractais | ❌ Não |

## 6. Pseudocódigo

```
BUSCA_BINARIA_CONTINUA(f, T, a, b, ε, max_iter):
    """
    Encontra x* ∈ [a,b] tal que |f(x*) - T| < ε.
    
    Pré-condições:
      - f é contínua em [a,b]
      - T ∈ Im(f|_{[a,b]})
      - f é monótona no intervalo [a,b] (ou será restringida a ramo monótono)
    
    Parâmetros:
      f       : função contínua
      T       : valor alvo
      a, b    : extremos do intervalo de busca
      ε       : precisão desejada
      max_iter: número máximo de iterações
    """
    
    # Determinar direção da monotonicidade
    if f(a) < f(b):
        crescente = True
    else:
        crescente = False
    
    for k in range(max_iter):
        if b - a < ε:
            return (a + b) / 2
        
        # Calcular pivô (mediana ou ponto médio)
        x_m = calcular_pivo(f, a, b)
        
        # Avaliar f no pivô
        y_m = f(x_m)
        
        # Decisão: qual lado contém x*?
        if |y_m - T| < ε:
            return x_m
        
        if crescente:
            if y_m < T:
                a = x_m    # solução está à direita
            else:
                b = x_m    # solução está à esquerda
        else:  # decrescente
            if y_m < T:
                b = x_m    # solução está à esquerda
            else:
                a = x_m    # solução está à direita
    
    return (a + b) / 2


SELECIONAR_RAMO(f, T, a, b):
    """
    Para f não-monótona, encontra sub-intervalo monótono contendo solução.
    
    Estratégia: localizar um extremo (máx/mín) e escolher um lado.
    """
    
    # Encontrar ponto de máximo (para f unimodal)
    x_max = busca_ternaria(f, a, b)
    
    # Verificar qual ramo contém solução
    if f(a) <= T <= f(x_max):
        return (a, x_max, "crescente")
    elif f(b) <= T <= f(x_max):
        return (x_max, b, "decrescente")
    else:
        return None  # T > f(x_max): sem solução


CALCULAR_PIVO(f, a, b, metodo="ponto_medio"):
    """
    Calcula o pivô para a próxima iteração.
    
    Métodos:
      - "ponto_medio": x_m = (a+b)/2 (minimax, sem prior)
      - "mediana": x_m tal que ∫_a^{x_m} w(x)dx = ½·∫_a^b w(x)dx
    """
    
    if metodo == "ponto_medio":
        return (a + b) / 2
    
    elif metodo == "mediana":
        # Requer integração numérica para encontrar x_m
        # tal que a área à esquerda = área à direita
        total = integrar(|f|, a, b)
        
        # Busca binária interna para encontrar mediana
        lo, hi = a, b
        while hi - lo > ε_mediana:
            mid = (lo + hi) / 2
            area_esq = integrar(|f|, a, mid)
            if area_esq < total / 2:
                lo = mid
            else:
                hi = mid
        
        return (lo + hi) / 2
```

## 7. Complexidade

| Método de pivô | Iterações para precisão ε | Custo por iteração | Custo total |
|---|---|---|---|
| Ponto médio | ⌈log₂((b-a)/ε)⌉ | O(1) | O(log((b-a)/ε)) |
| Mediana (integração) | ≤ ⌈log₂((b-a)/ε)⌉ | O(n) para n pontos de quadratura | O(n·log((b-a)/ε)) |

## 8. Conexões com Complexidade Computacional

- Se f é dada por um circuito (implicitamente), computar a mediana (pivô ótimo) é **PP-completo**
- Se f é uma distribuição de contagem (#P), é **#P-hard** 
- A separação entre busca uniforme e busca adaptativa ótima conecta-se com separações de classes (NP vs PP/#P)
- Encontrar o extremo de f (necessário para selecionar ramo) pode ser NP-hard em espaços discretos

## 9. Próximos Passos

1. **Implementação:** Codificar o algoritmo em Python com ambos os métodos de pivô
2. **Bateria de testes:** Gaussiana, polinômios, funções oscilatórias, casos patológicos
3. **Métricas:** Número de iterações, erro por iteração, comparação ponto médio vs mediana
4. **Visualização:** Gráficos de convergência, posição dos pivôs, evolução do intervalo
5. **Análise:** Validar predições teóricas com resultados experimentais

## 10. Referências

- Brent, R. (1973). Algorithms for Minimization without Derivatives.
- Knuth, D. (1971). Optimum binary search trees. Acta Informatica.
- Shannon, C. (1948). A Mathematical Theory of Communication.
- Banach, S. & Mazurkiewicz, S. (1931). Sur les fonctions non-dérivables. Studia Math.
- Weierstrass, K. (1872). Über continuirliche Functionen eines reellen Arguments.
