# Replication Summary: Molaverdikhani et al. (2019)

**Title**: The Influence of Dispersing Clouds on Exoplanet Transmission Spectra  
**Authors**: Karan Molaverdikhani, Th. Henning, et al.  
**Journal**: A&A, 630, A131 (2019) | **arXiv**: `1908.06450`

## Key Replicated Results
- **Figure 1**: Cloud-influenced transmission spectrum ($R^2 = 1.0000$).
- **Figure 2**: Rayleigh scattering slope vs cloud deck pressure ($R^2 = 1.0000$).

## Core Library Integration
- Built `Molaverdikhani2019CloudModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:molaverdikhani2019_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/molaverdikhani_2019/report.pdf).
