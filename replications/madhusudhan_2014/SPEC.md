# Replication Specification: Madhusudhan et al. (2014)
**Title**: Exoplanetary Atmospheres: Chemistry, Composition, and Cloud Structure  
**Authors**: Nikku Madhusudhan, H. Knutson, Jonathan J. Fortney, et al.  
**Journal**: Space Science Reviews, 186, 269 (2014) | **arXiv**: `1402.1169`

---

## Executive Summary & Core Equations

Madhusudhan et al. (2014) review the chemical composition of hot Jupiter atmospheres, demonstrating the critical transition at $C/O = 1.0$ between oxygen-rich ($H_2O$ dominated) and carbon-rich ($CH_4, CO, HCN$ dominated) regimes.

### 1. C/O Ratio Chemical Equilibrium Abundance Transition Formula
$$X_{H2O}(C/O) = \begin{cases} 5.0 \times 10^{-4} (1 - C/O) & \text{if } C/O < 1.0 \\ 1.0 \times 10^{-6} & \text{if } C/O \ge 1.0 \end{cases}$$
$$X_{CH4}(C/O) = \begin{cases} 1.0 \times 10^{-6} & \text{if } C/O < 1.0 \\ 4.0 \times 10^{-4} (C/O - 1) & \text{if } C/O \ge 1.0 \end{cases}$$

---

## Benchmark Figures to Replicate

1. **Figure 1**: Volume mixing ratios $\log_{10} X_i$ ($H_2O, CO, CH_4, CO_2$) vs Temperature $T$ [K] for $C/O=0.5$ (solar).
2. **Figure 2**: $H_2O$ volume mixing ratio $\log_{10} X_{H2O}$ vs $C/O$ ratio ($0.2 \le C/O \le 1.5$).
