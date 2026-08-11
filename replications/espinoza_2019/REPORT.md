# Replication Summary: Espinoza et al. (2019)

**Title**: ACCESS: Confirmation of a Clear Atmosphere for WASP-19b  
**Authors**: Néstor Espinoza, Benjamin V. Rackham, Mercedes López-Morales, et al.  
**Journal**: MNRAS, 482, 2065 (2019) | **arXiv**: `1808.00688`

## Key Replicated Results
- **Figure 1**: WASP-19b optical transmission spectrum ($R^2 = 0.9988$).
- **Figure 2**: Sodium volume mixing ratio posterior distribution $P(\log_{10} X_{\text{Na}})$ ($R^2 = 1.0000$).

## Core Library Integration
- Built `Espinoza2019ClearAtmosphere` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:espinoza2019_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/espinoza_2019/report.pdf).
