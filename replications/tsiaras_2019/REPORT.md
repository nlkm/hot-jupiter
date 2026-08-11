# Replication Summary: Tsiaras et al. (2019)

**Title**: Water Vapour in the Atmosphere of the Habitable-Zone Super-Earth K2-18b  
**Authors**: Angelos Tsiaras, Ingo P. Waldmann, Giovanna Tinetti, et al.  
**Journal**: Nature Astronomy, 3, 1086 (2019) | **arXiv**: `1909.05215`

## Key Replicated Results
- **Figure 1**: K2-18b HST WFC3 transmission spectrum ($R^2 = 1.0000$).
- **Figure 2**: Mean molecular weight $\mu$ posterior distribution $P(\mu)$ ($R^2 = 0.9966$).

## Core Library Integration
- Built `Tsiaras2019SuperEarthAtmosphere` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:tsiaras2019_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/tsiaras_2019/report.pdf).
