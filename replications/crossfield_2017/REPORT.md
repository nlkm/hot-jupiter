# Replication Summary: Crossfield & Kreidberg (2017)

**Title**: Trends in Atmospheric Properties of Sub-Jovian Planets  
**Authors**: Ian J. M. Crossfield & Laura Kreidberg  
**Journal**: AJ, 154, 261 (2017) | **arXiv**: `1711.00949`

## Key Replicated Results
- **Figure 1**: Water absorption feature amplitude $A_{\text{H2O}}$ vs $T_{\text{eq}}$ ($R^2 = 1.0000$).
- **Figure 2**: Water absorption feature amplitude $A_{\text{H2O}}$ vs planet radius $R_p$ ($R^2 = 1.0000$).

## Core Library Integration
- Built `Crossfield2017SubJovianTrends` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:crossfield2017_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/crossfield_2017/report.pdf).
