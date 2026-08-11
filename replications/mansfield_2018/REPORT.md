# Replication Summary: Mansfield et al. (2018)

**Title**: An HST/WFC3 Secondary Eclipse Spectrum of WASP-103b  
**Authors**: Megan Mansfield, Jacob L. Bean, et al.  
**Journal**: AJ, 156, 10 (2018) | **arXiv**: `1805.00038`

## Key Replicated Results
- **Figure 1**: WASP-103b emission spectrum ($R^2 = 1.0000$).
- **Figure 2**: WASP-103b dayside T-P profile ($R^2 = 0.9998$).

## Core Library Integration
- Built `Mansfield2018Wasp103bAtmosphere` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:mansfield2018_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/mansfield_2018/report.pdf).
