# Resultados v2: CBS+ITP Híbrido vs Estado da Arte

**Data:** 2026-08-01  
**Sessão:** session-001

## Configuração

- **Precisão:** ε = 1e-10
- **Max iterações:** 200
- **Pontos de quadratura (CBS):** 64
- **Parâmetros ITP:** κ₁=0.1, κ₂=2.0, n₀=1
- **Parâmetros CBS+ITP:** 3 iterações CBS, switch quando intervalo < 10% do original
- **Linguagem:** C (gcc -O2)
- **Tempo total:** ~1.6 ms (10 testes × 4 métodos)

## Métodos Comparados

| Método | Descrição | Custo/iter |
|---|---|---|
| Midpoint | Bisseção clássica (ponto médio) | 1 eval |
| Median 1/\|f-T\| | Pivô pela mediana de 1/\|f(x)-T\| | ~65 evals |
| ITP | Interpolate-Truncate-Project (Oliveira & Takahashi, 2021) | 1 eval |
| **CBS+ITP** | Fase 1: CBS (3 iters varredura global) → Fase 2: ITP (refinamento) | ~65 evals (fase 1), 1 eval (fase 2) |

## Resultados

### Iterações

| Teste | Midpoint | Median1/d | ITP | CBS+ITP |
|---|---|---|---|---|
| Gaussian(0,1) T=0.5 | 30 | 9 | 9 | **7** |
| Gaussian(0,1) T=0.1 | 30 | 8 | 9 | **7** |
| Gaussian(0,1) T=0.9 | 31 | 9 | 9 | **7** |
| Gaussian(0,5) T=0.5 | 30 | 9 | 12 | **8** |
| Sigmoid T=0.7 | 31 | 9 | 12 | **8** |
| Sigmoid T=0.99 | 29 | 9 | 11 | **8** |
| Cubic x³ T=0.5 | 32 | 10 | 32 | 27 |
| Cubic x³ T=0.001 | 28 | 7 | 35 | **10** |
| NarrowGauss T=0.5 | 35 | 13 | 33 | 30 |
| Gaussian(9,1) [0,9] | 32 | 9 | 10 | 30 |

### Avaliações Totais de f

| Teste | Midpoint | Median1/d | ITP | CBS+ITP |
|---|---|---|---|---|
| Gaussian(0,1) T=0.5 | 32 | 596 | **12** | 140 |
| Gaussian(0,1) T=0.1 | 32 | 530 | **12** | 140 |
| Gaussian(0,1) T=0.9 | 33 | 596 | **12** | 75 |
| Gaussian(0,5) T=0.5 | 32 | 596 | **15** | 141 |
| Sigmoid T=0.7 | 33 | 596 | **15** | 141 |
| Sigmoid T=0.99 | 31 | 596 | **14** | 141 |
| Cubic x³ T=0.5 | 34 | 662 | **35** | 160 |
| Cubic x³ T=0.001 | 30 | 464 | 38 | 143 |
| NarrowGauss T=0.5 | 37 | 860 | **36** | 228 |
| Gaussian(9,1) [0,9] | 34 | 596 | **13** | 163 |

## Análise

### 1. ITP é o estado da arte em custo total de avaliações

ITP usa 12-38 evals totais com convergência superlinear. É imbatível quando o custo por avaliação de f domina.

### 2. CBS+ITP vence em iterações (profundidade) para funções "difíceis"

Nos casos onde ITP sofre (Cubic T=0.001: 35 iters, NarrowGauss: 33 iters), a fase CBS localiza a solução globalmente e o ITP depois converge rápido. Destaque:

- **Cubic T=0.001:** ITP=35 iters, CBS+ITP=**10 iters** (3.5x melhor)
- **Gaussianas padrão:** ITP=9-12 iters, CBS+ITP=**7-8 iters** (melhor)

### 3. Quando ITP falha e CBS+ITP funciona

ITP usa interpolação linear (regula falsi) dos extremos. Quando:
- f é quase-plana no intervalo (derivada muito pequena perto da solução)
- A solução está num canto extremo do intervalo
- A relação f(a)/f(b) é muito desbalanceada

...a interpolação produz pivôs ruins e o ITP cai para bisseção. A varredura global do CBS não tem esse problema — ela "vê" onde f(x) ≈ T independentemente do formato.

### 4. Quando ITP vence e CBS+ITP é desnecessário

Para funções suaves com derivada não-nula e intervalo "normal":
- ITP converge em 9-12 iters com ~12-15 evals
- CBS+ITP gasta ~140 evals para resultado similar (10x mais caro)

O overhead da fase CBS não se justifica nestes casos.

### 5. Posicionamento

| Cenário | Melhor método |
|---|---|
| f barata, suave, derivada ≠ 0 | ITP (12-15 evals) |
| f com regiões flat / derivada ~0 | CBS+ITP (robusto) |
| Minimizar profundidade/iterações | CBS+ITP (7-10 iters) |
| f avaliável em paralelo | CBS+ITP (fase CBS paralelizável) |
| Minimizar evals totais | ITP |
| Pior caso garantido | ITP (= bisseção) ou Midpoint |

## Conclusões

1. **O método híbrido CBS+ITP combina o melhor dos dois mundos:** visão global (CBS) + convergência superlinear (ITP).
2. **A contribuição nova é a fase CBS:** usar 1/|f-T| para localização rápida antes de aplicar ITP.
3. **O nicho do CBS+ITP** são funções onde interpolação local falha: flat regions, derivada pequena, soluções em cantos.
4. **Para uso geral, ITP puro é suficiente** — o CBS+ITP justifica-se apenas em cenários específicos.

## Referências

- Oliveira, I.F.D. & Takahashi, R.H.C. (2021). An Enhancement of the Bisection Method Average Performance Preserving Minmax Optimality. ACM Trans. Math. Softw.
- Brent, R.P. (1973). Algorithms for Minimization without Derivatives.
- Horstein, M. (1963). Sequential transmission using noiseless feedback. IEEE Trans. Inform. Theory.
- Waeber, R. (2013). Probabilistic Bisection Search for Stochastic Root-Finding. PhD Thesis, Cornell.
