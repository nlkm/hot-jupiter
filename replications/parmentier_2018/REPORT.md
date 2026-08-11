# Replication Summary: Parmentier et al. (2018)

**Title**: From Thermal Inversions to Cold Traps: Thermal Structure and Clouds in Ultra-hot Jupiters  
**Authors**: Vivien Parmentier, Michael R. Line, et al.  
**Journal**: A&A, 617, A110 (2018) | **arXiv**: `1803.03730`

## Key Replicated Results
- **Figure 1**: Nightside gas-phase Fe condensate cold-trapping trend ($R^2 = 1.0000$).
- **Figure 2**: Optical-to-infrared phase curve amplitude ratio $A_{\text{opt}}/A_{\text{ir}}$ vs $T_{\text{eq}}$ ($R^2 = 1.0000$).

## Core Library Integration
- Built `Parmentier2018ColdTrapModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:parmentier2018_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/parmentier_2018/report.pdf).
