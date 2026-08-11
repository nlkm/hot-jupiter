# Replication Summary: Showman et al. (2015)

**Title**: 3D Atmospheric Circulation Models of Hot Jupiters: Atmospheric Waves and Thermal Structure  
**Authors**: Adam P. Showman, X. Tan, et al.  
**Journal**: ApJ, 801, 95 (2015) | **arXiv**: `1411.4728`

## Key Replicated Results
- **Figure 1**: Thermal hotspot eastward phase offset $\Delta \phi_{\text{hotspot}}$ vs radiative timescale $\tau_{\text{rad}}$ ($R^2 = 0.9999$).
- **Figure 2**: Day-night temperature contrast $\Delta T_{\text{day-night}}$ vs pressure $P$ ($R^2 = 1.0000$).

## Core Library Integration
- Built `Showman2015CirculationModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:showman2015_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/showman_2015/report.pdf).
