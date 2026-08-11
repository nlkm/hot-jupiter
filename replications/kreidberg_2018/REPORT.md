# Replication Summary: Kreidberg et al. (2018)

**Title**: Global Climate and Atmospheric Composition of the Ultra-Hot Jupiter WASP-103b  
**Authors**: Laura Kreidberg, Michael R. Line, et al.  
**Journal**: AJ, 156, 17 (2018) | **arXiv**: `1805.00029`

## Key Replicated Results
- **Figure 1**: WASP-103b Spitzer $4.5\,\mu\text{m}$ phase curve ($R^2 = 1.0000$).
- **Figure 2**: WASP-103b phase-dependent temperature profile ($R^2 = 0.9995$).

## Core Library Integration
- Built `Kreidberg2018Wasp103bPhaseCurveModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:kreidberg2018_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/kreidberg_2018/report.pdf).
