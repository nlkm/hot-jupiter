# Replication Summary: Zhang & Showman (2018a)

**Title**: Atmospheric Circulation of Tidally Locked Exoplanets: Principles and Models  
**Authors**: Xi Zhang, Adam P. Showman  
**Journal**: ApJ, 866, 1 (2018) | **arXiv**: `1808.08249`

## Key Replicated Results
- **Figure 1**: Equatorial superrotation speed $U_{\text{eq}}$ vs equilibrium temperature ($R^2 = 1.0000$).
- **Figure 2**: Day-night flux contrast amplitude $\mathcal{A}_{\text{dn}}$ vs drag timescale $\tau_{\text{drag}}$ ($R^2 = 1.0000$).

## Core Library Integration
- Built `Zhang2018aCirculationModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:zhang2018a_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/zhang_2018a/report.pdf).
