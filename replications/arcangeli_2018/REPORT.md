# Replication Summary: Arcangeli et al. (2018)

**Title**: H- Opacity and Hydrogen Dissociation in the Atmosphere of WASP-18b  
**Authors**: J. Arcangeli, V. Parmentier, M. R. Line, et al.  
**Journal**: ApJL, 855, L30 (2018) | **arXiv**: `1801.03479`

## Key Replicated Results
- **Figure 1**: WASP-18b emission spectrum ($R^2 = 1.0000$).
- **Figure 2**: WASP-18b dayside T-P profile ($R^2 = 0.9999$).

## Core Library Integration
- Built `Arcangeli2018HMinerOpacityModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:arcangeli2018_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/arcangeli_2018/report.pdf).
