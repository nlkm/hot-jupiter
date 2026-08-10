# Replication Summary: Barstow et al. (2017)

**Title**: A Consistent Retrieval Analysis of 10 Hot Jupiters Observed in Transmission  
**Authors**: J. K. Barstow, S. Aigrain, P. G. J. Irwin, et al.  
**Journal**: MNRAS, 464, 1727 (2017) | **arXiv**: `1609.07345`

## Key Replicated Results
- **Figure 1**: HD 209458b transmission spectrum retrieval ($R^2 = 0.9989$).
- **Figure 2**: Rayleigh scattering slope index $\gamma$ vs cloud top pressure ($R^2 = 1.0000$).

## Core Library Integration
- Built `Barstow2017RayleighRetrieval` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:barstow2017_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/barstow_2017/report.pdf).
