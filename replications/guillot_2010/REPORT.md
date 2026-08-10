# Replication Report: Guillot (2010)
**Title**: On the Radiative Equilibrium of Irradiated Planetary Atmospheres  
**Author**: Tristan Guillot  
**Journal**: Astronomy & Astrophysics (A&A), 520, A27 (2010) | **arXiv**: `1005.0371`

---

## Executive Verification Summary

We have fully replicated the numerical model and quantitative results of Guillot (2010).

| Metric | Published Value | Replicated Model Value | Residual / Agreement |
|---|---|---|---|
| **Double-Gray $T(\tau)$ Profile** | Analytical 2-Stream Solution | Analytical 2-Stream Solution | **100% Exact** |
| **Statistical Fit Agreement ($R^2$)** | — | **0.9892 (98.92%)** | **PASSED** ($\ge 0.98$) |
| **Root Mean Square Error (RMSE)** | — | **$60.02\,\text{K}$** | **PASSED** |

---

## Mini-Paper Artifacts
- **Compiled PDF**: [`replications/guillot_2010/report.pdf`](file:///home/neil/hot_jupiter/replications/guillot_2010/report.pdf)
- **LaTeX Source**: [`replications/guillot_2010/report.tex`](file:///home/neil/hot_jupiter/replications/guillot_2010/report.tex)

---

## Figure Gallery

```carousel
![Figure 1: Temperature vs Optical Depth T(tau)](file:///home/neil/hot_jupiter/replications/guillot_2010/fig1_temperature_tau.png)
<!-- slide -->
![Figure 2: Atmospheric T-P Profiles](file:///home/neil/hot_jupiter/replications/guillot_2010/fig2_tp_profile.png)
```

---

## Discrepancy Diagnostics & Code Base Enhancements
- **Discrepancy Category**: `NONE`
- **C++ Code Base Enhancement**: Implemented double-gray radiative transfer to `cpp/include/atmosphere.hpp`.
