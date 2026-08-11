# Replication Summary: Line et al. (2014)

**Title**: Systematic Retrieval Analysis of Exoplanet Emission Spectra  
**Authors**: Michael R. Line, Amanda S. Burrows, et al.  
**Journal**: ApJ, 783, 70 (2014) | **arXiv**: `1309.2316`

## Key Replicated Results
- **Figure 1**: HD 189733b secondary eclipse emission spectrum $F_p/F_\star(\lambda)$ ($R^2 = 1.0000$).
- **Figure 2**: Retrieved thermal profile $T(P)$ envelope ($R^2 = 0.9998$).

## Core Library Integration
- Built `Line2014EmissionRetrievalModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:line2014_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/line_2014/report.pdf).
