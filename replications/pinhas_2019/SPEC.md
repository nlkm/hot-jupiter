# Replication Specification: Pinhas et al. (2019)
**Title**: H2O Abundances and Cloud Properties in 10 Hot Jupiter Atmospheres  
**Authors**: Arazi Pinhas, Nikku Madhusudhan, Siddharth Gandhi, et al.  
**Journal**: Monthly Notices of the Royal Astronomical Society (MNRAS), 482, 1485 (2019) | **arXiv**: `1808.01283`

---

## Executive Summary & Core Equations

Pinhas et al. (2019) conduct atmospheric retrievals of 10 HST/Spitzer hot Jupiters, constraining water volume mixing ratios $\log_{10} X_{\text{H2O}}$ and non-uniform cloud fraction $a_c$.

### 1. Partial Cloud Coverage Spectrum Formula
$$\left(\frac{R_p}{R_\star}\right)^2(\lambda) = (1 - a_c) \left(\frac{R_p}{R_\star}\right)_{\text{clear}}^2(\lambda) + a_c \left(\frac{R_p}{R_\star}\right)_{\text{cloudy}}^2(\lambda)$$

---

## Benchmark Figures to Replicate

1. **Figure 1**: WASP-31b transmission spectrum $(R_p / R_\star)^2$ [%] vs wavelength $\lambda$ [$\mu$m] (0.3 to 5.0 $\mu$m).
2. **Figure 2**: Water mixing ratio $\log_{10} X_{\text{H2O}}$ vs equilibrium temperature $T_{\text{eq}}$ [K] across the 10 hot Jupiters.
