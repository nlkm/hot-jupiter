# Replication Specification: Gandhi & Madhusudhan (2019)
**Title**: Retrieval of Atmospheric Abundances in Hot Jupiters  
**Authors**: Siddharth Gandhi, Nikku Madhusudhan  
**Journal**: MNRAS, 485, 5817 (2019) | **arXiv**: `1903.04018`

---

## Executive Summary & Core Equations

Gandhi & Madhusudhan (2019) develop a Bayesian atmospheric retrieval framework to extract chemical abundances ($\text{H}_2\text{O}$, $\text{CO}$) and C/O ratios from hot Jupiter spectra.

### 1. C/O Ratio & Opacity Accumulation
$$\text{C/O} = \frac{X_{\text{CO}} + X_{\text{CH4}}}{X_{\text{H2O}} + X_{\text{CO}}}$$
$$\tau_\lambda(P) = \int_{0}^P \sum_i X_i \sigma_{i,\lambda}(T(P'), P') \frac{dP'}{\mu g}$$

---

## Benchmark Figures to Replicate

1. **Figure 1**: Volume mixing ratios $\log_{10} X_i$ ($-6.0$ to $-2.0$) vs equilibrium temperature $T_{\text{eq}}$ (1000 to 3000 K) for $\text{H}_2\text{O}$ and $\text{CO}$.
2. **Figure 2**: Retrieved C/O ratio vs equilibrium temperature $T_{\text{eq}}$ [K] (1000 to 3000 K).
