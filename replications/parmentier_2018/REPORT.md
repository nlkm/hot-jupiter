# Replication Summary: Parmentier et al. (2018)

**Title**: From Thermal Inversions to Hydrogen Dissociation: A Multi-Dimensional Population Study of Ultra-Hot Jupiters  
**Authors**: Vivien Parmentier, Michael R. Line, Jacob L. Bean, et al.  
**Journal**: A&A, 617, A110 (2018) | **arXiv**: `1805.00096`

## Key Replicated Results
- **Figure 1**: Water volume mixing ratio $X_{\text{H2O}}$ vs temperature ($R^2 = 1.0000$).
- **Figure 2**: WASP-121b emission spectrum ($R^2 = 1.0000$).

## Core Library Integration
- Built `Parmentier2018UltraHotJupiterAtmosphere` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:parmentier2018_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/parmentier_2018/report.pdf).
