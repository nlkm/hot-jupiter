# Replication Summary: Welbanks et al. (2019)

**Title**: Mass-Metallicity Trends in Exoplanet Atmospheres: Colossal Water Depletion  
**Authors**: Luis Welbanks, Nikku Madhusudhan, Leonardo Allard, et al.  
**Journal**: ApJL, 887, L20 (2019) | **arXiv**: `1912.04291`

## Key Replicated Results
- **Figure 1**: WASP-127b transmission spectrum ($R^2 = 0.9944$).
- **Figure 2**: Water $[H_2O/H]$ and Sodium $[Na/H]$ mass-abundance scaling ($R^2 = 1.0000$).

## Core Library Integration
- Built `Welbanks2019WaterDepletion` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:welbanks2019_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/welbanks_2019/report.pdf).
