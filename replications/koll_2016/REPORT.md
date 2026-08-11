# Replication Summary: Koll & Abbot (2016)

**Title**: Decoupling Thermal Inversions and Atmospheric Circulation on Synchronous Exoplanets  
**Authors**: Daniel D. B. Koll, Dorian S. Abbot  
**Journal**: ApJ, 825, 99 (2016) | **arXiv**: `1506.01389`

## Key Replicated Results
- **Figure 1**: Day-night contrast $\Delta T_{\text{dn}}$ vs equilibrium temperature ($R^2 = 1.0000$).
- **Figure 2**: Thermal inversion strength $\eta_{\text{inv}}$ vs shortwave-to-longwave opacity ratio $\gamma$ ($R^2 = 0.9999$).

## Core Library Integration
- Built `Koll2016InversionModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:koll2016_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/koll_2016/report.pdf).
