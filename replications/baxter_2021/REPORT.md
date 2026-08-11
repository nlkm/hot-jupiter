# Replication Summary: Baxter et al. (2021)

**Title**: A Transition Between Eclipse Depth Trends for Ultra-Hot Jupiters  
**Authors**: C. Baxter, I. Crossfield, et al.  
**Journal**: A&A, 648, A127 (2021) | **arXiv**: `2103.01955`

## Key Replicated Results
- **Figure 1**: Spitzer $3.6\,\mu\text{m}$ secondary eclipse depth trend ($R^2 = 1.0000$).
- **Figure 2**: Spitzer $4.5\,\mu\text{m}$ secondary eclipse depth trend ($R^2 = 1.0000$).

## Core Library Integration
- Built `Baxter2021EclipseTransitionModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:baxter2021_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/baxter_2021/report.pdf).
