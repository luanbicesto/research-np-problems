# Sessão 001 — Busca Binária Contínua

**Data:** 2026-08-01  
**Duração:** ~30 min  
**Participantes:** Luan + agentes (mathematician, computer-scientist)

## Resumo

Sessão inicial explorando a ideia de generalizar busca binária discreta para o domínio contínuo. Partiu de uma pergunta sobre o significado da mediana para uma função f(x) num intervalo.

## Progresso

### Conceito de mediana para f(x)
- Múltiplas interpretações identificadas
- A mais natural: ponto que divide a área sob f em duas partes iguais
- Para Gaussiana simétrica: mediana = μ (centro)

### Busca binária contínua
- Analogia formal discreta → contínuo estabelecida
- Pivô geométrico (ponto médio) = bisseção clássica
- Pivô pela mediana = ótimo informacionalmente (1 bit/query)
- Trade-off: custo de computar mediana vs ganho em convergência

### Monotonicidade e seleção de ramo
- Busca binária requer monotonicidade para decisão determinística
- Solução: restringir domínio a ramo monótono ("dar uma direção")
- Impossibilidade de transformação global preservando raízes
- Classificação: Classe A (funciona), B (otimização), C (falha)

## Decisões

1. Documentar teoria completa → `theory.md` ✅
2. Próximo passo: implementação em Python + benchmarks
3. Plano de experimentos definido → `experiments/PLAN.md` ✅

## Questões em aberto

- Como a dificuldade de computar o pivô ótimo (mediana de distribuição implícita) se conecta com classes de complexidade?
- Em espaços discretos/combinatórios, a "seleção de ramo" corresponde a quê?
- Existe analogia com particionamento de espaço de soluções em problemas NP?
