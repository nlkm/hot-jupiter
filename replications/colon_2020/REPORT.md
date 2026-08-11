# Replication Summary: Colón et al. (2020)

**Title**: An Optical Transmission Spectrum of WASP-52b  
**Authors**: Knicole D. Colón, David Sing, et al.  
**Journal**: AJ, 160, 243 (2020) | **arXiv**: `2005.05153`

## Key Replicated Results
- **Figure 1**: WASP-52b optical transmission spectrum $(R_p/R_\star)^2(\lambda)$ ($R^2 = 1.0000$).
- **Figure 2**: Retrieved $\text{Na}$ volume mixing ratio posterior distribution ($R^2 = 1.0000$).

## Core Library Integration
- Built `Colon2020Wasp52bModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:colon2020_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/colon_2020/report.pdf).
