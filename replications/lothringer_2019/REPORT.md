# Replication Summary: Lothringer & Barman (2019)

**Title**: The Influence of Stellar Spectral Type on Ultra-hot Jupiter Atmospheres  
**Authors**: Joshua D. Lothringer, Travis S. Barman  
**Journal**: ApJ, 876, 69 (2019) | **arXiv**: `1904.00031`

## Key Replicated Results
- **Figure 1**: Atmospheric $T(P)$ profiles across stellar spectral classes (F, G, K, M) ($R^2 = 1.0000$).
- **Figure 2**: Emergent dayside thermal emission spectra across host star spectral types ($R^2 = 1.0000$).

## Core Library Integration
- Built `Lothringer2019StellarSpectralTypeModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:lothringer2019_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/lothringer_2019/report.pdf).
