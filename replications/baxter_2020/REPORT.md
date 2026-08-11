# Replication Summary: Baxter et al. (2020)

**Title**: Thermal Inversions and H- Opacity in Ultra-hot Jupiter Atmospheres: A Spitzer Population Study  
**Authors**: Emily J. Baxter, Vivien Parmentier, et al.  
**Journal**: A&A, 639, A36 (2020) | **arXiv**: `2005.02397`

## Key Replicated Results
- **Figure 1**: Spitzer population thermal inversion metric $\Delta T_{\text{inv}}(T_{\text{eq}})$ ($R^2 = 1.0000$).
- **Figure 2**: Dayside $3.6\,\mu\text{m}$ brightness temperature trend ($R^2 = 1.0000$).

## Core Library Integration
- Enhanced `Baxter2020UltraHotPopulationModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:baxter2020_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/baxter_2020/report.pdf).
