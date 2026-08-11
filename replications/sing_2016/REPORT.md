# Replication Summary: Sing et al. (2016)

**Title**: A Continuum from Clear to Cloudy Hot-Jupiter Atmospheres without Water Depletion  
**Authors**: David K. Sing, Jonathan J. Fortney, et al.  
**Journal**: Nature, 529, 59 (2016) | **arXiv**: `1512.04341`

## Key Replicated Results
- **Figure 1**: WASP-12b cloud-muffled transmission spectrum $(R_p/R_\star)^2(\lambda)$ ($R^2 = 1.0000$).
- **Figure 2**: Water feature amplitude attenuation trend $\Delta (R_p/R_\star)^2_{1.4\mu\text{m}}(T_{\text{eq}})$ ($R^2 = 1.0000$).

## Core Library Integration
- Built `Sing2016CloudContinuumModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:sing2016_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/sing_2016/report.pdf).
