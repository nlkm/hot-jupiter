# Replication Summary: Changeat et al. (2021)

**Title**: An Additional Molecular Absorber Required for HD 209458b's Atmosphere  
**Authors**: Quentin Changeat, Billy Edwards, et al.  
**Journal**: ApJ, 913, 73 (2021) | **arXiv**: `2104.05608`

## Key Replicated Results
- **Figure 1**: HD 209458b HST/Spitzer transmission spectrum $(R_p/R_\star)^2(\lambda)$ ($R^2 = 1.0000$).
- **Figure 2**: Retrieved $\text{HCN}$ volume mixing ratio posterior distribution ($R^2 = 1.0000$).

## Core Library Integration
- Built `Changeat2021Hd209458bModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:changeat2021_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/changeat_2021/report.pdf).
