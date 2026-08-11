# Replication Specification: Line et al. (2021)
**Title**: A Solar C/O and Carbon Abundance in the Atmosphere of the Ultra-Hot Jupiter WASP-77Ab  
**Authors**: Michael R. Line, Matteo Brogi, et al.  
**Journal**: Nature, 598, 580 (2021) | **arXiv**: `2110.14810`

---

## Executive Summary & Core Equations

Line et al. (2021) detect $H_2O$ and $CO$ emission lines at high spectral resolution ($R \sim 100,000$) on WASP-77Ab, constraining solar metallicity and C/O ratio.

### 1. High-Resolution Cross-Correlation & C/O Ratio
$$CCF(v, K_p) = \sum_i \frac{f(\lambda_i) M(\lambda_i(1 + (v + K_p \sin 2\pi\phi)/c))}{\sigma_i^2}$$
$$\text{C/O} = \frac{X_{\text{CO}} + X_{\text{CH4}}}{X_{\text{CO}} + X_{\text{H2O}} + 2 X_{\text{CO2}}}$$

---

## Benchmark Figures to Replicate

1. **Figure 1**: High-resolution cross-correlation peak signal $S/N$ map in $(v_{\text{sys}}, K_p)$ space ($S/N = 8.5$ peak at $v_{\text{sys}} = -20.1$ km/s, $K_p = 191$ km/s).
2. **Figure 2**: Retrieved posterior probability distribution for $\log_{10} X_{\text{H2O}}$ and $\log_{10} X_{\text{CO}}$ centered at solar abundance.
