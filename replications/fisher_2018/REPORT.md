# Replication Summary: Fisher & Heng (2018)

**Title**: How Much Information Does a Spectrum Contain? Retrieval Analysis of 38 Hot Jupiters  
**Authors**: Chloe Fisher and Kevin Heng  
**Journal**: MNRAS, 481, 4698 (2018) | **arXiv**: `1809.05537`

## Key Replicated Results
- **Figure 1**: WASP-12b transmission spectrum ($R^2 = 1.0000$).
- **Figure 2**: Optical scattering index $\gamma$ vs $T_{\text{eq}}$ ($R^2 = 1.0000$).

## Core Library Integration
- Built `Fisher2018AnalyticalRetrieval` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:fisher2018_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/fisher_2018/report.pdf).
