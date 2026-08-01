# Resultados: Busca Binária Contínua — Benchmark v1

**Data:** 2026-08-01  
**Sessão:** session-001

## Configuração

- **Precisão:** ε = 1e-10
- **Max iterações:** 200
- **Pontos de quadratura (mediana):** 64
- **Linguagem:** C (gcc -O2)
- **Tempo total de execução:** ~2 ms

## Métodos Comparados

| Método | Pivô | Custo por iteração |
|---|---|---|
| Midpoint | x_p = (a+b)/2 | 1 eval de f |
| Median\|f\| | Mediana de \|f(x)\| no intervalo | ~65 evals de f |
| Median 1/\|f-T\| | Mediana de 1/\|f(x)-T\| no intervalo | ~65 evals de f |

## Resultados

### Tabela de Iterações

| Teste | Midpoint | Median\|f\| | Median 1/\|f-T\| |
|---|---|---|---|
| Gaussian(0,1) T=0.5 | 30 | 33 | **9** |
| Gaussian(0,1) T=0.1 | 30 | 33 | **8** |
| Gaussian(0,1) T=0.9 | 31 | 32 | **9** |
| Gaussian(0,5) T=0.5 | 30 | 33 | **9** |
| Sigmoid T=0.7 | 31 | 32 | **9** |
| Sigmoid T=0.99 | 29 | 28 | **9** |
| Cubic x³ T=0.5 | 32 | 32 | **10** |
| Cubic x³ T=0.001 | 28 | 39 | **7** |
| NarrowGauss(0.7,0.01) T=0.5 | 35 | 31 | **13** |
| Gaussian(9,1) T=0.5 [0,9] | 32 | 32 | **9** |

### Tabela de Avaliações Totais de f

| Teste | Midpoint | Median\|f\| | Median 1/\|f-T\| |
|---|---|---|---|
| Gaussian(0,1) T=0.5 | 32 | 2180 | 596 |
| Gaussian(0,1) T=0.1 | 32 | 2180 | 530 |
| Gaussian(0,1) T=0.9 | 33 | 2114 | 596 |
| Gaussian(0,5) T=0.5 | 32 | 2180 | 596 |
| Sigmoid T=0.7 | 33 | 2114 | 596 |
| Sigmoid T=0.99 | 31 | 1850 | 596 |
| Cubic x³ T=0.5 | 34 | 2114 | 662 |
| Cubic x³ T=0.001 | 30 | 2576 | 464 |
| NarrowGauss(0.7,0.01) T=0.5 | 37 | 2048 | 860 |
| Gaussian(9,1) T=0.5 [0,9] | 34 | 2114 | 596 |

## Análise

### 1. Median 1/|f-T| converge em ~3x menos iterações

O peso 1/|f(x)-T| concentra massa onde f(x) ≈ T (perto da solução x*). A mediana desse peso é um excelente estimador da posição de x*, funcionando como uma **interpolação implícita** sem derivada.

Redução média: de ~31 iterações (midpoint) para ~9 iterações (median 1/|f-T|).

### 2. Median|f| não melhora (e às vezes piora)

O peso |f(x)| concentra massa onde f é grande — geralmente **longe** da solução (para inversão f(x)=T com T < max(f)). Resultado: não traz vantagem, confirma a predição teórica.

### 3. Trade-off iterações vs avaliações

| Método | Iterações (média) | Evals totais (média) | Evals/iter |
|---|---|---|---|
| Midpoint | 30.8 | 33.0 | 1.07 |
| Median\|f\| | 32.5 | 2153 | 66.2 |
| Median 1/\|f-T\| | 9.2 | 609 | 66.2 |

O midpoint vence em custo bruto de avaliações (~33 vs ~609). Porém a median 1/|f-T| vence em:
- **Número de decisões** (iterações do loop externo)
- **Profundidade da árvore de busca**
- Cenários onde o custo de "decidir" (não de avaliar f) domina

### 4. Quando Median 1/|f-T| é preferível

- Quando cada **iteração** tem custo fixo alto além da avaliação de f
- Quando f pode ser avaliada em batch (paralelo) dentro da quadratura
- Quando queremos minimizar a **profundidade** (ex: pipelines sequenciais)
- Quando f é barata mas a decisão/comunicação é cara

### 5. Quando Midpoint é preferível

- Quando o custo é dominado por avaliações de f
- Quando f é cara de avaliar
- Quando não temos acesso a f em pontos arbitrários (oráculo restrito)
- Quando queremos garantia minimax de pior caso

## Conclusões

1. **A escolha do peso é crítica:** 1/|f-T| é um prior informativo correto; |f| é um prior incorreto para inversão.
2. **A teoria se confirma:** pivô pela mediana de um bom prior reduz iterações proporcionalmente à informação ganha por query.
3. **Trade-off fundamental:** menos iterações ↔ mais avaliações por iteração. A métrica relevante depende do contexto.

## Próximos Passos

- [ ] Reduzir pontos de quadratura (32, 16, 8) e medir impacto na convergência
- [ ] Implementar peso adaptativo que melhora a cada iteração
- [ ] Gerar gráficos de convergência (erro vs iteração)
- [ ] Testar com funções de custo maior por avaliação
- [ ] Explorar conexão com complexidade: o que acontece quando f é um oráculo NP?
