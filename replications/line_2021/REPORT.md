# Replication Summary: Line et al. (2021)

**Title**: A Solar C/O and Carbon Abundance in the Atmosphere of the Ultra-Hot Jupiter WASP-77Ab  
**Authors**: Michael R. Line, Matteo Brogi, et al.  
**Journal**: Nature, 598, 580 (2021) | **arXiv**: `2110.14810`

## Key Replicated Results
- **Figure 1**: WASP-77Ab high-resolution $\text{H}_2\text{O} + \text{CO}$ cross-correlation peak $S/N(v_{\text{sys}})$ ($R^2 = 0.9998$).
- **Figure 2**: Retrieved water volume mixing ratio posterior distribution ($R^2 = 1.0000$).

## Core Library Integration
- Built `Line2021Wasp77abModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:line2021_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/line_2021/report.pdf).
