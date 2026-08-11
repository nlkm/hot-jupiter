# Replication Specification: Molaverdikhani et al. (2019)
**Title**: The Influence of Dispersing Clouds on Exoplanet Transmission Spectra  
**Authors**: Karan Molaverdikhani, Th. Henning, et al.  
**Journal**: A&A, 630, A131 (2019) | **arXiv**: `1908.06450`

---

## Executive Summary & Core Equations

Molaverdikhani et al. (2019) model how cloud particle size distributions $r_{\text{eff}}$ and cloud top pressure $P_{\text{cloud}}$ shape transmission spectra and Rayleigh slopes in hot Jupiter atmospheres.

### 1. Cloud Extinction & Transmission Transit Depth
$$\tau_{\text{cloud}}(\lambda) = \int Q_{\text{ext}}(\lambda, r) \pi r^2 \frac{dN}{dr} dz$$
$$\left(\frac{R_p}{R_\star}\right)^2(\lambda) = \left(\frac{R_0}{R_\star}\right)^2 + \frac{2 R_0 H}{R_\star^2} \ln\left[1 + \tau_{\text{gas}}(\lambda) + \tau_{\text{cloud}}(\lambda)\right]$$

---

## Benchmark Figures to Replicate

1. **Figure 1**: Cloud-influenced transmission spectra $(R_p/R_\star)^2$ vs wavelength $\lambda$ (0.3 to 5.0 $\mu$m) for particle sizes $r_{\text{eff}} = 0.01, 0.1, 1.0\,\mu$m.
2. **Figure 2**: Spectral Rayleigh slope $d(R_p/R_\star)/d\ln\lambda$ vs cloud deck pressure $P_{\text{cloud}}$ ($10^{-4}$ to $1.0$ bar).
