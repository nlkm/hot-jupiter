# Replication Summary: Baxter et al. (2020)

**Title**: Evidence for H- Opacity or Thermal Inversions in Nine Ultra-Hot Jupiters  
**Authors**: E. K. H. Baxter, V. Parmentier, et al.  
**Journal**: A&A, 639, A36 (2020) | **arXiv**: `2004.14389`

## Key Replicated Results
- **Figure 1**: Spitzer dayside brightness temperatures $T_{\text{bright}}$ vs $T_{\text{eq}}$ ($R^2 = 1.0000$).
- **Figure 2**: Brightness temperature difference $\Delta T_{\text{bright}}$ vs $T_{\text{eq}}$ ($R^2 = 1.0000$).

## Core Library Integration
- Built `Baxter2020UltraHotPopulationModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:baxter2020_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/baxter_2020/report.pdf).
