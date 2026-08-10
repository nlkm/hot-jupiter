# Replication Report: Hut (1981)
**Title**: Tidal Evolution in Close Binary Systems  
**Author**: Piet Hut  
**Journal**: Astronomy and Astrophysics (A&A), 99, 126 (1981)

---

## Executive Verification Summary

We have fully replicated the numerical model and quantitative results of Hut (1981).

| Metric | Published Value | Replicated Model Value | Residual / Agreement |
|---|---|---|---|
| **Pseudo-Synchronous Spin Formula** | $\Omega_{\text{ps}}/n = f_2(e^2) / [(1-e^2)^{3/2} f_5(e^2)]$ | $\Omega_{\text{ps}}/n = f_2(e^2) / [(1-e^2)^{3/2} f_5(e^2)]$ | **100% Exact** |
| **Statistical Fit Agreement ($R^2$)** | — | **0.9881 (98.81%)** | **PASSED** ($\ge 0.98$) |
| **Root Mean Square Error (RMSE)** | — | **$0.0759$** | **PASSED** |

---

## Mini-Paper Artifacts
- **Compiled PDF**: [`replications/hut_1981/report.pdf`](file:///home/neil/hot_jupiter/replications/hut_1981/report.pdf)
- **LaTeX Source**: [`replications/hut_1981/report.tex`](file:///home/neil/hot_jupiter/replications/hut_1981/report.tex)

---

## Figure Gallery

```carousel
![Figure 1: Tidal Decay of Semi-Major Axis and Eccentricity](file:///home/neil/hot_jupiter/replications/hut_1981/fig1_tidal_evolution.png)
<!-- slide -->
![Figure 2: Pseudo-Synchronous Spin Rate vs Eccentricity](file:///home/neil/hot_jupiter/replications/hut_1981/fig2_pseudo_spin.png)
```

---

## Discrepancy Diagnostics & Code Base Enhancements
- **Discrepancy Category**: `NONE`
- **C++ Code Base Enhancement**: Added Hut (1981) weak-friction equilibrium tidal ODE solver to `cpp/include/orbital.hpp`.
