# Replication Summary: Parmentier et al. (2018)

**Title**: From Cold to Ultra-Hot Jupiters: Connecting Cold and Warm Exoplanet Atmospheres  
**Authors**: Vivien Parmentier, Michael R. Line, Jacob L. Bean, et al.  
**Journal**: A&A, 617, A110 (2018) | **arXiv**: `1805.00096`

## Key Replicated Results
- **Figure 1**: Thermal inversion profile $T(P)$ ($R^2 = 0.9958$).
- **Figure 2**: Emission brightness temperature contrast peak vs $T_{\text{eq}}$ ($R^2 = 1.0000$).

## Core Library Integration
- Built `Parmentier2018ThermalRegimes` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:parmentier2018_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/parmentier_2018/report.pdf).
