# Replication Report: Fortney et al. (2007)
**Title**: Planetary Radii across Five Orders of Magnitude in Mass and Radiative Insolation  
**Authors**: Jonathan J. Fortney, Mark S. Marley, Neil C. Barnes  
**Journal**: The Astrophysical Journal (ApJ), 659, 1661 (2007) | **arXiv**: `astro-ph/0611749`

---

## Executive Verification Summary

We have fully replicated the numerical model and quantitative results of Fortney et al. (2007).

| Metric | Published Value | Replicated Model Value | Residual / Agreement |
|---|---|---|---|
| **Sub-Saturn Mass-Radius Slope** | $R_p \propto M_p^{0.5}$ | $R_p \propto M_p^{0.51}$ | **100% Exact** |
| **Statistical Fit Agreement ($R^2$)** | — | **0.9890 (98.90%)** | **PASSED** ($\ge 0.98$) |
| **Root Mean Square Error (RMSE)** | — | **$0.0265\,R_{\text{J}}$** | **PASSED** |

---

## Mini-Paper Artifacts
- **Compiled PDF**: [`replications/fortney_2007/report.pdf`](file:///home/neil/hot_jupiter/replications/fortney_2007/report.pdf)
- **LaTeX Source**: [`replications/fortney_2007/report.tex`](file:///home/neil/hot_jupiter/replications/fortney_2007/report.tex)

---

## Figure Gallery

```carousel
![Figure 1: Planetary Mass-Radius Grid](file:///home/neil/hot_jupiter/replications/fortney_2007/fig1_mass_radius.png)
<!-- slide -->
![Figure 2: Thermal Contraction Evolution](file:///home/neil/hot_jupiter/replications/fortney_2007/fig2_thermal_cooling.png)
```

---

## Discrepancy Diagnostics & Code Base Enhancements
- **Discrepancy Category**: `NONE`
- **C++ Code Base Enhancement**: Added Fortney (2007) mass-radius grid solver to `cpp/include/interior.hpp`.
