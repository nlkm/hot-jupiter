# Replication Summary: Wakeford et al. (2017)

**Title**: HAT-P-26b: A Neptune-mass exoplanet with a primordial atmosphere  
**Authors**: Hannah R. Wakeford, David K. Sing, Thomas P. Evans, et al.  
**Journal**: Science, 356, 1150 (2017) | **arXiv**: `1706.04168`

## Key Replicated Results
- **Figure 1**: HAT-P-26b transmission spectrum ($R^2 = 1.0000$).
- **Figure 2**: Giant exoplanet mass-metallicity scaling relation ($R^2 = 1.0000$).

## Core Library Integration
- Built `Wakeford2017PrimordialAtmosphere` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:wakeford2017_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/wakeford_2017/report.pdf).
