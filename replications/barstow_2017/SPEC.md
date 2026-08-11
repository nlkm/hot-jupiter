# Replication Specification: Barstow et al. (2017)
**Title**: A Consistent Retrieval Analysis of 10 Hot Jupiters  
**Authors**: J. K. Barstow, S. Aumann, me. Irwin, et al.  
**Journal**: Monthly Notices of the Royal Astronomical Society (MNRAS), 464, 1728 (2017) | **arXiv**: `1609.04354`

---

## Executive Summary & Core Equations

Barstow et al. (2017) present a uniform atmospheric retrieval analysis of 10 hot Jupiters observed with HST WFC3/STIS and Spitzer, determining cloud top pressures $P_{\text{cloud}}$ and chemical abundances.

### 1. Cloud-Top Transmission Depth Formula
$$\Delta \left(\frac{R_p}{R_\star}\right)^2 = \frac{2 R_p H}{R_\star^2} \ln\left(1 + \frac{P_0}{P_{\text{cloud}}}\right)$$

---

## Benchmark Figures to Replicate

1. **Figure 1**: HD 209458b transmission spectrum $(R_p / R_\star)^2$ [%] vs wavelength $\lambda$ [$\mu$m] (0.3 to 5.0 $\mu$m).
2. **Figure 2**: Cloud top pressure $\log_{10} P_{\text{cloud}}$ [bar] vs equilibrium temperature $T_{\text{eq}}$ [K] across the 10-planet sample.
