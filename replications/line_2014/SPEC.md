# Replication Specification: Line et al. (2014)
**Title**: A Systematic Retrieval Analysis of Secondary Eclipse Spectra II: Hot Jupiters  
**Authors**: Michael R. Line, P. Kopparapu, Y. L. Yung, et al.  
**Journal**: The Astrophysical Journal (ApJ), 783, 70 (2014) | **arXiv**: `1401.3787`

---

## Executive Summary & Core Equations

Line et al. (2014) apply systematic atmospheric retrieval to a sample of 9 hot Jupiters (including WASP-43b and HD 209458b) to infer thermal structure $T(P)$ and metallicity $Z$.

### 1. Thermal Structure & Eclipse Emission Spectrum Formula
$$T(P) = T_0 + \Delta T \log_{10}(P/\text{1 bar})$$
$$F_{\text{planet}}(\lambda) = 2\pi \int_0^1 B_\lambda(T(P(\mu))) \mu \, d\mu$$

---

## Benchmark Figures to Replicate

1. **Figure 1**: Retrieved thermal profile $T(P)$ median and 1-$\sigma$ envelope for WASP-43b [K] vs pressure $P$ [bar].
2. **Figure 2**: Secondary eclipse spectrum planet-to-star flux ratio $F_{\text{planet}} / F_{\star}(\lambda)$ [\%] vs wavelength [$\mu$m].
