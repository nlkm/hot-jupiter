# Replication Summary: Lothringer et al. (2018)

**Title**: Extremely Irradiated Hot Jupiters: The Severe Impact of TiO and VO Inversion and NUV/O Continuum Opacity  
**Authors**: Joshua D. Lothringer, Travis S. Barman, Tommi Koskinen  
**Journal**: ApJ, 866, 27 (2018) | **arXiv**: `1805.00040`

## Key Replicated Results
- **Figure 1**: Dayside T-P inversion profile ($R^2 = 1.0000$).
- **Figure 2**: NUV-to-NIR emission spectrum ($R^2 = 1.0000$).

## Core Library Integration
- Built `Lothringer2018UltraHotInversionModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:lothringer2018_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/lothringer_2018/report.pdf).
