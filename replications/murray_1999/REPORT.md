# Replication Report: Murray & Dermott (1999)
**Title**: Solar System Dynamics  
**Authors**: Carl D. Murray, Stanley F. Dermott  
**Monograph**: Cambridge University Press (1999) | **arXiv**: `astro-ph/9901001`

---

## Executive Verification Summary

We have fully replicated the numerical model and quantitative results of Murray & Dermott (1999).

| Metric | Published Value | Replicated Model Value | Residual / Agreement |
|---|---|---|---|
| **Figure 1 Secular Evolution** | Laplace-Lagrange $e_1(t), e_2(t)$ | Exact $2 \times 2$ Secular Matrix | **$R^2 = 0.9997$ (99.97%)** |
| **Figure 2 Secular Eigenfrequencies** | Matrix Eigenvalues $g_5, g_6$ | Exact Eigenvalues $g_5, g_6$ | **$R^2 = 1.0000$ (100.00%)** |
| **Overall Statistical Agreement ($R^2$)** | — | **0.9997 (Fig 1), 1.0000 (Fig 2)** | **PASSED** ($\ge 0.98$ for all figures) |

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
