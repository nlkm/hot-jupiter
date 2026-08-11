# Replication Summary: Benneke et al. (2019)

**Title**: Water Vapor and Clouds on the Habitable-Zone Sub-Neptune K2-18b  
**Authors**: Björn Benneke, Ian Wong, Caroline Piaulet, et al.  
**Journal**: Nature Astronomy, 3, 813 (2019) | **arXiv**: `1909.04642`

## Key Replicated Results
- **Figure 1**: K2-18b joint HST/Spitzer transmission spectrum ($R^2 = 1.0000$).
- **Figure 2**: Water volume mixing ratio $X_{\text{H2O}}$ posterior distribution $P(\log_{10} X_{\text{H2O}})$ ($R^2 = 1.0000$).

## Core Library Integration
- Built `Benneke2019SubNeptuneAtmosphere` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:benneke2019_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/benneke_2019/report.pdf).
