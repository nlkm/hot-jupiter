# Replication Summary: Beatty et al. (2019)

**Title**: The Spitzer Phase Curve of KELT-1b: A High-mass Brown Dwarf  
**Authors**: Thomas G. Beatty, Marley S. Marley, et al.  
**Journal**: AJ, 158, 166 (2019) | **arXiv**: `1811.05477`

## Key Replicated Results
- **Figure 1**: KELT-1b Spitzer $3.6\,\mu\text{m}$ phase curve ($R^2 = 1.0000$).
- **Figure 2**: Recirculation efficiency $\varepsilon(T_{\text{eq}})$ trend ($R^2 = 1.0000$).

## Core Library Integration
- Built `Beatty2019Kelt1bPhaseCurveModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:beatty2019_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/beatty_2019/report.pdf).
