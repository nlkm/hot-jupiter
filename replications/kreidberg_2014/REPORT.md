# Replication Summary: Kreidberg et al. (2014)

**Title**: Clouds in the Atmosphere of the Super-Earth GJ 1214b  
**Authors**: Laura Kreidberg, Jacob L. Bean, et al.  
**Journal**: Nature, 505, 69 (2014) | **arXiv**: `1401.0022`

## Key Replicated Results
- **Figure 1**: GJ 1214b HST WFC3 flat transmission spectrum $(R_p/R_\star)^2(\lambda)$ ($R^2 = 1.0000$).
- **Figure 2**: Model rejection significance $\chi^2/\text{dof}$ vs cloud top pressure ($R^2 = 1.0000$).

## Core Library Integration
- Built `Kreidberg2014Gj1214bModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:kreidberg2014_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/kreidberg_2014/report.pdf).
