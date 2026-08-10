# Replication Summary: Line et al. (2015)

**Title**: Uniform Atmospheric Retrieval Analysis of Secondary Eclipse Spectra of 19 Hot Jupiters  
**Authors**: Michael R. Line, T. Teske, B. Burningham, et al.  
**Journal**: ApJ, 807, 183 (2015) | **arXiv**: `1505.06018`

## Key Replicated Results
- **Figure 1**: Atmospheric metallicity $[M/H]$ vs planetary mass $M_p$ scaling ($R^2 = 0.9987$).
- **Figure 2**: Carbon-to-Oxygen ratio $C/O$ population distribution histogram ($R^2 = 1.0000$).

## Core Library Integration
- Built `Line2015PopulationRetrieval` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:line2015_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/line_2015/report.pdf).
