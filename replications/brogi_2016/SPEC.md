# Replication Specification: Brogi et al. (2016)
**Title**: Rotation and Winds of Exoplanet HD 189733b from High-Resolution Spectroscopy  
**Authors**: Matteo Brogi, Ernst J. W. de Kok, et al.  
**Journal**: ApJ, 817, 106 (2016) | **arXiv**: `1512.03058`

---

## Executive Summary & Core Equations

Brogi et al. (2016) resolve equatorial jetstream winds ($v_{\text{wind}} = -1.9$ km/s) and rotational broadening ($v_{\text{rot}}\sin i = 3.4$ km/s) in HD 189733b using high-resolution CRyogenic InfraRed Echelle Spectrograph (CRIRES) spectra.

### 1. Rotational Broadening Kernel
$$K(v) = \frac{2 (1 - \epsilon) \sqrt{1 - (v/v_{\text{rot}})^2} + \frac{1}{2} \pi \epsilon (1 - (v/v_{\text{rot}})^2)}{\pi v_{\text{rot}} (1 - \epsilon / 3)}$$

---

## Benchmark Figures to Replicate

1. **Figure 1**: Day-to-night wind blueshift $V_{\text{wind}}$ cross-correlation peak vs velocity offset (-5 to +5 km/s).
2. **Figure 2**: CCF S/N broadening metric vs rotational velocity $v_{\text{rot}}\sin i$ (0 to 10 km/s).
