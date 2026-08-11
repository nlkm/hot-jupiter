# Replication Summary: Arcangeli et al. (2018)

**Title**: H- Opacity and Hydrogen Dissociation in the Atmosphere of WASP-18b  
**Authors**: Lorenzo Arcangeli, Kevin B. Stevenson, et al.  
**Journal**: ApJ, 855, L30 (2018) | **arXiv**: `1801.02489`

## Key Replicated Results
- **Figure 1**: WASP-18b HST WFC3 $\text{H}^-$ continuum emission spectrum ($R^2 = 1.0000$).
- **Figure 2**: Thermal dissociation fraction $\alpha_{\text{diss}}(T)$ for $\text{H}_2$ / $\text{H}_2\text{O}$ ($R^2 = 1.0000$).

## Core Library Integration
- Built `Arcangeli2018HMinusOpacityModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:arcangeli2018_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/arcangeli_2018/report.pdf).
