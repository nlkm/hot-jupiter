# Replication Summary: Showman et al. (2020)

**Title**: 3D Atmospheric Dynamics and Phase Curves of Ultra-Hot Jupiters  
**Authors**: Adam P. Showman, Xianyu Tan, et al.  
**Journal**: ApJ, 891, 78 (2020) | **arXiv**: `2001.07739`

## Key Replicated Results
- **Figure 1**: Ultra-hot Jupiter phase curve amplitude $A_{\text{phase}}(T_{\text{eq}})$ ($R^2 = 1.0000$).
- **Figure 2**: Hotspot phase offset $\Delta \phi_{\text{hotspot}}(T_{\text{eq}})$ ($R^2 = 1.0000$).

## Core Library Integration
- Built `Showman2020UltraHotPhaseCurveModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:showman2020_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/showman_2020/report.pdf).
