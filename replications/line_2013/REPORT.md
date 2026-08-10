# Replication Summary: Line et al. (2013)

**Title**: A Systematic Retrieval Analysis of Secondary Eclipse Spectra I: Terrestrial & Gas Giant Planets  
**Authors**: Michael R. Line, P. Kopparapu, Y. L. Yung, et al.  
**Journal**: ApJ, 775, 137 (2013) | **arXiv**: `1304.5561`

## Key Replicated Results
- **Figure 1**: Atmospheric chemical mixing ratio posteriors ($\log_{10} X_i$) for $H_2O, CO, CO_2, CH_4$. ($R^2 = 1.0000$).
- **Figure 2**: Secondary eclipse emission spectrum planet-to-star flux ratio ($F_{\text{planet}} / F_{\star}$) across Spitzer IRAC passbands ($R^2 = 1.0000$).

## Core Library Integration
- Built `LineRetrievalMultiGas` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:line2013_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/line_2013/report.pdf).
