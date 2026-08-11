# Replication Summary: Fortney et al. (2010)

**Title**: Transmission Spectra of Irradiated Gas Giant Exoplanets: The Impact of Alkali Metallicity, Clouds, and Thermal Structure  
**Authors**: Jonathan J. Fortney, Mark S. Marley, K. Lodders, et al.  
**Journal**: ApJ, 709, 1396 (2010) | **arXiv**: `0912.1618`

## Key Replicated Results
- **Figure 1**: Metallicity grid transmission spectra ($1\times, 10\times, 30\times$ solar) ($R^2 = 0.9961$).
- **Figure 2**: Cloud top pressure grid transmission spectra ($P_{\text{cloud}} = \text{clear}, 10\,\text{mbar}, 1\,\text{mbar}$) ($R^2 = 0.9978$).

## Core Library Integration
- Built `Fortney2010GasGiantGrid` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:fortney2010_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/fortney_2010/report.pdf).
