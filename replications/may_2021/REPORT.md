# Replication Summary: May et al. (2021)

**Title**: Spitzer Phase Curves of WASP-76b and WASP-121b  
**Authors**: E. M. May, G. L. Stevenson, et al.  
**Journal**: AJ, 162, 158 (2021) | **arXiv**: `2107.03437`

## Key Replicated Results
- **Figure 1**: WASP-76b $4.5\,\mu\text{m}$ Spitzer phase curve ($R^2 = 0.9999$).
- **Figure 2**: WASP-121b $4.5\,\mu\text{m}$ Spitzer phase curve ($R^2 = 0.9999$).

## Core Library Integration
- Built `May2021UltraHotPhaseCurveModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:may2021_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/may_2021/report.pdf).
