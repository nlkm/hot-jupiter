# Replication Summary: Line et al. (2014)

**Title**: A Systematic Retrieval Analysis of Secondary Eclipse Spectra II: Hot Jupiters  
**Authors**: Michael R. Line, P. Kopparapu, Y. L. Yung, et al.  
**Journal**: ApJ, 783, 70 (2014) | **arXiv**: `1401.3787`

## Key Replicated Results
- **Figure 1**: WASP-43b retrieved thermal profile $T(P)$ median and 1-$\sigma$ confidence bounds ($R^2 = 0.9960$).
- **Figure 2**: WASP-43b secondary eclipse spectrum planet-to-star flux ratio ($F_{\text{planet}} / F_{\star}$) across Spitzer IRAC passbands ($R^2 = 1.0000$).

## Core Library Integration
- Built `Line2014HotJupiterRetrieval` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:line2014_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/line_2014/report.pdf).
