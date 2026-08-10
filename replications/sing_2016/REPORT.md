# Replication Summary: Sing et al. (2016)

**Title**: A continuum from clear to cloudy hot-Jupiter atmospheres  
**Authors**: David K. Sing, Jonathan J. Fortney, Nikolay Nikolov, et al.  
**Journal**: Nature, 529, 59 (2016) | **arXiv**: `1512.04341`

## Key Replicated Results
- **Figure 1**: WASP-39b clear transmission spectrum ($R^2 = 1.0000$).
- **Figure 2**: 10-planet clear-to-cloudy water absorption feature continuum ($R^2 = 1.0000$).

## Core Library Integration
- Built `Sing2016TransmissionContinuum` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:sing2016_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/sing_2016/report.pdf).
