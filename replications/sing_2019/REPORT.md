# Replication Summary: Sing et al. (2019)

**Title**: The HST PanCET Program: Exospheric Na I and K I Absorption in WASP-121b  
**Authors**: David K. Sing, Thomas Mikal-Evans, et al.  
**Journal**: AJ, 158, 91 (2019) | **arXiv**: `1905.07684`

## Key Replicated Results
- **Figure 1**: WASP-121b HST PanCET optical transmission spectrum $(R_p/R_\star)^2(\lambda)$ ($R^2 = 1.0000$).
- **Figure 2**: Exospheric sodium line profile excess $\Delta (R_p/R_\star)^2(v)$ ($R^2 = 1.0000$).

## Core Library Integration
- Built `Sing2019Wasp121bModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:sing2019_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/sing_2019/report.pdf).
