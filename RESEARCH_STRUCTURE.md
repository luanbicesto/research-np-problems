# Research Structure — NP Problems Research Project

## Overview

This project is a systematic research effort investigating NP problems, computational complexity, and related mathematical structures. It uses a multi-agent system with specialist researchers coordinated by a central agent.

## Project Layout

```
research-np-problems/
├── .kiro/
│   ├── agents/
│   │   ├── mathematician.json          # Senior math researcher agent
│   │   ├── computer-scientist.json     # Senior CS researcher agent
│   │   └── research-coordinator.json   # Orchestration agent
│   └── prompts/
│       ├── mathematician.md            # Detailed math agent prompt
│       ├── computer-scientist.md       # Detailed CS agent prompt
│       └── research-coordinator.md     # Coordinator prompt
├── research-graph.json                 # Knowledge graph tracking all research
├── RESEARCH_STRUCTURE.md               # This file
├── sessions/                           # Research session directories
│   └── YYYY-MM-DD-slug/
│       ├── notes.md
│       ├── results.md
│       └── scripts/
└── results/                            # Final, polished results
    ├── theorems/
    ├── algorithms/
    └── surveys/
```

## Agent Architecture

### Research Coordinator (Primary Agent)
- **Role**: Orchestrates the entire research project
- **Invocation**: Default agent for the project
- **Capabilities**:
  - Manages the research graph
  - Delegates to specialist agents
  - Creates and manages research sessions
  - Commits progress to git
  - Educates the user on mathematical concepts
  - Formats results in LaTeX/markdown

### Mathematician (Specialist Agent)
- **Role**: Senior mathematical researcher (post-doctoral level)
- **Invocation**: Called by coordinator when mathematical rigor is needed
- **Capabilities**:
  - Proves/disproves theorems with full rigor
  - Surveys mathematical literature (arXiv, conferences)
  - Formulates conjectures with evidence
  - Finds counterexamples and constructions
  - Covers all areas: algebra, analysis, topology, number theory, combinatorics, logic, complexity

### Computer Scientist (Specialist Agent)
- **Role**: Senior theoretical CS researcher with implementation skills
- **Invocation**: Called by coordinator for computational tasks
- **Capabilities**:
  - Proves NP-completeness via reductions
  - Implements algorithms and experiments
  - Designs SAT encodings
  - Analyzes approximability and parameterized complexity
  - Runs computational searches and verification

## Research Graph

The `research-graph.json` file is the central data structure tracking all research progress. It is a directed graph where:

### Nodes represent research artifacts:
| Type | Description |
|------|-------------|
| `origin` | Starting point of the research |
| `exploration` | An active investigation into a question |
| `result` | A confirmed result (proven, verified) |
| `conjecture` | An unproven but plausible claim |
| `theorem` | A fully proven mathematical statement |
| `dead_end` | An approach that was abandoned |
| `branch` | A point where research splits |

### Node Fields:
- `id` — Unique identifier (slug format)
- `type` — One of the types above
- `title` — Short descriptive title
- `description` — What is being investigated or what was found
- `directory` — Relative path to the session directory
- `created` — Date of creation (YYYY-MM-DD)
- `status` — `active` | `completed` | `abandoned` | `blocked`
- `results` — List of key findings
- `scripts` — List of scripts/code generated
- `references` — Papers and sources referenced
- `tags` — Tags for search and categorization

### Edges represent relationships:
| Relation | Meaning |
|----------|---------|
| `evolves_to` | One exploration naturally led to another |
| `branches_from` | A new direction spawned from existing work |
| `contradicts` | Findings that conflict with another node |
| `supports` | Evidence supporting another node's claims |
| `refines` | A more precise version of a previous result |
| `generalizes` | Extends a result to a broader setting |

### Paths
Named sequences of nodes representing complete research threads (e.g., "From graph coloring to PCP").

## Workflow

### Starting a New Research Session

1. The coordinator reads the current state of `research-graph.json`
2. A new exploration node is created in the graph
3. A session directory is created: `sessions/YYYY-MM-DD-slug/`
4. The exploration begins, delegating to specialists as needed
5. Results are documented in the session directory
6. The graph is updated with findings
7. Progress is committed to git

### Git Conventions

Commit messages follow the format:
```
research: <action> — <description>
```

Actions: `start exploration`, `complete`, `dead end`, `branch`, `result`, `experiment`, `update graph`, `notes`

### Recording Results

When a significant result is obtained:
1. The coordinator creates a formal write-up in `results/`
2. A `theorem` or `result` node is added to the graph
3. Edges connect it to the explorations that produced it
4. The result is added to relevant research paths

## Getting Started

To begin a research session:

1. Open the project with the research-coordinator agent
2. The coordinator will display the current state of research
3. Tell the coordinator what you want to investigate
4. The coordinator will create a session and begin work

## Key References

- Cook, S. (1971). "The complexity of theorem-proving procedures." STOC.
- Karp, R. (1972). "Reducibility among combinatorial problems."
- Arora, S. & Barak, B. (2009). *Computational Complexity: A Modern Approach.*
- Sipser, M. (2012). *Introduction to the Theory of Computation.*
- Garey, M. & Johnson, D. (1979). *Computers and Intractability.*
- Papadimitriou, C. (1994). *Computational Complexity.*
- Wigderson, A. (2019). *Mathematics and Computation.*
- Jukna, S. (2012). *Boolean Function Complexity.*
