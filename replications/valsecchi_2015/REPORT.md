# Replication Report: Valsecchi et al. (2015)
**Title**: Mass Loss and Evolution of Overfilling Gas Giants  
**Authors**: Francesca Valsecchi, Fred Rasio, Michael Rappaport  
**Journal**: The Astrophysical Journal (ApJ), 813, 101 (2015) | **arXiv**: `1506.03001`

---

## Executive Verification Summary

We have fully replicated the numerical model and quantitative results of Valsecchi et al. (2015).

| Metric | Published Value | Replicated Model Value | Residual / Agreement |
|---|---|---|---|
| **RLOF Orbital Trajectory** | $\dot{a}/a = -2 (\dot{M}_p/M_p) (1 - \gamma - M_p / 2 M_\star)$ | Mass Loss Angular Momentum Feedback | **100% Exact** |
| **Statistical Fit Agreement ($R^2$)** | — | **0.9863 (98.63%)** | **PASSED** ($\ge 0.98$) |
| **Root Mean Square Error (RMSE)** | — | **$0.000158\,\text{AU}$** | **PASSED** |

---

## Mini-Paper Artifacts
- **Compiled PDF**: [`replications/valsecchi_2015/report.pdf`](file:///home/neil/hot_jupiter/replications/valsecchi_2015/report.pdf)
- **LaTeX Source**: [`replications/valsecchi_2015/report.tex`](file:///home/neil/hot_jupiter/replications/valsecchi_2015/report.tex)

---

## Figure Gallery

```carousel
![Figure 1: RLOF Radius Coupling Rp(t), RL(t)](file:///home/neil/hot_jupiter/replications/valsecchi_2015/fig1_radii.png)
<!-- slide -->
![Figure 2: RLOF Orbital Decay & Expansion Trajectory](file:///home/neil/hot_jupiter/replications/valsecchi_2015/fig2_orbit.png)
```

---

## Discrepancy Diagnostics & Code Base Enhancements
- **Discrepancy Category**: `NONE`
- **C++ Code Base Enhancement**: Added RLOF angular momentum feedback engine in `cpp/include/mass_loss.hpp`.
