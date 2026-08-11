# Replication Summary: Mansfield et al. (2021)

**Title**: Confirmation of Water Emission and Thermal Inversion in WASP-33b  
**Authors**: Megan Mansfield, Jacob L. Bean, et al.  
**Journal**: Nature Astronomy, 5, 1224 (2021) | **arXiv**: `2110.09540`

## Key Replicated Results
- **Figure 1**: WASP-33b HST WFC3 secondary eclipse emission spectrum $F_p/F_\star(\lambda)$ ($R^2 = 0.9997$).
- **Figure 2**: Day-side thermal inversion profile $T(P)$ ($R^2 = 1.0000$).

## Core Library Integration
- Built `Mansfield2021Wasp33bModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:mansfield2021_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/mansfield_2021/report.pdf).
