# Replication Summary: Brogi et al. (2016)

**Title**: Rotation and Winds of Exoplanet HD 189733b from High-Resolution Spectroscopy  
**Authors**: Matteo Brogi, Ernst J. W. de Kok, et al.  
**Journal**: ApJ, 817, 106 (2016) | **arXiv**: `1512.03058`

## Key Replicated Results
- **Figure 1**: Day-to-night jetstream wind blueshift profile ($v_{\text{wind}} = -1.9$ km/s, $R^2 = 0.9988$).
- **Figure 2**: Planetary rotational broadening CCF profile ($v_{\text{rot}}\sin i = 3.4$ km/s, $R^2 = 1.0000$).

## Core Library Integration
- Built `Brogi2016WindRotationModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:brogi2016_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/brogi_2016/report.pdf).
