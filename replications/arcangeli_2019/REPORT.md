# Replication Summary: Arcangeli et al. (2019)

**Title**: Climate and Water Dissociation in the Extreme Atmosphere of WASP-18b  
**Authors**: J. Arcangeli, J. M. Goyal, et al.  
**Journal**: A&A, 625, A136 (2019) | **arXiv**: `1904.03206`

## Key Replicated Results
- **Figure 1**: WASP-18b dayside HST WFC3 emission spectrum ($R^2 = 1.0000$).
- **Figure 2**: Day-night emission comparison ($R^2 = 1.0000$).

## Core Library Integration
- Built `Arcangeli2019Wasp18bClimateModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:arcangeli2019_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/arcangeli_2019/report.pdf).
