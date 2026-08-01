# Pesquisa em Problemas NP

Sistema de pesquisa assistido por IA para investigação de problemas NP, classes de complexidade computacional, e a questão P vs NP. Utiliza múltiplos agentes especializados coordenados via [Kiro CLI](https://kiro.dev) para manter um grafo de conhecimento versionado com git.

## Visão Geral

Este projeto implementa um ambiente de pesquisa estruturado onde:

- **Agentes especializados** (matemático e cientista da computação) investigam problemas específicos
- **Um coordenador** gerencia o fluxo de pesquisa, delega tarefas, e mantém o grafo de conhecimento
- **Um grafo de pesquisa** (`research-graph.json`) registra todos os avanços, conexões entre ideias, e caminhos de investigação
- **Git** mantém o histórico completo de toda evolução da pesquisa

## Agentes Disponíveis

### 🧮 Mathematician (`/agent mathematician`)

Especialista em matemática pura e aplicada. Áreas de atuação:

- Teoria dos números e álgebra abstrata
- Análise combinatória e teoria dos grafos
- Topologia e geometria algébrica
- Lógica matemática e teoria dos modelos
- Provas formais e verificação de conjecturas

**Quando usar:** Para demonstrações rigorosas, exploração de estruturas algébricas, análise de propriedades matemáticas de problemas NP, provas de correção de reduções.

```
/agent mathematician
```

### 💻 Computer Scientist (`/agent computer-scientist`)

Especialista em ciência da computação teórica e aplicada. Áreas de atuação:

- Teoria da complexidade computacional (P, NP, coNP, PH, PSPACE...)
- Design e análise de algoritmos
- Criptografia e segurança computacional
- Reduções entre problemas e completude
- Implementação de experimentos computacionais

**Quando usar:** Para análise de complexidade, design de algoritmos aproximativos, implementação de experimentos, reduções formais entre problemas, análise de classes de complexidade.

```
/agent computer-scientist
```

### 🎯 Research Coordinator (`/agent research-coordinator`)

Coordenador central da pesquisa. Responsabilidades:

- Manutenção do grafo de pesquisa (`research-graph.json`)
- Delegação de tarefas para os agentes especializados
- Gerenciamento de sessões de pesquisa
- Commits no git com mensagens padronizadas
- Visão holística do progresso e próximos passos

**Quando usar:** Para iniciar novas linhas de investigação, revisar o estado geral da pesquisa, criar conexões entre descobertas, planejar próximos passos, ou quando não souber qual agente usar.

```
/agent research-coordinator
```

## Estrutura do Grafo de Pesquisa

O arquivo `research-graph.json` é o coração do sistema. Ele contém:

### Nodes (Nós)

Cada nó representa uma unidade de pesquisa:

```json
{
  "id": "identificador-unico",
  "type": "origin | exploration | theorem | conjecture | experiment | reduction | result",
  "title": "Título descritivo",
  "description": "Descrição detalhada",
  "directory": "caminho/para/arquivos/relacionados",
  "created": "YYYY-MM-DD",
  "status": "active | completed | abandoned | blocked",
  "results": ["Lista de resultados obtidos"],
  "scripts": ["scripts/relacionados.py"],
  "references": ["Referências bibliográficas"],
  "tags": ["tags", "relevantes"]
}
```

### Edges (Arestas)

Conexões entre nós que representam relações:

```json
{
  "from": "id-origem",
  "to": "id-destino",
  "relation": "leads_to | requires | contradicts | supports | reduces_to | generalizes",
  "description": "Descrição da relação"
}
```

### Paths (Caminhos)

Sequências de investigação que formam linhas de pesquisa:

```json
{
  "id": "path-id",
  "name": "Nome do caminho de pesquisa",
  "nodes": ["node-1", "node-2", "node-3"],
  "status": "active | completed | abandoned"
}
```

## Histórico via Git

Todo o progresso da pesquisa é versionado com git. O coordenador segue convenções de commit:

| Prefixo | Uso |
|---------|-----|
| `research:` | Avanços na pesquisa, novos resultados |
| `graph:` | Atualizações no grafo de conhecimento |
| `experiment:` | Adição/modificação de experimentos |
| `session:` | Início/fim de sessões de pesquisa |
| `docs:` | Atualizações em documentação |
| `fix:` | Correções em dados ou scripts |

### Exemplo de workflow git:

```bash
# O coordenador faz commits como:
git commit -m "research: prove NP-hardness of vertex cover variant"
git commit -m "graph: add node for 3-SAT reduction exploration"
git commit -m "experiment: add benchmark for approximation algorithm"
```

Para ver o histórico da pesquisa:

```bash
git log --oneline --graph
```

## Estrutura de Diretórios

```
research-np-problems/
├── .kiro/
│   ├── agents/
│   │   ├── mathematician.json         # Config do agente matemático
│   │   ├── computer-scientist.json    # Config do agente de CS
│   │   └── research-coordinator.json  # Config do coordenador
│   └── prompts/
│       ├── mathematician.md           # Prompt do matemático
│       ├── computer-scientist.md      # Prompt do cientista da computação
│       └── research-coordinator.md    # Prompt do coordenador
├── sessions/                          # Sessões de pesquisa (logs, notas)
├── research-graph.json                # Grafo de conhecimento (arquivo central)
├── RESEARCH_STRUCTURE.md              # Documentação técnica da estrutura
├── README.md                          # Este arquivo
└── .gitignore                         # Arquivos ignorados pelo git
```

À medida que a pesquisa avança, novas pastas serão criadas para:
- Experimentos computacionais
- Provas e demonstrações
- Scripts e implementações
- Notas e referências

## Como Começar

1. **Abra o projeto no Kiro CLI:**
   ```bash
   cd research-np-problems/
   kiro
   ```

2. **Inicie com o coordenador:**
   ```
   /agent research-coordinator
   ```
   O coordenador vai ler o estado atual do grafo e sugerir próximos passos.

3. **Delegue para especialistas conforme necessário:**
   ```
   /agent mathematician
   ```
   ou
   ```
   /agent computer-scientist
   ```

4. **Volte ao coordenador para consolidar:**
   ```
   /agent research-coordinator
   ```
   Peça para atualizar o grafo com as descobertas.

## Notas

- Os agentes respondem no idioma do usuário (português)
- O grafo é a fonte de verdade sobre o estado da pesquisa
- Sempre use o coordenador para manter consistência no grafo
- Commits frequentes preservam todo o histórico de raciocínio
