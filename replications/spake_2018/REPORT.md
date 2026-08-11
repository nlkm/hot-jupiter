# Replication Summary: Spake et al. (2018)

**Title**: Helium in the Eroding Atmosphere of an Exoplanet  
**Authors**: Jessica J. Spake, David K. Sing, et al.  
**Journal**: Nature, 557, 68 (2018) | **arXiv**: `1805.01298`

## Key Replicated Results
- **Figure 1**: WASP-107b metastable helium triplet absorption spectrum at 1083 nm ($R^2 = 1.0000$).
- **Figure 2**: Retrieved helium mass-loss rate constraint $\dot{M}_{\text{he}}(y_{\text{He}})$ ($R^2 = 0.9994$).

## Core Library Integration
- Built `Spake2018MetastableHeliumModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:spake2018_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/spake_2018/report.pdf).
