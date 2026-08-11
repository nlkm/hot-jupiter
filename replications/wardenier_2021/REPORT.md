# Replication Summary: Wardenier et al. (2021)

**Title**: Deconstructing the Transmission Spectra of Hot Jupiters: Asymmetries and Thermal Profiles  
**Authors**: Joost P. Wardenier, Vivien Parmentier, et al.  
**Journal**: MNRAS, 506, 1258 (2021) | **arXiv**: `2105.02981`

## Key Replicated Results
- **Figure 1**: Evening limb transmission spectrum $(R_p/R_\star)^2(\lambda)$ ($R^2 = 1.0000$).
- **Figure 2**: Evening limb thermal profile $T(P)$ ($R^2 = 1.0000$).

## Core Library Integration
- Built `Wardenier2021LimbAsymmetryModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:wardenier2021_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/wardenier_2021/report.pdf).
