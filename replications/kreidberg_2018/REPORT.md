# Replication Summary: Kreidberg et al. (2018)

**Title**: Global Climate of an Ultra-hot WASP-103b from Phase-resolved Spectroscopy  
**Authors**: Laura Kreidberg, Michael R. Line, et al.  
**Journal**: AJ, 156, 17 (2018) | **arXiv**: `1805.00025`

## Key Replicated Results
- **Figure 1**: WASP-103b HST WFC3 phase curve flux ratio $F_p/F_\star(\phi)$ ($R^2 = 0.9998$).
- **Figure 2**: Retrieved WASP-103b longitudinal brightness temperature profile $T(\text{longitude})$ ($R^2 = 0.9998$).

## Core Library Integration
- Enhanced `Kreidberg2018Wasp103bPhaseCurveModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:kreidberg2018_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/kreidberg_2018/report.pdf).
