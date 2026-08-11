# Replication Summary: Kempton et al. (2018)

**Title**: A Framework for Prioritizing Exoplanet Targets for Atmospheric Characterization  
**Authors**: Eliza M.-R. Kempton, Jacob L. Bean, et al.  
**Journal**: PASP, 130, 114401 (2018) | **arXiv**: `1805.03671`

## Key Replicated Results
- **Figure 1**: Transmission Spectroscopy Metric TSM vs planet radius $R_p$ ($R^2 = 1.0000$).
- **Figure 2**: Emission Spectroscopy Metric ESM vs equilibrium temperature $T_{\text{eq}}$ ($R^2 = 1.0000$).

## Core Library Integration
- Built `Kempton2018AtmosphericMetricsModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:kempton2018_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/kempton_2018/report.pdf).
