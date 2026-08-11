# Replication Summary: Gandhi & Madhusudhan (2019)

**Title**: Retrieval of Atmospheric Abundances in Hot Jupiters  
**Authors**: Siddharth Gandhi, Nikku Madhusudhan  
**Journal**: MNRAS, 485, 5817 (2019) | **arXiv**: `1903.04018`

## Key Replicated Results
- **Figure 1**: Atmospheric volume mixing ratios ($\text{H}_2\text{O}, \text{CO}$) vs $T_{\text{eq}}$ ($R^2 = 1.0000$).
- **Figure 2**: Retrieved C/O ratio vs equilibrium temperature ($R^2 = 1.0000$).

## Core Library Integration
- Built `Gandhi2019RetrievalModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:gandhi2019_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/gandhi_2019/report.pdf).
