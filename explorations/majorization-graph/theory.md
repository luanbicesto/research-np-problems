# Grafo de Diferenças em Grupos de Soma: Formalização

**Data:** 2026-08-02  
**Status:** Nova linha de pesquisa  
**Conexões:** CBS-nD, funções Schur-convexas, Young lattice

## 1. Setup

### 1.1 Geração dos vetores W

Dado:
- Vetor base Z = (z₁, z₂, ..., z_m) com z_i = n - i + 1 (decrescente)
- Parâmetros: E movimentos "esquerda" (decrement), D movimentos "direita" (mantém)
- Total de posições: m = E + D

Uma **execução** é uma sequência de m símbolos {D, E} com exatamente D ocorrências de 'D' e E de 'E'. Cada execução gera um vetor W de m posições, não-crescente.

**Exemplo concreto:** E=8, D=4, m=12.
- Total de execuções: C(12, 4) = 495
- Z = [8, 7, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0] (base)

### 1.2 Mecânica de geração

Processando a sequência da esquerda para direita:
- Valor inicial: v = D (número de movimentos Direita, = max possível)
- 'D' mantém o valor corrente (w[i] = v)
- 'E' decrementa (v = v - 1, w[i] = v)

**Exemplo:** DDDDEEEEEEEEE → [8, 8, 8, 8, 7, 6, 5, 4, 3, 2, 1, 0] — mas os dados mostram DDDDEEEEEEEE → [8,8,8,8,8,7,6,5,4,3,2,1] = soma 68.

Reinterpretando: w[0] = D (sempre), e cada 'D' subsequente incrementa o valor na posição seguinte relativo ao que 'E' produziria.

**Observação:** Os vetores W resultantes são exatamente as **partições inteiras** com no máximo D partes e maior parte ≤ E, representadas como vetores de m componentes.

### 1.3 Agrupamento por soma

As 495 execuções produzem vetores com somas variando de 36 (mínimo) a 68 (máximo):
- Soma mínima: EEEEEEEEDDDDD → 36
- Soma máxima: DDDDEEEEEEEE → 68
- Distribuição: unimodal, simétrica (palíndroma), máximo 33 execuções na soma central (52)

A distribuição é dada pelos **coeficientes do q-binomial Gaussiano** [m, D]_q = [12, 4]_q.

## 2. Estrutura de Majorização

### 2.1 Definição

Para vetores de mesma soma, a **ordem de majorização** (dominance order) é:

> W_k ≻ W_f ⟺ ∀j: Σᵢ₌₁ʲ w_k[i] ≥ Σᵢ₌₁ʲ w_f[i]

Isso define uma **ordem parcial** (poset) dentro de cada grupo de soma.

### 2.2 Propriedades teóricas

- **Hardy-Littlewood-Pólya (1934):** W_k ≻ W_f ⟺ W_f ∈ conv{σ(W_k) : σ ∈ S_n} (convex hull de permutações)
- **Schur (1923):** φ Schur-convexa ⟹ (W_k ≻ W_f ⟹ φ(W_k) ≥ φ(W_f))
- O poset de majorização para partições é um sub-poset do **Young lattice**

### 2.3 Dados empíricos (grupo soma=52, n=33 vetores)

- Pares totais: C(33, 2) = 528
- Pares comparáveis: ~82% (432/528)
- Largura do poset: 5
- Altura: 15
- Estrutura: quase-total order

## 3. Grafo de Diferenças

### 3.1 Construção

Para cada par (W_k, W_f) no grupo de soma:

1. Calcular diff = W_k - W_f (vetor de diferenças posição a posição)
2. Nota: Σ diff[i] = 0 (mesma soma total)
3. Identificar **fontes** (diff[i] > 0) e **destinos** (diff[i] < 0)
4. Parear fontes com destinos (matching)
5. Cada par (fonte_i, destino_j) → nó (i, j) no grafo

### 3.2 Propriedades do grafo

**Nó (i, j):** representa a transferência unitária Z_i - Z_j entre posição i e posição j.

**Direção importa:** (i, j) ≠ (j, i). Se W_k[i] > W_f[i] e W_k[j] < W_f[j], usamos (i, j). Na comparação inversa, usaríamos (j, i).

**Universo de nós:** Para m=12 posições, |Universo| ≤ m(m-1) = 132 (direcionado) ou ≤ C(m,2) = 66 (não-direcionado se agrupamos (i,j) e (j,i)).

### 3.3 Saturação

Observação empírica (grupo soma=52, lotes 0-5 de 33):

| Lote | Pares processados | Nós acumulados | Reuso |
|---|---|---|---|
| 0 (W1) | 32 | 19 | — |
| 1 (W2) | 31 | 24 | 87% |
| 2 (W3) | 30 | 28 | 90% |
| 3 (W4) | 29 | 29 | 97% |
| 4 (W5) | 28 | 33 | 86% |
| 5 (W6) | 27 | 35 | 93% |

**Explicação teórica:** A saturação é consequência da **finitude do universo**. Como |Universo| ≤ 66 nós e estamos amostrando de 528 pares, pelo **problema do colecionador de cupons** a saturação é inevitável e previsível.

### 3.4 Problema do matching (não-unicidade)

Quando |fontes| > 1 e |destinos| > 1, o pareamento **não é único**. Convenção adotada: ordem crescente de índice. Alternativas possíveis:
- Transporte ótimo (menor custo total Σ|i-j|)
- Matching canônico (lexicográfico)
- Decomposição minimal (menor número de nós)

**Questão em aberto:** A escolha do matching afeta a estrutura global do grafo? Ou o universo de nós converge independentemente?

## 4. Conexão com Funções Convexas

### 4.1 Teorema de Schur

> W_k ≻ W_f ⟺ ∀φ Schur-convexa: φ(W_k) ≥ φ(W_f)

Exemplos de funções Schur-convexas:
- φ(x) = Σ xᵢ² (esfera)
- φ(x) = max_i xᵢ
- φ(x) = Σ xᵢ · log(xᵢ) (entropia negativa)
- φ(x) = Σ f(xᵢ) para qualquer f convexa (separável)

### 4.2 Majorização como interseção de level sets

A ordem W_k ≻ W_f é equivalente a:
- W_f pertence ao **level set** {x : Σᵢ₌₁ʲ x[σ(i)] ≤ Σᵢ₌₁ʲ w_k[σ(i)]} para todo j e σ
- Para vetores já ordenados (nosso caso), são n-1 desigualdades lineares (somas parciais)
- Cada desigualdade é um **semi-espaço** (cutting plane)

### 4.3 CBS-nD como navigator da majorização

O CBS-nD navega level sets de funções convexas. Aplicação:
- **Espaço:** R^m restrito ao simplex ordenado {x : x₁ ≥ x₂ ≥ ... ≥ x_m, Σxᵢ = S}
- **Função:** φ(x) = Σ xᵢ² (ou qualquer Schur-convexa)
- **Target T:** valor de φ(W_k)
- **Busca:** encontrar x com φ(x) = T no simplex → ponto no level set

### 4.4 Tabela de correspondência

| Conceito Majorização | Conceito CBS |
|---|---|
| Vetor W | Ponto em R^m |
| Grupo de soma | Simplex {Σxᵢ = S} |
| W_k ≻ W_f | W_f ∈ level set(s) definidos por W_k |
| Somas parciais ≥ 0 | n-1 cutting planes |
| Função Schur-convexa φ | f: R^m → R (convexa, black-box) |
| Dominance order | Interseção de level sets |

## 5. Questões de Complexidade

### 5.1 O que é polinomial

- Gerar todos os C(m, D) vetores: O(C(m,D) × m)
- Decidir W_k ≻ W_f (dado ambos): **O(m)** (checar n-1 somas parciais)
- Construir grafo completo: O(C(n,2) × m) onde n = tamanho do grupo

### 5.2 O que pode ser mais difícil

- **Encontrar o matching ótimo** para a decomposição: problema de transporte, O(k³) onde k = número de posições com diff ≠ 0
- **Determinar se existe W com propriedade P** no grupo: depende de P
- **Encontrar a cadeia mais longa** no poset: O(n²) via DP no diagrama de Hasse

### 5.3 Onde está o NP?

A majorização em si é polinomial. Mas:
1. Encontrar o **antichain máximo** (largura exata) é equivalente a maximum independent set num DAG → polinomial (Dilworth)
2. Problemas que USAM majorização internamente (scheduling com makespan) podem ser NP-hard
3. A pergunta relevante: para quais **generalizações** da construção D/E o problema deixa de ser tratável?

## 6. Questões em Aberto

1. O multiconjunto de nós do grafo determina univocamente a ordem de majorização para todo (m, D)?
2. Existe fórmula fechada para |Universo(m, D)|?
3. A escolha do matching afeta a capacidade preditiva do grafo?
4. Para quais generalizações (steps > 1, não-inteiro, ...) o certificado compacto deixa de existir?
5. O CBS-Box pode resolver majorização mais rápido que O(m) por par via pré-computação?
6. O "gap" entre soma_nós > 0 e majorização (os ~18% de falsos positivos) tem estrutura computacional significativa?
7. A analogia "convexidade ↔ 2-SAT" pode ser formalizada como redução entre majorização e instâncias satisfatíveis?

## 7. Próximos Passos

1. [ ] Implementar gerador de vetores W para parâmetros (m, D) arbitrários
2. [ ] Construir diagrama de Hasse para grupo soma=52
3. [ ] Automatizar construção do grafo de diferenças (completar os 355 pares restantes)
4. [ ] Verificar se o universo de nós reconstrói toda a ordem
5. [ ] Testar CBS-Box como navigator no simplex ordenado
6. [ ] Parametrizar estudo para diferentes (m, D)
