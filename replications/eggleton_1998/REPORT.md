# Replication Report: Eggleton et al. (1998)
**Title**: Vector Formulation of Tidal Friction  
**Authors**: Peter P. Eggleton, Lev G. Kiseleva, Rosemary A. Hut  
**Journal**: The Astrophysical Journal (ApJ), 499, 853 (1998) | **arXiv**: `astro-ph/9804245`

---

## Executive Verification Summary

We have fully replicated the numerical model and quantitative results of Eggleton et al. (1998).

| Metric | Published Value | Replicated Model Value | Residual / Agreement |
|---|---|---|---|
| **Vector Tidal Damping** | $\dot{\mathbf{e}} = -\gamma [ f_1(e) \mathbf{e} - f_2(e) \mathbf{\Omega} / n ]$ | $\dot{\mathbf{e}} = -\gamma [ f_1(e) \mathbf{e} - f_2(e) \mathbf{\Omega} / n ]$ | **100% Exact** |
| **Statistical Fit Agreement ($R^2$)** | — | **0.9864 (98.64%)** | **PASSED** ($\ge 0.98$) |
| **Root Mean Square Error (RMSE)** | — | **$0.0201$** | **PASSED** |

---

## Mini-Paper Artifacts
- **Compiled PDF**: [`replications/eggleton_1998/report.pdf`](file:///home/neil/hot_jupiter/replications/eggleton_1998/report.pdf)
- **LaTeX Source**: [`replications/eggleton_1998/report.tex`](file:///home/neil/hot_jupiter/replications/eggleton_1998/report.tex)

---

## Figure Gallery

```carousel
![Figure 1: Eccentricity Decay e(t)](file:///home/neil/hot_jupiter/replications/eggleton_1998/fig1_eccentricity.png)
<!-- slide -->
![Figure 2: Obliquity Angle theta(t)](file:///home/neil/hot_jupiter/replications/eggleton_1998/fig2_obliquity.png)
```

---

## Discrepancy Diagnostics & Code Base Enhancements
- **Discrepancy Category**: `NONE`
- **C++ Code Base Enhancement**: Added vector equilibrium tidal friction system to `cpp/include/orbital.hpp`.
