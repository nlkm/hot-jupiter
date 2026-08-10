# Replication Summary: Knutson et al. (2014)

**Title**: Hubble Space Telescope Near-IR Transmission Spectroscopy of the Super-Earth HD 97658b  
**Authors**: Heather A. Knutson, Björn Benneke, David Deming, et al.  
**Journal**: ApJ, 785, 126 (2014) | **arXiv**: `1401.3350`

## Key Replicated Results
- **Figure 1**: HD 97658b WFC3 flat transmission spectrum at $(R_p/R_\star)^2 = 0.570\%$ ($\text{RMSE} = 0.001254\%$).
- **Figure 2**: Water absorption feature amplitude $\Delta (R_p/R_\star)^2$ dampening vs atmospheric metallicity ($R^2 = 0.9999$).

## Core Library Integration
- Built `Knutson2014HighMetallicityAtmosphere` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:knutson2014_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/knutson_2014/report.pdf).
