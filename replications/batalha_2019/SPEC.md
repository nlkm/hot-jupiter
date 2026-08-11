# Replication Specification: Batalha et al. (2019)
**Title**: PandExo: A Community Tool for Transiting Exoplanet JWST Observation Planning  
**Authors**: Natasha E. Batalha, Joseph D. Mandell, Thomas P. Greene, et al.  
**Journal**: The Astrophysical Journal (ApJ), 878, 70 (2019) | **arXiv**: `1903.04505`

---

## Executive Summary & Core Equations

Batalha et al. (2019) present PandExo, the community simulator for JWST exoplanet transiting spectroscopy, calculating wavelength-dependent noise precision $\sigma_{(R_p/R_\star)^2}(\lambda)$.

### 1. JWST Transmission Noise Floor Formula
$$\sigma_{(R_p/R_\star)^2}(\lambda) = \frac{\sqrt{2}}{S/N(\lambda)}$$

---

## Benchmark Figures to Replicate

1. **Figure 1**: Simulated JWST NIRSpec G395H noise floor $\sigma_{(R_p/R_\star)^2}$ [ppm] vs wavelength $\lambda$ [$\mu$m] (2.8 to 5.2 $\mu$m).
2. **Figure 2**: Signal-to-Noise Ratio (SNR) vs host star magnitude $J$ ($J = 6$ to $12$).
