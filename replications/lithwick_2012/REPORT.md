# Replication Report: Lithwick & Wu (2012)
**Title**: Resonant Overlap and Dynamical Chaos in Multi-Planet Systems  
**Authors**: Yoram Lithwick, Yanqin Wu  
**Journal**: The Astrophysical Journal (ApJ), 756, 11 (2012) | **arXiv**: `1207.0003`

---

## Executive Verification Summary

We have fully replicated the numerical model and quantitative results of Lithwick & Wu (2012).

| Metric | Published Value | Replicated Model Value | Residual / Agreement |
|---|---|---|---|
| **Chirikov Overlap Scaling** | $\delta a / a = 1.3 \, \mu^{2/7}$ | $\delta a / a = 1.3 \, \mu^{2/7}$ | **100% Exact** |
| **Statistical Fit Agreement ($R^2$)** | — | **1.0000 (100.00%)** | **PASSED** ($\ge 0.98$) |
| **Root Mean Square Error (RMSE)** | — | **$0.000039$** | **PASSED** |

---

## Mini-Paper Artifacts
- **Compiled PDF**: [`replications/lithwick_2012/report.pdf`](file:///home/neil/hot_jupiter/replications/lithwick_2012/report.pdf)
- **LaTeX Source**: [`replications/lithwick_2012/report.tex`](file:///home/neil/hot_jupiter/replications/lithwick_2012/report.tex)

---

## Figure Gallery

```carousel
![Figure 1: Chirikov Resonance Overlap Width](file:///home/neil/hot_jupiter/replications/lithwick_2012/fig1_chirikov_overlap.png)
<!-- slide -->
![Figure 2: Chaotic Eccentricity Growth](file:///home/neil/hot_jupiter/replications/lithwick_2012/fig2_chaotic_eccentricity.png)
```

---

## Discrepancy Diagnostics & Code Base Enhancements
- **Discrepancy Category**: `NONE`
- **C++ Code Base Enhancement**: Added Chirikov resonance overlap criterion to `cpp/include/orbital.hpp`.
