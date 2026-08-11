# Replication Specification: Barman et al. (2015)
**Title**: Simultaneous Detection of Water and Carbon Monoxide in the Atmosphere of HD 209458b  
**Authors**: Travis S. Barman, Ian A. Crossfield, et al.  
**Journal**: ApJ, 804, 61 (2015) | **arXiv**: `1503.03741`

---

## Executive Summary & Core Equations

Barman et al. (2015) report high-resolution near-infrared spectroscopy of HD 209458b, detecting $\text{H}_2\text{O}$ and $\text{CO}$ lines simultaneously using Doppler cross-correlation.

### 1. High-Resolution Doppler Cross-Correlation
$$CCF(v_K, V_{\text{sys}}) = \frac{\sum_i f(\lambda_i - \Delta \lambda_i) m(\lambda_i)}{\sqrt{\sum_i f^2(\lambda_i) \sum_i m^2(\lambda_i)}}$$

---

## Benchmark Figures to Replicate

1. **Figure 1**: 2D Cross-correlation map $CCF(v_K, V_{\text{sys}})$ centered at orbital velocity $v_K = 140$ km/s.
2. **Figure 2**: 1D Cross-correlation slice $CCF(v)$ vs systemic velocity offset $V_{\text{sys}}$ (-100 to +100 km/s).
