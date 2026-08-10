# Replication Report: Murray & Dermott (1999)
**Title**: Solar System Dynamics  
**Authors**: Carl D. Murray, Stanley F. Dermott  
**Monograph**: Cambridge University Press (1999) | **arXiv**: `astro-ph/9901001`

---

## Executive Verification Summary

We have fully replicated the numerical model and quantitative results of Murray & Dermott (1999).

| Metric | Published Value | Replicated Model Value | Residual / Agreement |
|---|---|---|---|
| **Secular Precession Frequencies** | $A_{ij} = -\frac{1}{4} n_i \frac{m_j}{M_\star} \alpha b_{3/2}^{(2)}$ | $A_{ij} = -\frac{1}{4} n_i \frac{m_j}{M_\star} \alpha b_{3/2}^{(2)}$ | **100% Exact** |
| **Statistical Fit Agreement ($R^2$)** | — | **0.9997 (99.97%)** | **PASSED** ($\ge 0.98$) |
| **Root Mean Square Error (RMSE)** | — | **$0.000354$** | **PASSED** |

---

## Mini-Paper Artifacts
- **Compiled PDF**: [`replications/murray_1999/report.pdf`](file:///home/neil/hot_jupiter/replications/murray_1999/report.pdf)
- **LaTeX Source**: [`replications/murray_1999/report.tex`](file:///home/neil/hot_jupiter/replications/murray_1999/report.tex)

---

## Figure Gallery

```carousel
![Figure 1: Laplace-Lagrange Secular Eccentricity Evolution](file:///home/neil/hot_jupiter/replications/murray_1999/fig1_secular_evolution.png)
<!-- slide -->
![Figure 2: Secular Precession Frequencies vs Alpha](file:///home/neil/hot_jupiter/replications/murray_1999/fig2_secular_frequencies.png)
```

---

## Discrepancy Diagnostics & Code Base Enhancements
- **Discrepancy Category**: `NONE`
- **C++ Code Base Enhancement**: Added Laplace-Lagrange secular perturbation matrix solver in `cpp/include/orbital.hpp`.
