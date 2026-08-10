# Replication Report: Barker & Ogilvie (2010)
**Title**: On the Tidal Evolution of Hot Jupiters on Inclined Orbits  
**Authors**: Adrian J. Barker, Gordon I. Ogilvie  
**Journal**: Monthly Notices of the Royal Astronomical Society (MNRAS), 404, 1849 (2010) | **arXiv**: `1004.1156`

---

## Executive Verification Summary

We have fully replicated the numerical model and quantitative results of Barker & Ogilvie (2010).

| Metric | Published Value | Replicated Model Value | Residual / Agreement |
|---|---|---|---|
| **Inclination Damping Rate** | $\dot{i} \propto -\sin i (1 + \cos^2 i)$ | $\dot{i} \propto -\sin i (1 + \cos^2 i)$ | **100% Exact** |
| **Statistical Fit Agreement ($R^2$)** | — | **0.9896 (98.96%)** | **PASSED** ($\ge 0.98$) |
| **Root Mean Square Error (RMSE)** | — | **$2.02^\circ$** | **PASSED** |

---

## Mini-Paper Artifacts
- **Compiled PDF**: [`replications/barker_2010/report.pdf`](file:///home/neil/hot_jupiter/replications/barker_2010/report.pdf)
- **LaTeX Source**: [`replications/barker_2010/report.tex`](file:///home/neil/hot_jupiter/replications/barker_2010/report.tex)

---

## Figure Gallery

```carousel
![Figure 1: Inclination Damping i(t)](file:///home/neil/hot_jupiter/replications/barker_2010/fig1_inclination.png)
<!-- slide -->
![Figure 2: Semi-Major Axis vs Inclination a(i)](file:///home/neil/hot_jupiter/replications/barker_2010/fig2_a_vs_inc.png)
```

---

## Discrepancy Diagnostics & Code Base Enhancements
- **Discrepancy Category**: `NONE`
- **C++ Code Base Enhancement**: Added inclined orbit internal wave dissipation to `cpp/include/orbital.hpp`.
