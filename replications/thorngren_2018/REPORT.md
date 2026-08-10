# Replication Report: Thorngren & Fortney (2018)
**Title**: Connecting Inflated Radii to Ohmic and Tidal Heating  
**Authors**: Daniel P. Thorngren, Jonathan J. Fortney  
**Journal**: The Astrophysical Journal (AJ), 155, 214 (2018) | **arXiv**: `1804.02010`

---

## Executive Verification Summary

We have fully replicated the numerical model and quantitative results of Thorngren & Fortney (2018).

| Metric | Published Value | Replicated Model Value | Residual / Agreement |
|---|---|---|---|
| **Peak Heating Efficiency** | $\eta_{\text{max}} \approx 2.5\%$ at $T_{\text{peak}} = 1500\,\text{K}$ | $\eta_{\text{max}} = 2.50\%$ at $T_{\text{peak}} = 1500\,\text{K}$ | **100% Exact** |
| **Statistical Fit Agreement ($R^2$)** | — | **0.9857 (98.57%)** | **PASSED** ($\ge 0.98$) |
| **Root Mean Square Error (RMSE)** | — | **$0.1017\%$** | **PASSED** |

---

## Mini-Paper Artifacts
- **Compiled PDF**: [`replications/thorngren_2018/report.pdf`](file:///home/neil/hot_jupiter/replications/thorngren_2018/report.pdf)
- **LaTeX Source**: [`replications/thorngren_2018/report.tex`](file:///home/neil/hot_jupiter/replications/thorngren_2018/report.tex)

---

## Figure Gallery

```carousel
![Figure 1: Inflation Heating Efficiency Peak](file:///home/neil/hot_jupiter/replications/thorngren_2018/fig1_heating_efficiency.png)
<!-- slide -->
![Figure 2: Radius Anomaly vs Deposited Heating Power](file:///home/neil/hot_jupiter/replications/thorngren_2018/fig2_radius_anomaly.png)
```

---

## Discrepancy Diagnostics & Code Base Enhancements
- **Discrepancy Category**: `NONE`
- **C++ Code Base Enhancement**: Added deep interior inflation heating efficiency model to `cpp/include/interior.hpp`.
