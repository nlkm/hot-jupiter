# Replication Summary: Komacek & Showman (2016)

**Title**: Atmospheric Circulation of Hot Jupiters: Dayside-to-Nightside Temperature Differences  
**Authors**: Thaddeus D. Komacek, Adam P. Showman  
**Journal**: ApJ, 821, 16 (2016) | **arXiv**: `1512.07279`

## Key Replicated Results
- **Figure 1**: Day-night fractional thermal contrast $A(T_{\text{eq}})$ ($R^2 = 1.0000$).
- **Figure 2**: Day-night contrast $A(\gamma_{\text{drag}})$ vs wave drag ($R^2 = 1.0000$).

## Core Library Integration
- Built `Komacek2016ThermalContrastModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:komacek2016_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/komacek_2016/report.pdf).
