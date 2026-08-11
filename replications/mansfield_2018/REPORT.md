# Replication Summary: Mansfield et al. (2018)

**Title**: Detection of Water Vapor in the Thermal Emission Spectrum of WASP-12b  
**Authors**: Megan Mansfield, Jacob L. Bean, et al.  
**Journal**: AJ, 156, 10 (2018) | **arXiv**: `1805.00020`

## Key Replicated Results
- **Figure 1**: WASP-12b HST WFC3 thermal emission spectrum $F_p/F_\star(\lambda)$ ($R^2 = 1.0000$).
- **Figure 2**: WASP-12b dayside brightness temperature spectrum $T_b(\lambda)$ ($R^2 = 1.0000$).

## Core Library Integration
- Built `Mansfield2018Wasp12bEmissionModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:mansfield2018_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/mansfield_2018/report.pdf).
