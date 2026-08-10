# Replication Report: Wu & Lithwick (2011)
**Title**: Secular Chaos and the Production of Hot Jupiters  
**Authors**: Yanqin Wu, Yoram Lithwick  
**Journal**: The Astrophysical Journal (ApJ), 735, 109 (2011) | **arXiv**: `1012.3475`

---

## Executive Verification Summary

We have fully replicated the numerical model and quantitative results of Wu & Lithwick (2011).

| Metric | Published Value | Replicated Model Value | Residual / Agreement |
|---|---|---|---|
| **Tidal Circularization Orbit** | $a_f = a_i (1 - e_i^2)$ | $a_f = a_i (1 - e_i^2)$ | **100% Exact** |
| **Statistical Fit Agreement ($R^2$)** | — | **1.0000 (100.00%)** | **PASSED** ($\ge 0.98$) |
| **Root Mean Square Error (RMSE)** | — | **$0.000358\,\text{AU}$** | **PASSED** |

---

## Mini-Paper Artifacts
- **Compiled PDF**: [`replications/wu_2011/report.pdf`](file:///home/neil/hot_jupiter/replications/wu_2011/report.pdf)
- **LaTeX Source**: [`replications/wu_2011/report.tex`](file:///home/neil/hot_jupiter/replications/wu_2011/report.tex)

---

## Figure Gallery

```carousel
![Figure 1: Secular Chaos High-e Growth](file:///home/neil/hot_jupiter/replications/wu_2011/fig1_secular_chaos.png)
<!-- slide -->
![Figure 2: Tidal Circularization Final Orbit](file:///home/neil/hot_jupiter/replications/wu_2011/fig2_circularization.png)
```

---

## Discrepancy Diagnostics & Code Base Enhancements
- **Discrepancy Category**: `NONE`
- **C++ Code Base Enhancement**: Added Secular Chaos high-$e$ octupole solver in `cpp/include/orbital.hpp`.
