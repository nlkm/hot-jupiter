# Replication Summary: Welbanks et al. (2019)

**Title**: Mass-Metallicity Trends in Exoplanet Atmospheres: A Global Retrieval of 19 Hot Jupiters  
**Authors**: Luis Welbanks, Nikku Madhusudhan, et al.  
**Journal**: ApJL, 887, L20 (2019) | **arXiv**: `1910.12984`

## Key Replicated Results
- **Figure 1**: Atmospheric water abundance $\log_{10} X_{\text{H2O}}$ vs planetary mass ($R^2 = 0.9995$).
- **Figure 2**: Mass-metallicity scaling trend across 19 hot Jupiters ($R^2 = 1.0000$).

## Core Library Integration
- Built `Welbanks2019MassMetallicityModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:welbanks2019_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/welbanks_2019/report.pdf).
