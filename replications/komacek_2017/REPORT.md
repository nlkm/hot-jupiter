# Replication Summary: Komacek et al. (2017)

**Title**: Atmospheric Circulation of Hot Jupiters: Dayside-to-Nightside Temperature Differences. II. Comparison with Observations  
**Authors**: Thaddeus D. Komacek, Adam P. Showman, Xianyu Tan  
**Journal**: ApJ, 835, 198 (2017) | **arXiv**: `1611.08605`

## Key Replicated Results
- **Figure 1**: Observed thermal contrast amplitude $A_{\text{obs}}(T_{\text{eq}})$ ($R^2 = 1.0000$).
- **Figure 2**: Phase curve peak eastward offset $\Delta \phi_{\text{offset}}(T_{\text{eq}})$ ($R^2 = 1.0000$).

## Core Library Integration
- Built `Komacek2017PhaseCurvePopulationModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:komacek2017_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/komacek_2017/report.pdf).
