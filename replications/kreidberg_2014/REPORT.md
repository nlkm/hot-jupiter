# Replication Summary: Kreidberg et al. (2014)

**Title**: Clouds in the Atmosphere of the Super-Earth GJ 1214b  
**Authors**: Laura Kreidberg, Jacob L. Bean, Jean-Michel Désert, et al.  
**Journal**: Nature, 505, 69 (2014) | **arXiv**: `1401.0022`

## Key Replicated Results
- **Figure 1**: GJ 1214b WFC3 flat transmission spectrum at $(R_p/R_\star)^2 = 1.345\%$ ($\text{RMSE} = 0.000756\%$).
- **Figure 2**: Water absorption feature amplitude $\Delta (R_p/R_\star)^2$ dampening vs cloud deck pressure ($R^2 = 1.0000$).

## Core Library Integration
- Built `Kreidberg2014CloudyAtmosphere` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:kreidberg2014_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/kreidberg_2014/report.pdf).
