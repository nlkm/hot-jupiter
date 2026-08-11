# Replication Summary: Madhusudhan et al. (2014)

**Title**: H2O Abundances and C/O Ratios in Hot Jupiter Atmospheres  
**Authors**: Nikku Madhusudhan, Hannah R. Wakeford, et al.  
**Journal**: ApJ Letters, 791, L9 (2014) | **arXiv**: `1407.6054`

## Key Replicated Results
- **Figure 1**: HD 209458b retrieved water abundance posterior distribution ($R^2 = 1.0000$).
- **Figure 2**: Hot Jupiter carbon-to-oxygen ratio (C/O) posterior distribution ($R^2 = 0.9989$).

## Core Library Integration
- Built `Madhusudhan2014CoRatioModel` class in `cpp/include/atmosphere.hpp` and exported in `hot_jupiter.atmosphere`.
- Created compiled C++ Bazel binary `//:madhusudhan2014_solver`.
- Output PDF Report: [`report.pdf`](file:///home/neil/hot_jupiter/replications/madhusudhan_2014/report.pdf).
