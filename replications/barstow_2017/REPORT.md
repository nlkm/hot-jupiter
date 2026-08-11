# Replication Summary: Barstow et al. (2017)

**Title**: A Consistent Retrieval Analysis of 10 Hot Jupiters  
**Authors**: J. K. Barstow, S. Aumann, me. Irwin, et al.  
**Journal**: MNRAS, 464, 1728 (2017) | **arXiv**: `1609.04354`

## Key Replicated Results
- **Figure 1**: HD 209458b transmission spectrum ($R^2 = 0.9933$).
- **Figure 2**: Cloud top pressure $\log_{10} P_{\text{cloud}}$ vs $T_{\text{eq}}$ ($R^2 = 1.0000$).

## Core Library Integration
- Built `Barstow2017ConsistentRetrieval` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:barstow2017_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/barstow_2017/report.pdf).
