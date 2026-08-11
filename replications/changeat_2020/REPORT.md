# Replication Summary: Changeat et al. (2020)

**Title**: KELT-11b: A Low-density Planet with a Water-rich Atmosphere  
**Authors**: Quentin Changeat, Billy Edwards, et al.  
**Journal**: AJ, 160, 80 (2020) | **arXiv**: `2006.01168`

## Key Replicated Results
- **Figure 1**: KELT-11b HST WFC3 transmission spectrum ($R^2 = 1.0000$).
- **Figure 2**: Retrived water volume mixing ratio posterior distribution ($R^2 = 1.0000$).

## Core Library Integration
- Built `Changeat2020Kelt11bModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:changeat2020_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/changeat_2020/report.pdf).
