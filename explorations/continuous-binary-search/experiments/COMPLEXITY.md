# Análise de Complexidade: CBS Complete

**Data:** 2026-08-01  
**Algoritmo:** Busca Binária Contínua Completa (Seleção de Ramo + CBS + ITP)

## Modelo de Custo

- Cada avaliação de f(x) custa O(C_f)
- Operações aritméticas custam O(1)
- Memória: O(n_q) para quadratura (64 doubles ≈ 512 bytes)

## Parâmetros

| Parâmetro | Valor default | Descrição |
|---|---|---|
| W = b - a | (input) | Largura do intervalo de busca |
| ε | (input) | Precisão desejada |
| n_s | 32 | Pontos de amostragem (seleção de ramo) |
| n_q | 64 | Pontos de quadratura (fase CBS) |
| k_cbs | 3 | Iterações máximas da fase CBS |
| n_0 | 1 | Slack do ITP (pior caso) |
| C_f | O(1) | Custo de avaliar f |

## Custo por Fase

### Fase 1: Seleção de Ramo (custo fixo)

| Caso | Condição | Avaliações |
|---|---|---|
| A — Bracketing direto + monótona | (f(a)-T)·(f(b)-T) < 0 e f monótona | 10 |
| B — Bracketing + não-monótona | Bracketing mas sem monotonicidade | 43 |
| C — Sem bracketing, unimodal | Precisa busca ternária | 105 |
| D — Fallback (amostragem densa) | Nenhum caso anterior funcionou | 234 |

### Fase 2: CBS — Localização Global (custo fixo)

- Executada se bracket > 1.0
- k_cbs iterações × (n_q + 1) avaliações/iter
- Custo: k_cbs × (n_q + 1) = 3 × 65 = **195 avaliações**

### Fase 3: ITP — Convergência Superlinear (custo logarítmico)

- W' = largura do bracket após fases 1 e 2
- Número de iterações: ⌈log₂(W'/(2ε))⌉ + n_0
- Custo por iteração: 1 avaliação
- Custo total: **⌈log₂(W'/(2ε))⌉ + 1 avaliações**

Nota: Na prática o ITP converge em menos iterações (convergência superlinear), mas a garantia de pior caso é a bisseção.

## Complexidade Total

### Expressão Exata

```
T(W, ε) = [K_branch + K_cbs + ⌈log₂(W'/(2ε))⌉ + n_0] × C_f
```

Onde:
- K_branch ∈ {10, 43, 105, 234} (seleção de ramo)
- K_cbs = k_cbs × (n_q + 1) = 195 (ou 0 se bracket estreito)
- W' ≤ W (largura residual após fases 1-2)

### Pior Caso

```
T_worst = [234 + 195 + ⌈log₂(W/(2ε))⌉ + 1] × C_f
        = [430 + ⌈log₂(W/(2ε))⌉] × C_f
```

### Notação Assintótica

**T(W, ε) = O(log(W/ε) × C_f)**

Justificativa: K_branch e K_cbs são constantes (independem de W e ε). O termo logarítmico domina assintoticamente quando ε → 0.

## Comparação

| Método | Avaliações (W=1000, ε=10⁻¹⁰) | Assintótica | Pré-condições |
|---|---|---|---|
| Bisseção | 43 | O(log(W/ε)) | Bracketing + monótona |
| ITP | 44 | O(log(W/ε)) | Bracketing |
| **CBS Complete** | **250 — 480** | **O(log(W/ε))** | **Apenas continuidade** |

## Análise do Overhead

O CBS Complete paga um overhead fixo K ≈ 200-430 avaliações para:
1. Encontrar um ramo monótono sem informação prévia
2. Localizar globalmente a região da solução via varredura

### Quando o overhead domina vs. quando é irrelevante

Para ε = 10⁻¹⁰ e W = 1000:
- Termo logarítmico: log₂(1000/(2×10⁻¹⁰)) ≈ 42
- Overhead fixo: K ≈ 200-430
- **Ratio: K/log ≈ 5-10×** → overhead domina

Para ε = 10⁻¹⁰⁰ e W = 1000:
- Termo logarítmico: log₂(1000/(2×10⁻¹⁰⁰)) ≈ 335
- Overhead fixo: K ≈ 200-430
- **Ratio: K/log ≈ 0.6-1.3×** → overhead comparável

Crossover (overhead = log): W/ε ≈ 2^K ≈ 2⁴³⁰ (irrealista em aritmética de precisão finita)

**Conclusão prática:** Para precisão de máquina (double), o overhead fixo sempre domina. O CBS Complete é 5-13× mais caro que ITP puro em avaliações de f.

## Justificativa do Overhead

O custo extra compra **generalidade**:

| Propriedade | ITP/Brent | CBS Complete |
|---|---|---|
| Requer bracketing prévio | Sim | **Não** |
| Requer monotonicidade | Implícito (via bracketing) | **Não** |
| Funciona com f não-monótona | Não diretamente | **Sim** |
| Seleção automática de ramo | Não | **Sim** |
| Múltiplas soluções | Requer saber qual ramo | Encontra uma automaticamente |

## Complexidade Espacial

- O(n_q) = O(64) para arrays de quadratura → **O(1)** (constante)
- O(n_s) para arrays de amostragem → **O(1)**
- Sem recursão → stack fixo

## Resumo

```
┌─────────────────────────────────────────────────────────┐
│ CBS Complete: T(W, ε) = O(log(W/ε))                    │
│                                                         │
│ Custo prático: ~250-480 avaliações de f                 │
│ (vs ~44 do ITP puro, vs ~43 da bisseção)               │
│                                                         │
│ Trade-off: 5-13× mais avaliações                        │
│            em troca de não precisar bracketing/monotonia │
└─────────────────────────────────────────────────────────┘
```
