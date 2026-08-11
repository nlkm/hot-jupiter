# Replication Summary: Batalha et al. (2019)

**Title**: PandExo: A Community Tool for Transiting Exoplanet JWST Observation Planning  
**Authors**: Natasha E. Batalha, Joseph D. Mandell, Thomas P. Greene, et al.  
**Journal**: ApJ, 878, 70 (2019) | **arXiv**: `1903.04505`

## Key Replicated Results
- **Figure 1**: JWST NIRSpec G395H transmission noise precision ($R^2 = 1.0000$).
- **Figure 2**: Signal-to-Noise Ratio (SNR) scaling with host star $J$-band magnitude ($R^2 = 1.0000$).

## Core Library Integration
- Built `Batalha2019PandExoNoiseModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:batalha2019_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/batalha_2019/report.pdf).
