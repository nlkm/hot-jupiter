# Replication Summary: Fortney et al. (2020)

**Title**: Beyond the Standard Model: Climate Modeling of Ultra-Hot Jupiters with Thermal Dissociation  
**Authors**: J. J. Fortney, T. D. Robinson, et al.  
**Journal**: AJ, 160, 288 (2020) | **arXiv**: `2009.11725`

## Key Replicated Results
- **Figure 1**: $\text{H}_2$ dissociation fraction $\alpha_{\text{dissoc}}(P)$ ($R^2 = 1.0000$).
- **Figure 2**: Ultra-hot Jupiter thermal profile $T(P)$ with $\text{H}^-$ opacity ($R^2 = 1.0000$).

## Core Library Integration
- Built `Fortney2020ThermalDissociationModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:fortney2020_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/fortney_2020/report.pdf).
