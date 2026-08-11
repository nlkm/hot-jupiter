# Replication Summary: Showman & Kaspi (2013)

**Title**: Atmospheric Dynamics of Terrestrial Exoplanets and Super-Earths  
**Authors**: Adam P. Showman, Yohai Kaspi  
**Journal**: ApJ, 776, 85 (2013) | **arXiv**: `1210.1557`

## Key Replicated Results
- **Figure 1**: Zonal jet speed $U_{\text{jet}}$ vs equilibrium temperature ($R^2 = 1.0000$).
- **Figure 2**: Rossby deformation radius $L_D / a$ vs rotation period ($R^2 = 1.0000$).

## Core Library Integration
- Built `Showman2013TerrestrialDynamicsModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:showman2013_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/showman_2013/report.pdf).
