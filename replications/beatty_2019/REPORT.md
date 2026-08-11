# Replication Summary: Beatty et al. (2019)

**Title**: Spitzer Phase Curves of KELT-1b and the Trends in Day-Night Heat Redistribution for Highly Irradiated Planet-Mass Companions  
**Authors**: Thomas G. Beatty, Marley C. Cooper, et al.  
**Journal**: AJ, 158, 166 (2019) | **arXiv**: `1812.08726`

## Key Replicated Results
- **Figure 1**: KELT-1b Spitzer $3.6\,\mu\text{m}$ phase curve ($R^2 = 1.0000$).
- **Figure 2**: Day-night recirculation efficiency $\varepsilon$ vs $T_{\text{eq}}$ ($R^2 = 1.0000$).

## Core Library Integration
- Built `Beatty2019Kelt1bPhaseCurveModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:beatty2019_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/beatty_2019/report.pdf).
