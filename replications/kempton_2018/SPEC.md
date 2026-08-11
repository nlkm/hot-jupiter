# Replication Specification: Kempton et al. (2018)
**Title**: A Framework for Prioritizing Exoplanet Targets for Atmospheric Characterization  
**Authors**: Eliza M.-R. Kempton, Jacob L. Bean, et al.  
**Journal**: PASP, 130, 114401 (2018) | **arXiv**: `1805.03671`

---

## Executive Summary & Core Equations

Kempton et al. (2018) formulate analytic Transmission (TSM) and Emission (ESM) Spectroscopy Metrics to prioritize exoplanets for JWST/HST atmospheric characterization.

### 1. Transmission & Emission Spectroscopy Metrics
$$\text{TSM} = S_{\text{scale}} \frac{R_p^3 T_{\text{eq}}}{M_p R_\star^2} 10^{-m_K / 5}$$
$$\text{ESM} = 4.29 \times 10^6 \frac{B_{7.5\mu\text{m}}(T_{\text{day}})}{B_{7.5\mu\text{m}}(T_\star)} \left(\frac{R_p}{R_\star}\right)^2 10^{-m_K / 5}$$

---

## Benchmark Figures to Replicate

1. **Figure 1**: Transmission Spectroscopy Metric (TSM) vs planet radius $R_p$ [$R_\oplus$] (1 to 20 R_Earth).
2. **Figure 2**: Emission Spectroscopy Metric (ESM) vs equilibrium temperature $T_{\text{eq}}$ [K] (300 to 3000 K).
