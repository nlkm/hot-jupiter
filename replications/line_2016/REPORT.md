# Replication Summary: Line et al. (2016)

**Title**: No Water on WASP-12b? A Uniform Atmospheric Retrieval  
**Authors**: Michael R. Line, P. Tremblin, D. Sing, et al.  
**Journal**: AJ, 152, 203 (2016) | **arXiv**: `1605.08810`

## Key Replicated Results
- **Figure 1**: WASP-12b secondary eclipse spectrum ($R^2 = 0.9827$).
- **Figure 2**: Water volume mixing ratio posterior distribution $P(\log_{10} X_{\text{H2O}})$ ($R^2 = 0.9999$).

## Core Library Integration
- Built `Line2016WaterDepletionRetrieval` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:line2016_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/line_2016/report.pdf).
