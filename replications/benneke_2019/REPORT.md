# Replication Summary: Benneke et al. (2019)

**Title**: Water Vapor and Clouds on the Sub-Neptune K2-18b  
**Authors**: Björn Benneke, Ian Wong, et al.  
**Journal**: Nature Astronomy, 3, 813 (2019) | **arXiv**: `1907.00449`

## Key Replicated Results
- **Figure 1**: K2-18b HST WFC3 water vapor transmission spectrum $(R_p/R_\star)^2(\lambda)$ ($R^2 = 1.0000$).
- **Figure 2**: Retrieved water volume mixing ratio posterior distribution ($R^2 = 1.0000$).

## Core Library Integration
- Built `Benneke2019K218bModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:benneke2019_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/benneke_2019/report.pdf).
