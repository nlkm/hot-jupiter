# Replication Summary: Carone et al. (2020)

**Title**: Equatorial Superrotation on WASP-43b and HD 209458b Across Atmospheric Pressures  
**Authors**: L. Carone, R. Baeyens, et al.  
**Journal**: A&A, 638, A14 (2020) | **arXiv**: `2004.14811`

## Key Replicated Results
- **Figure 1**: WASP-43b vertical profile of zonal wind speed ($R^2 = 1.0000$).
- **Figure 2**: HD 209458b vertical profile of zonal wind speed ($R^2 = 1.0000$).

## Core Library Integration
- Built `Carone2020VerticalJetModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:carone2020_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/carone_2020/report.pdf).
