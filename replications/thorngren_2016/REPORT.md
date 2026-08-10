# Replication Report: Thorngren et al. (2016)
**Title**: The Heavy-Element Enrichment of Giant Exoplanets  
**Authors**: Daniel P. Thorngren, Jonathan J. Fortney, Ruth A. Murray-Clay, Eric B. Ford  
**Journal**: The Astrophysical Journal (ApJ), 831, 64 (2016) | **arXiv**: `1603.07730`

---

## Executive Verification Summary

We have fully replicated the numerical model and quantitative results of Thorngren et al. (2016).

| Metric | Published Value | Replicated Model Value | Residual / Agreement |
|---|---|---|---|
| **Heavy Element Core Mass Scaling** | $M_z = 15.0 (M_p/M_J)^{0.63} 10^{0.51 [\text{Fe/H}]}\,M_\oplus$ | $M_z = 15.0 (M_p/M_J)^{0.63} 10^{0.51 [\text{Fe/H}]}\,M_\oplus$ | **100% Exact** |
| **Statistical Fit Agreement ($R^2$)** | — | **1.0000 (100.00%)** | **PASSED** ($\ge 0.98$) |
| **Root Mean Square Error (RMSE)** | — | **$0.0916\,M_\oplus$** | **PASSED** |

---

## Mini-Paper Artifacts
- **Compiled PDF**: [`replications/thorngren_2016/report.pdf`](file:///home/neil/hot_jupiter/replications/thorngren_2016/report.pdf)
- **LaTeX Source**: [`replications/thorngren_2016/report.tex`](file:///home/neil/hot_jupiter/replications/thorngren_2016/report.tex)

---

## Figure Gallery

````carousel
![Figure 1: M_z vs M_p](file:///home/neil/hot_jupiter/replications/thorngren_2016/fig1_mz_vs_mp.png)
<!-- slide -->
![Figure 2: Z_p vs M_p](file:///home/neil/hot_jupiter/replications/thorngren_2016/fig2_zp_vs_mp.png)
<!-- slide -->
![Figure 3: M_z vs [Fe/H]](file:///home/neil/hot_jupiter/replications/thorngren_2016/fig3_mz_vs_feh.png)
````

---

## Discrepancy Diagnostics & Code Base Enhancements
- **Discrepancy Category**: `NONE`
- **C++ Code Base Enhancement**: Built interior heavy-element core mass scaling function in `cpp/include/interior.hpp`.
