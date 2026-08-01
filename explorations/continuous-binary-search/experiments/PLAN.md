# Plano de Experimentos: Busca Binária Contínua

**Data:** 2026-08-01  
**Status:** Planejado

## Objetivo

Validar experimentalmente as predições teóricas sobre a busca binária contínua com pivô pela mediana vs ponto médio, em diferentes classes de funções.

## Implementação

### Módulos a desenvolver

1. **`cbs_algorithm.py`** — Algoritmo principal
   - `bisection_midpoint(f, T, a, b, eps)` — pivô por ponto médio
   - `bisection_median(f, T, a, b, eps, weight_fn)` — pivô pela mediana
   - `select_branch(f, T, a, b)` — seleção automática de ramo monótono
   - Registro de histórico: pivôs escolhidos, intervalos, erros por iteração

2. **`test_functions.py`** — Bateria de funções de teste
   - Gaussiana (unimodal, simétrica)
   - Polinômios (múltiplas raízes)
   - Funções trigonométricas (periódicas, múltiplos ramos)
   - Funções com concentração de massa assimétrica
   - Caso patológico: x·sin(1/x)

3. **`benchmarks.py`** — Coleta de métricas
   - Número de iterações até convergência
   - Erro |f(x_k) - T| por iteração
   - Largura do intervalo por iteração
   - Número de avaliações de f (custo total)

4. **`visualization.py`** — Geração de gráficos
   - Convergência comparada (midpoint vs median)
   - Posição dos pivôs sobre a curva f
   - Evolução do intervalo [a_k, b_k]
   - Heatmap de eficiência por tipo de função

## Bateria de Testes

### Teste 1: Gaussiana (caso ideal)
- f(x) = exp(-(x-μ)²/(2σ²))
- Variações: σ ∈ {0.5, 1, 2, 5}, T ∈ {0.1, 0.3, 0.5, 0.7, 0.9}
- Intervalo: [-5σ+μ, μ] (ramo crescente)
- Expectativa: ambos os métodos convergem em ~20 iterações para ε=10⁻⁶

### Teste 2: Distribuição assimétrica (onde mediana brilha)
- f(x) = x³·exp(-x) em [0, 20], T variável
- Massa concentrada à esquerda → mediana ≠ ponto médio
- Expectativa: mediana converge mais rápido em expectativa

### Teste 3: Polinômio com múltiplos cruzamentos
- f(x) = x³ - 6x² + 11x - 6 (raízes em 1, 2, 3)
- Testar seleção automática de ramo
- Expectativa: algoritmo encontra uma raiz corretamente

### Teste 4: Função altamente oscilatória
- f(x) = sin(20x) em [0, π]
- Múltiplos ramos monótonos, testa robustez da seleção de ramo
- Expectativa: seleção de ramo funciona, busca converge em cada ramo

### Teste 5: Função com concentração extrema
- f(x) = exp(-100·(x-0.7)²) em [0, 1] (Gaussiana muito estreita)
- Prior: peso ∝ |f(x)| concentra massa em x≈0.7
- Expectativa: mediana com peso |f| foca na região correta, converge muito mais rápido que ponto médio

### Teste 6: Caso patológico
- f(x) = x·sin(1/x) perto de x=0
- Infinitas oscilações, testa limites do algoritmo
- Expectativa: falha ou convergência muito lenta perto de 0

## Métricas de Comparação

| Métrica | Descrição |
|---|---|
| `n_iters` | Número de iterações até |f(x)-T| < ε |
| `n_evals` | Número total de avaliações de f (inclui custo da mediana) |
| `error_curve` | Sequência de |f(x_k)-T| por iteração |
| `interval_curve` | Sequência de (b_k - a_k) por iteração |
| `pivot_positions` | Lista de x_m escolhidos |
| `bits_per_query` | Informação efetiva ganha por query |

## Gráficos a Gerar

1. **Convergência comparada:** log(erro) vs iteração, midpoint vs median, para cada função
2. **Posição dos pivôs:** f(x) plotada com marcadores nos pivôs de cada método
3. **Intervalo vs iteração:** largura do intervalo ao longo do tempo
4. **Barra de eficiência:** n_iters por função × método (gráfico de barras)
5. **Caso assimétrico detalhado:** mostrar como a mediana "pula" para a região de interesse

## Critérios de Sucesso

- [ ] Mediana ≤ ponto médio em iterações para todos os casos com prior informativo
- [ ] Ponto médio = mediana para prior uniforme (confirmar equivalência teórica)
- [ ] Seleção de ramo funciona corretamente para funções unimodais
- [ ] Gráficos claros que ilustrem a vantagem informacional da mediana
- [ ] Caso patológico documentado mostrando os limites do método
