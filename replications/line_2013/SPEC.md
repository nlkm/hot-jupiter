# Replication Specification: Line et al. (2013)
**Title**: A Systematic Retrieval Analysis of Secondary Eclipse Spectra I: Terrestrial & Gas Giant Planets  
**Authors**: Michael R. Line, P. Kopparapu, Y. L. Yung, et al.  
**Journal**: The Astrophysical Journal (ApJ), 775, 137 (2013) | **arXiv**: `1304.5561`

---

## Executive Summary & Core Equations

Line et al. (2013) establish systematic Bayesian MCMC retrieval of atmospheric chemical mixing ratios $X_i$ ($H_2O, CO, CO_2, CH_4$) from secondary eclipse spectra.

### 1. Multi-Gas Monochromatic Opacity & Eclipse Spectrum Formula
$$\kappa_\lambda(P) = \sum_i X_i \sigma_{i,\lambda}(T(P), P)$$
$$F_{\text{planet}}(\lambda) = 2\pi \int_0^1 B_\lambda(T(\mu)) \mu \, d\mu$$

---

## Benchmark Figures to Replicate

1. **Figure 1**: Posterior probability distribution $P(\log_{10} X_i)$ of chemical mixing ratios.
2. **Figure 2**: Best-fit secondary eclipse spectrum $F_{\text{planet}} / F_{\text{star}}(\lambda)$ [$\mu$m].
