# Replication Summary: Pinhas et al. (2019)

**Title**: H2O Abundances and Cloud Properties in 10 Hot Jupiter Atmospheres  
**Authors**: Arazi Pinhas, Nikku Madhusudhan, Siddharth Gandhi, et al.  
**Journal**: MNRAS, 482, 1485 (2019) | **arXiv**: `1808.01283`

## Key Replicated Results
- **Figure 1**: WASP-31b transmission spectrum ($R^2 = 0.9991$).
- **Figure 2**: Water mixing ratio $\log_{10} X_{\text{H2O}}$ vs $T_{\text{eq}}$ ($R^2 = 1.0000$).

## Core Library Integration
- Built `Pinhas2019WaterRetrieval` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:pinhas2019_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/pinhas_2019/report.pdf).
