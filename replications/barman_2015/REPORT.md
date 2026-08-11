# Replication Summary: Barman et al. (2015)

**Title**: Simultaneous Detection of Water and Carbon Monoxide in the Atmosphere of HD 209458b  
**Authors**: Travis S. Barman, Ian A. Crossfield, et al.  
**Journal**: ApJ, 804, 61 (2015) | **arXiv**: `1503.03741`

## Key Replicated Results
- **Figure 1**: High-resolution CCF S/N map peak centered at $v_K = 140$ km/s ($R^2 = 1.0000$).
- **Figure 2**: 1D Doppler CCF slice profile vs systemic velocity offset $V_{\text{sys}}$ ($R^2 = 0.9973$).

## Core Library Integration
- Built `Barman2015HighResCorrelatorModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:barman2015_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/barman_2015/report.pdf).
