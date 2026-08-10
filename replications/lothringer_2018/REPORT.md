# Replication Summary: Lothringer et al. (2018)

**Title**: Extremely Irradiated Hot Jupiters: The Transition to Ultra-Hot Jupiters  
**Authors**: Joshua D. Lothringer, Travis S. Barman, Thomas P. Greene  
**Journal**: ApJ, 866, 27 (2018) | **arXiv**: `1808.00538`

## Key Replicated Results
- **Figure 1**: Ultra-hot Jupiter thermal inversion profile $T(P)$ ($R^2 = 0.9944$).
- **Figure 2**: Emergent emission spectrum $F_\lambda(\lambda)$ ($R^2 = 0.9998$).

## Core Library Integration
- Built `Lothringer2018UltraHotJupiter` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:lothringer2018_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/lothringer_2018/report.pdf).
